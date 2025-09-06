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

void setup() {

  setCpuFrequencyMhz(CPU_FREQUENCY_MHZ);
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("Running");

  preferences.begin(PREFS_NAMESPACE, false); // false = read/write mode

  setup_watchdog();

  strcpy(transmitData[TEMPERATURE_IDX].topic, MQTT_TOPIC_TEMPERATURE);
  strcpy(transmitData[HUMIDITY_IDX].topic, MQTT_TOPIC_HUMIDITY);
  strcpy(transmitData[PRESSURE_IDX].topic, MQTT_TOPIC_PRESSURE);
  strcpy(transmitData[ALTITUDE_IDX].topic, MQTT_TOPIC_ALTITUDE);
  strcpy(transmitData[SOIL_MOISTURE_IDX].topic, MQTT_TOPIC_MOISTURE);
  strcpy(transmitData[SUPPLY_VOLTAGE_IDX].topic, MQTT_TOPIC_SUPPLY_VOLTAGE);
  strcpy(transmitData[ANALOG_PINS_IDX].topic, MQTT_TOPIC_ANALOG_PINS);
  strcpy(transmitData[UPTIME_IDX].topic, MQTT_TOPIC_UPTIME);

  transmitTask_init();
  setup_mqtt(MQTT_BROKER_IP, MQTT_BROKER_PORT, DEVICE_ID, MQTT_TOPIC_MANAGEMENT);
  sensorTask_init();

  #ifdef WAKE_LED
  pinMode(WAKE_LED_PIN, OUTPUT);
  #endif
}


void loop() {
  upon_wake();
  enter_deep_sleep();
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

  // Read all connected sensors
  #if SIMULATION_MODE == true
  stubReadSensors(&(transmitData[SOIL_MOISTURE_IDX].data), &(transmitData[TEMPERATURE_IDX].data), &(transmitData[HUMIDITY_IDX].data), &(transmitData[PRESSURE_IDX].data), &(transmitData[ALTITUDE_IDX].data), &(transmitData[SUPPLY_VOLTAGE_IDX].data), &(transmitData[UPTIME_IDX].data), &(transmitData[ANALOG_PINS_IDX].data) );
  #else
  readSensors(&(transmitData[SOIL_MOISTURE_IDX].data), &(transmitData[TEMPERATURE_IDX].data), &(transmitData[HUMIDITY_IDX].data), &(transmitData[PRESSURE_IDX].data), &(transmitData[ALTITUDE_IDX].data), &(transmitData[SUPPLY_VOLTAGE_IDX].data), &(transmitData[UPTIME_IDX].data), &(transmitData[ANALOG_PINS_IDX].data) );
  #endif

  pat_watchdog();

  transmitTask_run(transmitData);

  wake_led_off();

  return;
}

void enter_deep_sleep() {

  // Record when we're going to sleep
  #ifdef UPTIME_MONITORING
  device_uptime += DEFAULT_SLEEP_TIME_SECONDS; // Convert to seconds
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
