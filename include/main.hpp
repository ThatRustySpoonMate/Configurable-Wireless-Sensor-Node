#ifndef MAIN_HPP
#define MAIN_HPP

#include <Arduino.h>
#include <Preferences.h>
#include "configuration.h"
#include "device.hpp"

#define MY_DEBUG_PRINT(_STRING_) if(debug_log) { Serial.print(_STRING_); }
#define MY_DEBUG_PRINTLN(_STRING_) if(debug_log) { Serial.println(_STRING_); }

#define LOCATION_SLUG_MAX_LENGTH 64

extern bool debug_log;
extern uint32_t time_to_sleep;
extern uint32_t device_uptime;
extern Preferences preferences;
extern char location_slug[LOCATION_SLUG_MAX_LENGTH];

typedef enum data_points_e {
    TEMPERATURE_IDX = 0,
    HUMIDITY_IDX,
    PRESSURE_IDX,
    ALTITUDE_IDX,
    SOIL_MOISTURE_IDX,
    SUPPLY_VOLTAGE_IDX,
    UPTIME_IDX,
    ANALOG_PINS_IDX,
    WIFI_RSSI_IDX,
    AQI_IDX,
    TVOC_IDX,
    CO2_IDX,


    DATAPOINTS_NUM
} data_points_t;

// State management
typedef enum {
    STATE_WAKE_UP,
    STATE_WAITING_FOR_TRANSMISSION,
    STATE_READY_TO_SLEEP
} device_state_t;

void upon_wake();
void setup_watchdog();
void pat_watchdog();
void enter_deep_sleep();
void error_handler();

inline void wake_led_on();
inline void wake_led_off();

#endif