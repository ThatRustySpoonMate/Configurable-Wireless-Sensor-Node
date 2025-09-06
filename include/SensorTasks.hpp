#ifndef SENSOR_TASK_HPP
#define SENSOR_TASK_HPP

#include "Arduino.h"
#include <Wire.h>
#include "MQTTTasks.hpp"
#include "configuration.h"

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

void sensorTask_init();

void stubReadSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v, transmit_data_t *uptime, transmit_data_t *analog_pins);

void readSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v, transmit_data_t *uptime, transmit_data_t *analog_pins);

uint16_t readADCAveraged(uint8_t adc_pin, uint8_t samples);

#endif