#ifndef SENSOR_TASK_HPP
#define SENSOR_TASK_HPP

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/* Define connected devices here */
#define SOIL_MOISTURE_SENSOR
#define BME280


/* Configure connected devices here */
#ifdef SOIL_MOISTURE_SENSOR
#define SOIL_MOISUTRE_SENS_VCC 7
#define SOIL_MOISTURE_SENS_DIN 8 // 5 - ADC1_CH4 for ESP32 Dev Board
#define SOIL_MOISTURE_SETTLE_TIME 250
uint32_t init_soil_sensor();
#endif

#ifdef BME280
uint32_t init_bme280();
#endif

#define SEALEVELPRESSURE_HPA (1013.25)



void initSensors();

void readSensors(uint16_t *moistureReading, float *temp, float *humidity, float *baroPres, float *altitude);

#endif