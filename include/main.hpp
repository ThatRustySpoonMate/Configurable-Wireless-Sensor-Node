#ifndef MAIN_HPP
#define MAIN_HPP

#include <Arduino.h>
#include <Preferences.h>
#include "configuration.h"
#include "device.hpp"

#define MY_DEBUG_PRINT(_STRING_) if(device_state.debug_log) { Serial.print(_STRING_); }
#define MY_DEBUG_PRINTLN(_STRING_) if(device_state.debug_log) { Serial.println(_STRING_); }
#ifdef WAKE_LED
#define MY_WAKE_LED_ON() digitalWrite(LED_PIN, HIGH)
#define MY_WAKE_LED_OFF() digitalWrite(LED_PIN, LOW)
#define MY_WAKE_LED_TOGGLE() digitalWrite(LED_PIN, !digitalRead(LED_PIN))
#else
#define MY_WAKE_LED_ON()
#define MY_WAKE_LED_OFF()
#define MY_WAKE_LED_TOGGLE()
#endif

#ifdef STATEFUL_LED
#define MY_STATE_LED_ON() digitalWrite(LED_PIN, HIGH)
#define MY_STATE_LED_OFF() digitalWrite(LED_PIN, LOW)
#define MY_STATE_LED_TOGGLE() digitalWrite(LED_PIN, !digitalRead(LED_PIN))
#else
#define MY_STATE_LED_ON()
#define MY_STATE_LED_OFF()
#define MY_STATE_LED_TOGGLE()
#endif

#define LOCATION_SLUG_MAX_LENGTH 64

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
    STATE_READY_TO_SLEEP,
    STATE_IDENTIFY
} device_states_t;

typedef struct {
    device_states_t current_state;
    uint32_t identify_duration; // in millis
    uint32_t identify_entered_time;
    uint32_t time_to_sleep; /* Time ESP32 will sleep for between readings (in seconds) */
    uint32_t device_uptime; /* Ongoing uptime calculation between deep-sleeps */
    bool debug_log;
} device_state_t;

extern Preferences preferences;
extern char location_slug[LOCATION_SLUG_MAX_LENGTH];
extern device_state_t device_state;

void upon_wake();
void setup_watchdog();
void pat_watchdog();
void enter_deep_sleep();
void error_handler(String reason);

#endif