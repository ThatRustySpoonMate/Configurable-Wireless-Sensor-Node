/*
  NOTE: If WIFI Fails to connect, you either have the credentials incorrect, or the DHT data pin is on an invalid GPIO port
*/

/* Includes */
#include <Arduino.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>

#include "main.h"
#include "credentials.h"
#include "MQTTTasks.hpp"
#include "InputTask.hpp"

/* Variable Declarations*/
static const uint32_t SAMPLING_INTERVAL = 2000; // ms
static const uint32_t TRANSMIT_INTERVAL = 5000; // ms

float payload[3]; // MQTT Data to be transmitted

bool debug_log = false;

DHT dht(DHTPIN, DHTTYPE); // DHT Sensor


/* Arduino Framework base functions */
void setup() {
  Serial.begin(115200);
  dht.begin();

  setup_wifi(WIFI_SSID, WIFI_PASSWORD);
  

  setup_mqtt(MQTT_BROKER_IP, MQTT_BROKER_PORT, DEVICE_ID, MQTT_MANAGEMENT_TOPIC);

}

void loop() {

  // Transmit readings
  transmitTask();
  
  // Read DHT Sensor
  sampleTask();

  // Handle serial input
  serialTask();

}

/* Function Definitions*/

void sampleTask() {
  static uint32_t lastSampleTime = 0;

  uint32_t currentTime = millis();

  // Only run once per x timeframe 
  if(currentTime > lastSampleTime + SAMPLING_INTERVAL) {
    // Take a new sample - can take a substantial amount of time - maybe split each read over set timespan
    payload[0] = dht.readTemperature();
    payload[1] = dht.readHumidity();
    payload[2] = dht.computeHeatIndex(false);

    if(debug_log) {
      Serial.println("Polled the following values from DHT11 sensor:");Serial.print("Temp: ");Serial.println(payload[0]);Serial.print("Humidity: ");Serial.println(payload[1]);Serial.print("Heat Index: ");Serial.println(payload[2]); 
    }

    lastSampleTime = currentTime;
  }


  return;

}

void transmitTask() {
  static uint32_t lastTransmitTime = 0;

  uint32_t currentTime = millis();

  mqtt_keep_alive(); // Keep MQTT session alive


  // Only run once per x timeframe 
  if(millis() > lastTransmitTime + TRANSMIT_INTERVAL) {
    // Transmit payload via MQTT
    char temperatureString[8], humidityString[8], heatIndexString[8];

    // Convert floats to strings
    dtostrf(payload[Payload_Temperature], 2, 2, temperatureString);
    dtostrf(payload[Payload_Humidity], 2, 2, humidityString);
    dtostrf(payload[Payload_Heat_Index], 2, 2, heatIndexString);

    // Transmit strings
    mqtt_transmit(MQTT_TOPIC_TEMP, temperatureString);
    mqtt_transmit(MQTT_TOPIC_HUMI, humidityString);
    mqtt_transmit(MQTT_TOPIC_HEATINDEX, heatIndexString);

    if(debug_log) {
      Serial.println("Transmitted the following values to MQTT broker:"); Serial.println(temperatureString); Serial.println(humidityString); Serial.println(heatIndexString); 
    }
    
    lastTransmitTime = currentTime;
  }
  
}

