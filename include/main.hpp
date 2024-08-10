#ifndef MAIN_HPP
#define MAIN_HPP

#include <Arduino.h>

extern bool debug_log;

#define DEBUG_PRINT(_STRING_) if(debug_log) { Serial.print(_STRING_); }
#define DEBUG_PRINTLN(_STRING_) if(debug_log) { Serial.println(_STRING_); }

extern uint32_t time_to_sleep;

typedef enum data_points_e {
    TEMPERATURE_IDX = 0,
    HUMIDITY_IDX,
    PRESSURE_IDX,
    ALTITUDE_IDX,
    SOIL_MOISTURE_IDX,
    SUPPLY_VOLTAGE_IDX,

    DATAPOINTS_NUM
} data_points_t;

void upon_wake();

#endif