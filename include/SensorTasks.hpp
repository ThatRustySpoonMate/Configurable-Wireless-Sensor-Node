#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SOIL_MOISUTRE_SENS_VCC 7
#define SOIL_MOISTURE_SENS_DIN 8 // 5 - ADC1_CH4 for ESP32 Dev Board

#define SEALEVELPRESSURE_HPA (1013.25)

void initSensors();

void readSensors(uint16_t *moistureReading, float *temp, float *humidity, float *baroPres, float *altitude);