#include "main.hpp"
#include "credentials.h"
#include "MQTTTasks.hpp"
#include "SensorTasks.hpp"
#include "TransmitTask.hpp"
#include "esp_task_wdt.h" 
#include "SerialTask.hpp"
#include "dev_config.hpp"



Preferences preferences;

// These variables are set during first time setup and can be reconfigured later
char location_slug[LOCATION_SLUG_MAX_LENGTH]; /* MQTT Location slug that prefixes all topics*/ 
String WIFI_SSID, WIFI_PASSWORD, MQTT_BROKER_IP, DEVICE_NAME; /* Credentials */
uint16_t MQTT_BROKER_PORT; /* Credentials */

// Data reading array to store sensor data and transmit it 
transmit_data_entry_t transmitData[DATAPOINTS_NUM];

// Stateful device information
device_state_t device_state;

volatile bool factoryResetRequested = false;
void IRAM_ATTR factoryResetSignalled();

void setup() {
  device_state.current_state = STATE_WAKE_UP;
  device_state.debug_log = DEBUG_DEFAULT_STATE;

  #ifdef LED_PIN
  pinMode(LED_PIN, OUTPUT);
  #endif

  #ifdef HARDWARE_FACTORY_RESET
  pinMode(FACTORY_RESET_PIN, INPUT_PULLDOWN);
  delay(50);
  attachInterrupt(digitalPinToInterrupt(FACTORY_RESET_PIN), factoryResetSignalled, RISING);
  #endif

  setCpuFrequencyMhz(CPU_FREQUENCY_MHZ);
  serial_setup();

  preferences.begin(PREFS_NAMESPACE, false); // false = read/write mode

  load_config(&DEVICE_NAME, &(device_state.time_to_sleep), &WIFI_SSID, &WIFI_PASSWORD, &MQTT_BROKER_IP, &MQTT_BROKER_PORT);

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
  if(factoryResetRequested) {
    preferences.putBool(FIRST_SETUP_KEY, false);
    preferences.end();
    MY_DEBUG_PRINTLN("Factory Reset Initialized.")
    Serial.flush();

    ESP.restart();
  }

  switch (device_state.current_state) {
    case STATE_WAKE_UP:
      upon_wake();
      device_state.current_state = STATE_WAITING_FOR_TRANSMISSION;
      break;
        
    case STATE_WAITING_FOR_TRANSMISSION:
      pat_watchdog(); // Keep watchdog happy while waiting
      
      mqtt_keep_alive();

      // Check if transmission buffer time has elapsed
      if (transmitTask_isReadyForSleep()) {
          MY_DEBUG_PRINTLN("Ready to enter deep sleep");
          device_state.current_state = STATE_READY_TO_SLEEP;
      }
      
      // Small delay to prevent tight polling loop
      delay(10);
      break;
        
    case STATE_READY_TO_SLEEP:
      MY_WAKE_LED_OFF();
      enter_deep_sleep();
      //delay(30000);
      // This point should never be reached as deep sleep resets the device
      device_state.current_state = STATE_WAKE_UP; // Just in case
      break;
    
    case STATE_IDENTIFY:
      // Leave once configured time is up
      if(millis() - device_state.identify_entered_time > device_state.identify_duration){
        MY_DEBUG_PRINTLN("Identify state ended.");
        device_state.current_state = STATE_READY_TO_SLEEP;
        return;
      }
      
      pat_watchdog(); // Keep watchdog happy while waiting
      mqtt_keep_alive();
      
      MY_STATE_LED_TOGGLE();
      delay(IDENTIFY_STATE_LED_DURATION);

      

      break;
  }
}


void upon_wake() {

  // LED On
  MY_WAKE_LED_ON();

  pat_watchdog();

  // Connect to WIFI
  if (!setup_wifi_with_timeout(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str(), WIFI_CONNECT_TIMEOUT_MS)) { // 30 second timeout
    MY_DEBUG_PRINTLN("WiFi connection failed - entering deep sleep");
    MY_WAKE_LED_OFF();
    enter_deep_sleep();
    return;
  }

  pat_watchdog(); 

  // Connect to MQTT
  if (!mqtt_reconnect_with_timeout(MQTT_CONNECT_TIMEOUT_MS)) { // 10 second timeouts
    MY_DEBUG_PRINTLN("MQTT connection failed - entering deep sleep");
    MY_WAKE_LED_OFF();
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

  device_state.device_uptime += (millis() / 1000);
  device_state.device_uptime += device_state.time_to_sleep; // Convert to seconds
  preferences.putULong(UPTIME_KEY, device_state.device_uptime);
  #endif

  // If interval is 0, then don't go to sleep
  if(device_state.time_to_sleep == 0) {
    device_state.current_state = STATE_WAKE_UP; // Just in case
    return;
  }

  // Close preferences to ensure pending writes are complete
  preferences.end();

  // Disable WiFi and Bluetooth to save power
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();
  
  // Configure wake sources
  esp_sleep_enable_timer_wakeup(device_state.time_to_sleep * uS_TO_S_FACTOR);
  
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

void error_handler() {
  while(true){
    MY_STATE_LED_TOGGLE();
    delay(ERROR_STATE_LED_DURATION);
  }

  return;
}

// Interrupt that is called when a factory reset is commanded via shorting GPIO pin high
void IRAM_ATTR factoryResetSignalled() {
  factoryResetRequested = true;

  return;
}