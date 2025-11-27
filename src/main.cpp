#include "main.hpp"
#include "credentials.h"
#include "MQTTTasks.hpp"
#include "SensorTasks.hpp"
#include "TransmitTask.hpp"
#include "esp_task_wdt.h" 
#include "SerialTask.hpp"


uint32_t time_to_sleep = DEFAULT_SLEEP_TIME_SECONDS;    /* Time ESP32 will sleep for between readings (in seconds) */
bool debug_log = DEBUG_DEFAULT_STATE;
char location_slug[LOCATION_SLUG_MAX_LENGTH];

Preferences preferences;
uint32_t device_uptime = 0;
String WIFI_SSID, WIFI_PASSWORD, MQTT_BROKER_IP, DEVICE_NAME;
uint16_t MQTT_BROKER_PORT;

// Data reading array to store sensor data and transmit it 
transmit_data_entry_t transmitData[DATAPOINTS_NUM];

static device_state_t current_state = STATE_WAKE_UP;

void first_time_setup();

void setup() {

  #ifdef WAKE_LED
  pinMode(WAKE_LED_PIN, OUTPUT);
  #endif

  setCpuFrequencyMhz(CPU_FREQUENCY_MHZ);
  serial_setup();
  Serial.println("Running");
  Serial.flush();

  preferences.begin(PREFS_NAMESPACE, false); // false = read/write mode

  load_config();

  setup_watchdog();

  transmitTask_init();
  setup_mqtt(MQTT_BROKER_IP.c_str(), MQTT_BROKER_PORT, DEVICE_NAME.c_str());

  // Build transmit data topics
  strcpy(transmitData[TEMPERATURE_IDX].topic, MQTT_TOPIC_TEMPERATURE);
  strcpy(transmitData[HUMIDITY_IDX].topic, MQTT_TOPIC_HUMIDITY);
  strcpy(transmitData[AQI_IDX].topic, MQTT_TOPIC_AQI);
  strcpy(transmitData[TVOC_IDX].topic, MQTT_TOPIC_TVOC);
  strcpy(transmitData[CO2_IDX].topic, MQTT_TOPIC_CO2);
  strcpy(transmitData[PRESSURE_IDX].topic, MQTT_TOPIC_PRESSURE);
  strcpy(transmitData[ALTITUDE_IDX].topic, MQTT_TOPIC_ALTITUDE);
  strcpy(transmitData[SOIL_MOISTURE_IDX].topic, MQTT_TOPIC_MOISTURE);
  strcpy(transmitData[SUPPLY_VOLTAGE_IDX].topic, MQTT_TOPIC_SUPPLY_VOLTAGE);
  strcpy(transmitData[ANALOG_PINS_IDX].topic, MQTT_TOPIC_ANALOG_PINS);
  strcpy(transmitData[UPTIME_IDX].topic, MQTT_TOPIC_UPTIME);
  strcpy(transmitData[WIFI_RSSI_IDX].topic, MQTT_TOPIC_WIFI_RSSI);

  // Reset transmission state on startup
  transmitTask_reset();
}


void loop() {
  switch (current_state) {
    case STATE_WAKE_UP:
      upon_wake();
      current_state = STATE_WAITING_FOR_TRANSMISSION;
      break;
        
    case STATE_WAITING_FOR_TRANSMISSION:
      pat_watchdog(); // Keep watchdog happy while waiting
      
      mqtt_keep_alive();

      // Check if transmission buffer time has elapsed
      if (transmitTask_isReadyForSleep()) {
          MY_DEBUG_PRINTLN("Ready to enter deep sleep");
          current_state = STATE_READY_TO_SLEEP;
      }
      
      // Small delay to prevent tight polling loop
      delay(10);
      break;
        
    case STATE_READY_TO_SLEEP:
      wake_led_off();
      enter_deep_sleep();
      //delay(30000);
      // This point should never be reached as deep sleep resets the device
      current_state = STATE_WAKE_UP; // Just in case
      break;
  }
}


void upon_wake() {

  // LED On
  wake_led_on();

  pat_watchdog();

  // Connect to WIFI
  if (!setup_wifi_with_timeout(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str(), WIFI_CONNECT_TIMEOUT_MS)) { // 30 second timeout
    MY_DEBUG_PRINTLN("WiFi connection failed - entering deep sleep");
    wake_led_off();
    enter_deep_sleep();
    return;
  }

  pat_watchdog(); 

  // Connect to MQTT
  if (!mqtt_reconnect_with_timeout(MQTT_CONNECT_TIMEOUT_MS)) { // 10 second timeouts
    MY_DEBUG_PRINTLN("MQTT connection failed - entering deep sleep");
    wake_led_off();
    enter_deep_sleep();
    return;
  }

  pat_watchdog(); 

  // Init sensors after WiFi and MQTT connections are made for logging purposes
  sensorTask_init();

  // Read all connected sensors
  #if SIMULATION_MODE == true
  stubReadSensors(&(transmitData[SOIL_MOISTURE_IDX].data), &(transmitData[TEMPERATURE_IDX].data), &(transmitData[HUMIDITY_IDX].data), &(transmitData[PRESSURE_IDX].data), &(transmitData[ALTITUDE_IDX].data), &(transmitData[SUPPLY_VOLTAGE_IDX].data), &(transmitData[UPTIME_IDX].data), &(transmitData[ANALOG_PINS_IDX].data), &(transmitData[WIFI_RSSI_IDX].data), &(transmitData[AQI_IDX].data), &(transmitData[TVOC_IDX].data), &(transmitData[CO2_IDX].data) );
  #else
  readSensors(&(transmitData[SOIL_MOISTURE_IDX].data), &(transmitData[TEMPERATURE_IDX].data), &(transmitData[HUMIDITY_IDX].data), &(transmitData[PRESSURE_IDX].data), &(transmitData[ALTITUDE_IDX].data), &(transmitData[SUPPLY_VOLTAGE_IDX].data), &(transmitData[UPTIME_IDX].data), &(transmitData[ANALOG_PINS_IDX].data), &(transmitData[WIFI_RSSI_IDX].data), &(transmitData[AQI_IDX].data), &(transmitData[TVOC_IDX].data), &(transmitData[CO2_IDX].data));
  #endif

  pat_watchdog();

  transmitTask_run(transmitData);

  return;
}

void enter_deep_sleep() {

  // Record when we're going to sleep
  #ifdef UPTIME_MONITORING
  device_uptime += time_to_sleep; // Convert to seconds
  preferences.putULong(UPTIME_KEY, device_uptime);
  #endif

  // If interval is 0, then don't go to sleep
  if(time_to_sleep == 0) {
    current_state = STATE_WAKE_UP; // Just in case
    return;
  }

  // Close preferences to ensure pending writes are complete
  preferences.end();

  // Disable WiFi and Bluetooth to save power
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();
  
  // Configure wake sources
  esp_sleep_enable_timer_wakeup(time_to_sleep * uS_TO_S_FACTOR);
  
  // Power down peripherals to save battery
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  
  MY_DEBUG_PRINTLN("Entering deep sleep...");
  Serial.flush(); // Make sure debug message is sent
  
  esp_deep_sleep_start();
}


void setup_watchdog() {
  esp_task_wdt_init(WATCHDOG_TIMEOUT_SECONDS, true); // 30 second timeout, panic on timeout
  esp_task_wdt_add(NULL); // Add current task to watchdog
}

void pat_watchdog() {
  esp_task_wdt_reset();
}

inline void wake_led_on() {
  #ifdef WAKE_LED
  digitalWrite(WAKE_LED_PIN, HIGH);
  #endif
}

inline void wake_led_off() {
  #ifdef WAKE_LED
  digitalWrite(WAKE_LED_PIN, LOW);
  #endif
}


void load_config() {

  bool firstTimeSetupComplete = preferences.getBool(FIRST_SETUP_KEY, false);

  if(firstTimeSetupComplete == false) {
    first_time_setup();
  } else {
    MY_DEBUG_PRINT("Skipping first time setup.");
  }

  // Load fts
  MY_DEBUG_PRINT("Loaded first time setup from flash: ");
  MY_DEBUG_PRINTLN(preferences.getBool(FIRST_SETUP_KEY, false));

  // Load Device Name
  DEVICE_NAME = preferences.getString(DEVICE_NAME_KEY);
  MY_DEBUG_PRINT("Loaded device name from flash: ");
  MY_DEBUG_PRINTLN(DEVICE_NAME);

  // Load interval
  uint32_t interval_temp = preferences.getULong(INTERVAL_KEY, 0);
  MY_DEBUG_PRINT("Loaded interval from flash: ");
  MY_DEBUG_PRINTLN(interval_temp);
  time_to_sleep = interval_temp;
  
  // Load location slug from preferences or use default
  String stored_slug = preferences.getString(LOCATION_SLUG_KEY, "");
  
  if(stored_slug.length() > 0) {
    // Location slug has been stored in flash
    strncpy(location_slug, stored_slug.c_str(), LOCATION_SLUG_MAX_LENGTH - 1);
    location_slug[LOCATION_SLUG_MAX_LENGTH - 1] = '\0'; // Ensure null termination
    MY_DEBUG_PRINT("Loaded location slug from flash: ");
    MY_DEBUG_PRINTLN(location_slug);
  } else {
    // No location slug in flash (likely first boot), use default
    strncpy(location_slug, MQTT_TOPIC_LOCATION_SLUG, LOCATION_SLUG_MAX_LENGTH - 1);
    location_slug[LOCATION_SLUG_MAX_LENGTH - 1] = '\0'; // Ensure null termination
    preferences.putString(LOCATION_SLUG_KEY, location_slug);
    MY_DEBUG_PRINT("Using default location slug: ");
    MY_DEBUG_PRINTLN(location_slug);
  }

  // Load WiFI Credentials
  WIFI_SSID = preferences.getString(WIFI_SSID_KEY, "");
  MY_DEBUG_PRINT("Loaded WiFi SSID from flash: ");
  MY_DEBUG_PRINTLN(WIFI_SSID);

  WIFI_PASSWORD = preferences.getString(WIFI_PWD_KEY, "");

  // Load MQTT Credentials
  MQTT_BROKER_IP = preferences.getString(MQTT_BROKER_IP_KEY, "");
  MY_DEBUG_PRINT("Loaded MQTT Broker IP from flash: ");
  MY_DEBUG_PRINTLN(MQTT_BROKER_IP);

  MQTT_BROKER_PORT = preferences.getUShort(MQTT_BROKER_PORT_KEY, 1883);
  MY_DEBUG_PRINT("Loaded MQTT Broker Port from flash: ");
  MY_DEBUG_PRINTLN(MQTT_BROKER_PORT);

  return;

}

String readLineFromSerial() {
  while (Serial.available()) {
    Serial.read(); // Clear any leftover data
  }

  String input = "";
  while (true) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n' || c == '\r') {
        break;
      }

      input += c;
    }
    delay(100);
    digitalWrite(WAKE_LED_PIN, !digitalRead(WAKE_LED_PIN));
  }
  return input;
}

void first_time_setup() {
  digitalWrite(WAKE_LED_PIN, HIGH);
  delay(5000); // Give time for serial monitor to connect


  Serial.println("\n=== First Time Setup ===");
  Serial.println("Please enter the following configuration:");
  
  String input;
  String deviceName, wifiSSID, wifiPassword, mqttBrokerIP, deviceLocation;
  uint32_t wakeInterval;
  uint16_t mqttBrokerPort;

  
  // Read Device Name
  Serial.print("\nDevice Name: ");
  input = readLineFromSerial();
  deviceName = input;
  preferences.putString(DEVICE_NAME_KEY, input);
  Serial.println("Stored: " + input);
  
  // Read WiFi SSID
  Serial.print("\nWiFi SSID: ");
  input = readLineFromSerial();
  wifiSSID = input;
  preferences.putString(WIFI_SSID_KEY, input);
  Serial.println("Stored: " + input);
  
  // Read WiFi Password
  Serial.print("\nWiFi Password: ");
  input = readLineFromSerial();
  wifiPassword = input;
  preferences.putString(WIFI_PWD_KEY, input);
  Serial.println("Stored: " + input);
  
  // Read MQTT Broker IP Address
  Serial.print("\nMQTT Broker IP Address: ");
  input = readLineFromSerial();
  mqttBrokerIP = input;
  preferences.putString(MQTT_BROKER_IP_KEY, input);
  Serial.println("Stored: " + input);
  
  // Read MQTT Broker Port
  Serial.print("\nMQTT Broker Port: ");
  input = readLineFromSerial();
  mqttBrokerPort = input.toInt();
  preferences.putUShort(MQTT_BROKER_PORT_KEY, mqttBrokerPort);
  Serial.println("Stored: " + String(mqttBrokerPort));
  
  // Read Wake Interval
  Serial.print("\nWake Interval (seconds): ");
  input = readLineFromSerial();
  wakeInterval = input.toInt();
  preferences.putULong(INTERVAL_KEY, wakeInterval);
  Serial.println("Stored: " + String(wakeInterval));
  
  // Read Device Location
  Serial.print("\nDevice Location: ");
  input = readLineFromSerial();
  deviceLocation = input;
  preferences.putString(LOCATION_SLUG_KEY, input);
  Serial.println("Stored: " + input);
  
  // Display all configuration and ask for confirmation
  Serial.println("\n=== Configuration Summary ===");
  Serial.println("Device Name: " + deviceName);
  Serial.println("WiFi SSID: " + wifiSSID);
  Serial.println("WiFi Password: " + wifiPassword);
  Serial.println("MQTT Broker IP: " + mqttBrokerIP);
  Serial.println("MQTT Broker Port: " + String(mqttBrokerPort));
  Serial.println("Wake Interval: " + String(wakeInterval) + " seconds");
  Serial.println("Device Location: " + deviceLocation);
  Serial.println("\nIs this configuration correct? (y/n): ");
  
  input = readLineFromSerial();
  input.toLowerCase();
  
  if (input == "y" || input == "yes") {
    // Flag first-time-setup complete - we can skip this function on future startup
    preferences.putBool(FIRST_SETUP_KEY, true);
    digitalWrite(WAKE_LED_PIN, LOW);
    Serial.println("\n=== Setup Complete! ===");
  } else {
    // Clear the preferences and restart
    Serial.println("\n=== Restarting setup... ===");
    digitalWrite(WAKE_LED_PIN, LOW);
    delay(1000);
    ESP.restart();
  }

  return;
}


void error_handler() {
  while(true){
    delay(100);
    wake_led_on();
    delay(100);
    wake_led_off();
  }

  return;
}