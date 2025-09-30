#include "main.hpp"
#include "credentials.h"
#include "MQTTTasks.hpp"
#include "SensorTasks.hpp"
#include "TransmitTask.hpp"
#include "esp_task_wdt.h" 


uint32_t time_to_sleep = DEFAULT_SLEEP_TIME_SECONDS;    /* Time ESP32 will sleep for between readings (in seconds) */
bool debug_log = DEBUG_DEFAULT_STATE;

Preferences preferences;
uint32_t device_uptime = 0;

// Data reading array to store sensor data and transmit it 
transmit_data_entry_t transmitData[DATAPOINTS_NUM];

static device_state_t current_state = STATE_WAKE_UP;

void setup() {

  #ifdef WAKE_LED
  pinMode(WAKE_LED_PIN, OUTPUT);
  #endif

  setCpuFrequencyMhz(CPU_FREQUENCY_MHZ);
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("Running");

  preferences.begin(PREFS_NAMESPACE, false); // false = read/write mode

  setup_watchdog();

  strcpy(transmitData[TEMPERATURE_IDX].topic, MQTT_TOPIC_TEMPERATURE);
  strcpy(transmitData[HUMIDITY_IDX].topic, MQTT_TOPIC_HUMIDITY);
  strcpy(transmitData[AQI_IDX].topic, MQTT_TOPIC_AQI);
  strcpy(transmitData[TVOC_IDX].topic, MQTT_TOPIC_TVOC);
  strcpy(transmitData[ECO2_IDX].topic, MQTT_TOPIC_ECO2);
  strcpy(transmitData[PRESSURE_IDX].topic, MQTT_TOPIC_PRESSURE);
  strcpy(transmitData[ALTITUDE_IDX].topic, MQTT_TOPIC_ALTITUDE);
  strcpy(transmitData[SOIL_MOISTURE_IDX].topic, MQTT_TOPIC_MOISTURE);
  strcpy(transmitData[SUPPLY_VOLTAGE_IDX].topic, MQTT_TOPIC_SUPPLY_VOLTAGE);
  strcpy(transmitData[ANALOG_PINS_IDX].topic, MQTT_TOPIC_ANALOG_PINS);
  strcpy(transmitData[UPTIME_IDX].topic, MQTT_TOPIC_UPTIME);
  strcpy(transmitData[WIFI_RSSI_IDX].topic, MQTT_TOPIC_WIFI_RSSI);

  load_config();

  transmitTask_init();
  setup_mqtt(MQTT_BROKER_IP, MQTT_BROKER_PORT, DEVICE_ID);

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
  if (!setup_wifi_with_timeout(WIFI_SSID, WIFI_PASSWORD, WIFI_CONNECT_TIMEOUT_MS)) { // 30 second timeout
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
  stubReadSensors(&(transmitData[SOIL_MOISTURE_IDX].data), &(transmitData[TEMPERATURE_IDX].data), &(transmitData[HUMIDITY_IDX].data), &(transmitData[PRESSURE_IDX].data), &(transmitData[ALTITUDE_IDX].data), &(transmitData[SUPPLY_VOLTAGE_IDX].data), &(transmitData[UPTIME_IDX].data), &(transmitData[ANALOG_PINS_IDX].data), &(transmitData[WIFI_RSSI_IDX].data), &(transmitData[AQI_IDX].data), &(transmitData[TVOC_IDX].data), &(transmitData[ECO2_IDX].data) );
  #else
  readSensors(&(transmitData[SOIL_MOISTURE_IDX].data), &(transmitData[TEMPERATURE_IDX].data), &(transmitData[HUMIDITY_IDX].data), &(transmitData[PRESSURE_IDX].data), &(transmitData[ALTITUDE_IDX].data), &(transmitData[SUPPLY_VOLTAGE_IDX].data), &(transmitData[UPTIME_IDX].data), &(transmitData[ANALOG_PINS_IDX].data), &(transmitData[WIFI_RSSI_IDX].data), &(transmitData[AQI_IDX].data), &(transmitData[TVOC_IDX].data), &(transmitData[ECO2_IDX].data));
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
  uint32_t interval_temp = preferences.getULong(INTERVAL_KEY, 0);

  if(interval_temp != 0) {
    // Interval has been stored in flash
    time_to_sleep = interval_temp;
  } else{
    // No interval in flash (likely first boot)
    preferences.putULong(INTERVAL_KEY, DEFAULT_SLEEP_TIME_SECONDS);
  }


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