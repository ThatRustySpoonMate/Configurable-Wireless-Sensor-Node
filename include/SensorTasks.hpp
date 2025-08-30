#ifndef SENSOR_TASK_HPP
#define SENSOR_TASK_HPP

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "MQTTTasks.hpp"
#include "configuration.h"  // Added configuration include

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

/* Device configuration moved to configuration.h 
   Uncomment the devices you have in configuration.h:
   - DEVICE_BME280
   - DEVICE_SOIL_MOISTURE_SENSOR  
   - SUPPLY_MONITORING
*/

/* Configure connected devices here */
#ifdef DEVICE_SOIL_MOISTURE_SENSOR
uint32_t init_soil_sensor();
#endif

#ifdef DEVICE_BME280
// No additional config needed
#endif

#ifdef SUPPLY_MONITORING
// No additional config needed
#endif

/* For other tasks to see what is connected */

#if defined(DEVICE_BME280)
#define TEMPERATURE_SENSOR_CONNECTED
#define HUMIDITY_SENSOR_CONNECTED
#define PRESSURE_SENSOR_CONNECTED
#define ALTITUDE_SENSOR_CONNECTED
#endif

#if defined(DEVICE_SOIL_MOISTURE_SENSOR)
#define SOIL_MOISTURE_SENSOR_CONNECTED
#endif

#ifdef SUPPLY_MONITORING
#define SUPPLY_MONITORING_CONNECTED
#endif


void sensorTask_init();

void stubReadSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v, transmit_data_t *uptime);

void readSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v, transmit_data_t *uptime);

uint16_t readADCAveraged(uint8_t adc_pin, uint8_t samples);

#endif