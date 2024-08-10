#ifndef SENSOR_TASK_HPP
#define SENSOR_TASK_HPP

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "MQTTTasks.hpp"

typedef union transmit_data_u {
    uint8_t data_u8;
    uint16_t data_u16;
    uint32_t data_u32;
    int8_t data_i8;
    int16_t data_i16;
    int32_t data_i32;
    float data_f32;
    double data_d64;
} transmit_data_t;

typedef struct transmit_data_entry_s {
    transmit_data_t data;
    char topic[MQTT_TOPIC_LENGTH_MAX];

} transmit_data_entry_t;

/* Define connected devices here */
#define SOIL_MOISTURE_SENSOR
#define BME280


/* Configure connected devices here */
#ifdef SOIL_MOISTURE_SENSOR
#define SOIL_MOISUTRE_SENS_VCC 7
#define SOIL_MOISTURE_SENS_DIN 8 // 5 - ADC1_CH4 for ESP32 Dev Board
#define SOIL_MOISTURE_SETTLE_TIME 1000
uint32_t init_soil_sensor();
#endif

#ifdef BME280
uint32_t init_bme280();
void read_bme280(transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude);
#endif

#define SEALEVELPRESSURE_HPA (1013.25)



void sensorTask_init();

void readSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude);

#endif