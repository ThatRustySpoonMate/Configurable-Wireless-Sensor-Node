/* Includes */
#include <Arduino.h>
#include <DHT.h>
#include "main.h"

/* Variable Declarations*/
DHT dht(DHTPIN, DHTTYPE);

static const uint32_t SAMPLING_INTERVAL = 2000; // ms
static const uint32_t TRANSMIT_INTERVAL = 5000; // ms

float payload[3];

/* Function Declarations */
void sampleTask();
void transmitTask();

/* Arduino Framework base functions */
void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  
  sampleTask();
  transmitTask();

}

/* Function Definitions*/

void sampleTask() {
  static uint32_t lastSampleTime = 0;

  uint32_t currentTime = millis();

  if(currentTime > lastSampleTime + SAMPLING_INTERVAL) {
    // Take a new sample - can take a substantial amount of time - maybe split each read over set timespan
    payload[0] = dht.readTemperature();
    payload[1] = dht.readHumidity();
    payload[2] = dht.computeHeatIndex(false);

    lastSampleTime = currentTime;
  }


  return;

}

void transmitTask() {
  static uint32_t lastTransmitTime = 0;

  uint32_t currentTime = millis();

  if(millis() > lastTransmitTime + TRANSMIT_INTERVAL) {
    // Tranmit payload via MQTT
    
    lastTransmitTime = currentTime;
  }
  
}