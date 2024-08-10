#include "main.hpp"
#include "credentials.h"
#include "MQTTTasks.hpp"
#include "SensorTasks.hpp"
#include "TransmitTask.hpp"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

uint32_t time_to_sleep = 30;      /* Time ESP32 will sleep for (in seconds) */

bool debug_log = false;

transmit_data_entry_t transmitData[DATAPOINTS_NUM];

void setup() {

  setCpuFrequencyMhz(80);

  Serial.begin(115200);

  Serial.println("Running");

  strcpy(transmitData[TEMPERATURE].topic, MQTT_TOPIC_TEMPERATURE);
  strcpy(transmitData[HUMIDITY].topic, MQTT_TOPIC_HUMIDITY);
  strcpy(transmitData[PRESSURE].topic, MQTT_TOPIC_PRESSURE);
  strcpy(transmitData[ALTITUDE].topic, MQTT_TOPIC_ALTITUDE);
  strcpy(transmitData[SOIL_MOISTURE].topic, MQTT_TOPIC_MOISTURE);

  transmitTask_init();

  sensorTask_init();

  setup_mqtt(MQTT_BROKER_IP, MQTT_BROKER_PORT, DEVICE_ID, MQTT_MANAGEMENT_TOPIC);

  pinMode(LED_BUILTIN, OUTPUT);

}


void loop() {
  esp_sleep_enable_timer_wakeup(time_to_sleep * uS_TO_S_FACTOR);
  upon_wake();
  esp_deep_sleep_start();
}


void upon_wake() {
  // LED On
  digitalWrite(LED_BUILTIN, HIGH);

  // Connect to WIFI
  setup_wifi(WIFI_SSID, WIFI_PASSWORD);

  // Connect to MQTT
  mqtt_reconnect();

  // Read soil moisture and BME280 sensors
  readSensors(&(transmitData[SOIL_MOISTURE].data), &(transmitData[TEMPERATURE].data), &(transmitData[HUMIDITY].data), &(transmitData[PRESSURE].data), &(transmitData[ALTITUDE].data));

  transmitTask_run(transmitData);

  digitalWrite(LED_BUILTIN, LOW);

  return;
}


