#include "main.hpp"
#include "credentials.h"
#include "MQTTTasks.hpp"
#include "SensorTasks.hpp"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

uint32_t time_to_sleep = 30;      /* Time ESP32 will sleep for (in seconds) */

bool debug_log = false;

void setup() {

  setCpuFrequencyMhz(80);

  Serial.begin(115200);

  Serial.println("Running");

  initSensors();

  setup_mqtt(MQTT_BROKER_IP, MQTT_BROKER_PORT, DEVICE_ID, MQTT_MANAGEMENT_TOPIC);

  pinMode(LED_BUILTIN, OUTPUT);

}


void loop() {
  esp_sleep_enable_timer_wakeup(time_to_sleep * uS_TO_S_FACTOR);
  upon_wake();
  esp_deep_sleep_start();
}


void upon_wake() {
  static uint16_t moistureReadingRaw;
  static float tempReadingRaw, humidityReadingRaw, pressureReadingRaw, altitudeReadingRaw;
  static String moistureReadingStr, tempReadingStr, humidityReadingStr, pressureReadingStr, altitudeReadingStr;
  // LED On
  digitalWrite(LED_BUILTIN, HIGH);

  // Connect to WIFI
  setup_wifi(WIFI_SSID, WIFI_PASSWORD);

  // Connect to MQTT
  mqtt_reconnect();

  // Read soil moisture and BME280 sensors
  readSensors(&moistureReadingRaw, &tempReadingRaw, &humidityReadingRaw, &pressureReadingRaw, &altitudeReadingRaw);

  /* Convert readings to string objects */
  moistureReadingStr = String(moistureReadingRaw);
  tempReadingStr     = String(tempReadingRaw);
  humidityReadingStr = String(humidityReadingRaw);
  pressureReadingStr = String(pressureReadingRaw);
  altitudeReadingStr = String(altitudeReadingRaw);

  /* Transmit Soil moisture reading to MQTT Broker */
  mqtt_transmit(MQTT_TOPIC_MOISTURE, moistureReadingStr.c_str());

  /* Transmit BME280 Readings to MQTT Broker */
  mqtt_transmit(MQTT_TOPIC_TEMPERATURE, tempReadingStr.c_str()); 
  mqtt_transmit(MQTT_TOPIC_HUMIDITY, humidityReadingStr.c_str()); 
  mqtt_transmit(MQTT_TOPIC_PRESSURE, pressureReadingStr.c_str()); 
  mqtt_transmit(MQTT_TOPIC_ALTITUDE, altitudeReadingStr.c_str()); 

  // Disconnect WIFI & MQTT
  delay(500); // Allow time to transmit message before disconnecting
  mqtt_disconnect();
  //wifi_disconnect();

  digitalWrite(LED_BUILTIN, LOW);

  return;
}






// void transmitTask() {
//   static uint32_t lastTransmitTime = 0;

//   uint32_t currentTime = millis();

//   mqtt_keep_alive(); // Keep MQTT session alive


//   // Only run once per x timeframe 
//   if(millis() > lastTransmitTime + TRANSMIT_INTERVAL) {
//     // Transmit payload via MQTT
//     char temperatureString[8], humidityString[8], heatIndexString[8];

//     // Convert floats to strings
//     dtostrf(payload[Payload_Temperature], 2, 2, temperatureString);
//     dtostrf(payload[Payload_Humidity], 2, 2, humidityString);
//     dtostrf(payload[Payload_Heat_Index], 2, 2, heatIndexString);

//     // Transmit strings
//     mqtt_transmit(MQTT_TOPIC_TEMP, temperatureString);
//     mqtt_transmit(MQTT_TOPIC_HUMI, humidityString);
//     mqtt_transmit(MQTT_TOPIC_HEATINDEX, heatIndexString);

//     if(debug_log) {
//       Serial.println("Transmitted the following values to MQTT broker:"); Serial.println(temperatureString); Serial.println(humidityString); Serial.println(heatIndexString); 
//     }
    
//     lastTransmitTime = currentTime;
//   }
  
// }

