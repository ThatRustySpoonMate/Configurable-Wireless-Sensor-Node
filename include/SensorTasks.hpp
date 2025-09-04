#ifndef SENSOR_TASK_HPP
#define SENSOR_TASK_HPP

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "MQTTTasks.hpp"
#include "configuration.h"  // Added configuration include

typedef union transmit_data_u {
    uint8_t data_u8[8];
    uint16_t data_u16[4];
    uint32_t data_u32[2];
    int8_t data_i8[8];
    int16_t data_i16[4];
    int32_t data_i32[2];
    float data_f32[2];
    double data_d64;
} transmit_data_t;

typedef struct transmit_data_entry_s {
    transmit_data_t data;
    char topic[MQTT_TOPIC_LENGTH_MAX];

} transmit_data_entry_t;

/* Device configuration moved to configuration.h 
   Uncomment the devices you have in configuration.h:
   - DEVICE_BME280
   - DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR  
   - DEVICE_AHT20
   - INTERNAL_SUPPLY_MONITORING
*/

/* Configure connected devices here */
#ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR
// No additional public config needed
#endif

#ifdef DEVICE_BME280
#include <Adafruit_BME280.h>
// No additional public config needed
#endif

#ifdef DEVICE_AHT20
#include <Adafruit_AHTX0.h>
// No additional public config needed
#endif

#ifdef INTERNAL_SUPPLY_MONITORING
// No additional public config needed
#endif


void sensorTask_init();

void stubReadSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v, transmit_data_t *uptime);

void readSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v, transmit_data_t *uptime);

uint16_t readADCAveraged(uint8_t adc_pin, uint8_t samples);

#endif