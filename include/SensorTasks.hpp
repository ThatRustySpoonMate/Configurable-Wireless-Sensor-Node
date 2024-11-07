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

/* Define connected devices here
*  Available devices:
*  - DEVICE_BME280
*  - DEVICE_SOIL_MOISTURE_SENSOR
*  - SUPPLY_MONITORING
*/

//#define DEVICE_SOIL_MOISTURE_SENSOR
#define DEVICE_BME280
#define SUPPLY_MONITORING // Resistor divider from supply to ADC Pin


/* Configure connected devices here */
#ifdef DEVICE_SOIL_MOISTURE_SENSOR
#define SOIL_MOISUTRE_SENS_VCC 7
#define SOIL_MOISTURE_SENS_DIN 8 // 5 - ADC1_CH4 for ESP32 Dev Board
#define SOIL_MOISTURE_SETTLE_TIME 1000
uint32_t init_soil_sensor();
#endif

#ifdef DEVICE_BME280

#endif

#ifdef SUPPLY_MONITORING
#define SUPPLY_MON_ADC_PIN 3 // ADC1_CH2 on WEMOS
#define SUPPLY_MON_R1 100000.0 // 100k
#define SUPPLY_MON_R2 100000.0 // 100k
#define SUPPLY_MON_RDIV_RATIO (1.0 / (SUPPLY_MON_R2 / (SUPPLY_MON_R1 + SUPPLY_MON_R2)))
#define SUPPLY_MON_FUDGE_FACTOR_DIV 1.25200
#endif

#define SEALEVELPRESSURE_HPA (1031.0f) // hPa

/* For other tasks to see what is connected */

#if defined(DEVICE_BME280) // || defined(DHT11)
#define TEMPERATURE_SENSOR_CONNECTED
#endif

#if defined(DEVICE_BME280)
#define HUMIDITY_SENSOR_CONNECTED
#endif

#if defined(DEVICE_BME280)
#define PRESSURE_SENSOR_CONNECTED
#endif

#if defined(DEVICE_BME280)
#define ALTITUDE_SENSOR_CONNECTED
#endif

#if defined(DEVICE_SOIL_MOISTURE_SENSOR) // || defined(DHT11)
#define SOIL_MOISTURE_SENSOR_CONNECTED
#endif

#ifdef SUPPLY_MONITORING
#define SUPPLY_MONITORING_CONNECTED
#endif


void sensorTask_init();

void readSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v);

uint16_t readADCAveraged(uint8_t adc_pin, uint8_t samples);

#endif