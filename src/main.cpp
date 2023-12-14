/*
  NOTE: If WIFI Fails to connect, you either have the credentials incorrect, or the DHT data pin is on an invalid GPIO port
*/

/* Includes */
#include <Arduino.h>
#include "main.h"

#include <Adafruit_Sensor.h>

#if SENSOR == 1
#include <DHT.h>
#elif SENSOR == 2
#include <Adafruit_AHTX0.h>
#endif


#include "credentials.h"
#include "MQTTTasks.hpp"
#include "InputTask.hpp"

/* Variable Declarations*/
static const uint32_t SAMPLING_INTERVAL = 10000; // ms
static const uint32_t TRANSMIT_INTERVAL = 10000; // ms

float payload[3]; // MQTT Data to be transmitted

bool debug_log = false;

#if SENSOR == 1
DHT dht(DHTPIN, DHTTYPE); // DHT Sensor
#elif SENSOR == 2
Adafruit_AHTX0 aht;
#endif


/* Arduino Framework base functions */
void setup() {
  setCpuFrequencyMhz(80);


  Serial.begin(115200);

  #if SENSOR == 1
  dht.begin();

  #elif SENSOR == 2
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");

  #endif

  setup_wifi(WIFI_SSID, WIFI_PASSWORD);
  

  setup_mqtt(MQTT_BROKER_IP, MQTT_BROKER_PORT, DEVICE_ID, MQTT_MANAGEMENT_TOPIC);

}

void loop() {

  // Read DHT Sensor
  sampleTask();

  // Transmit readings
  transmitTask();

  // Handle serial input
  serialTask();   

  // For Power efficiency - change to sleep state later
  delay(500); 

}

/* Function Definitions*/

void sampleTask() {
  static uint32_t lastSampleTime = 0;

  uint32_t currentTime = millis();

  float pl_temperature, pl_humidity, pl_heatIndex;

  // Only run once per x timeframe 
  if(currentTime > lastSampleTime + SAMPLING_INTERVAL) {
    // Take a new sample - can take a substantial amount of time - maybe split each read over set timespan
    
    #if SENSOR == 1
    pl_temperature = dht.readTemperature();
    pl_humidity = dht.readHumidity();
    pl_heatIndex = dht.computeHeatIndex(false);

    #elif SENSOR == 2
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
    pl_temperature = temp.temperature;
    pl_humidity = humidity.relative_humidity;
    #endif
    
    payload[0] = pl_temperature;
    payload[1] = pl_humidity;
    payload[2] = pl_heatIndex;

    if(debug_log) {
      Serial.println("Polled the following values:");Serial.print("Temp: ");Serial.println(payload[0]);Serial.print("Humidity: ");Serial.println(payload[1]);Serial.print("Heat Index: ");Serial.println(payload[2]); 
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

