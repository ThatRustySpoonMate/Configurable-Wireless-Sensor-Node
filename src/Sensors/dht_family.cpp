#include "Sensors/dht_family.hpp"

#if defined(DEVICE_DHT11) || defined(DEVICE_DHT21) || defined(DEVICE_DHT22)

#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include "main.hpp"

// Global sensor instances
#ifdef DEVICE_DHT11
static DHT_Unified dht11(DHT11_OUT_PIN, DHT11);
#endif

#ifdef DEVICE_DHT21
static DHT_Unified dht21(DHT21_OUT_PIN, DHT21);
#endif

#ifdef DEVICE_DHT22
static DHT_Unified dht22(DHT22_OUT_PIN, DHT22);
#endif

uint8_t init_dht_unified() {
    uint8_t status = 2; // Uninitialized

    #ifdef DEVICE_DHT11
    MY_DEBUG_PRINTLN("Detected DHT11");
    dht11.begin();
    MY_DEBUG_PRINTLN("DHT11 successfully Initialized");
    status = 1;
    #endif

    #ifdef DEVICE_DHT21
    MY_DEBUG_PRINTLN("Detected DHT21");
    dht21.begin();
    MY_DEBUG_PRINTLN("DHT21 successfully Initialized");
    status = 1;
    #endif

    #ifdef DEVICE_DHT22
    MY_DEBUG_PRINTLN("Detected DHT22");
    dht22.begin();
    MY_DEBUG_PRINTLN("DHT22 successfully Initialized");
    status = 1;
    #endif

    return status;
}

void read_dht_unified(transmit_data_t *temp, transmit_data_t *humidity) {
    sensors_event_t event;
    uint8_t tempSuccess = 0;
    uint8_t humiditySuccess = 0;

    #ifdef DEVICE_DHT11
    tempSuccess = 0;
    humiditySuccess = 0;
    for (uint8_t i = 0; i < DHT11_OVERSAMPLING_RATIO; i ++) {
        dht11.temperature().getEvent(&event);
        if (isnan(event.temperature)) {
            MY_DEBUG_PRINTLN(F("Error reading temperature from DHT11!"));
        } else {
            if(DHT11_TEMPERATURE_ID != -1) {
                temp->data_f32[DHT11_TEMPERATURE_ID] = event.temperature;
            }
            tempSuccess += 1;
        }

        dht11.humidity().getEvent(&event);
        if (isnan(event.relative_humidity)) {
            MY_DEBUG_PRINTLN(F("Error reading humidity from DHT11!"));
        } else {
            if(DHT11_HUMIDITY_ID != -1) {
                humidity->data_f32[DHT11_HUMIDITY_ID] = event.relative_humidity;
            }
            humiditySuccess += 1;
        }
    
    }

    // Scale back down oversampling
    if(DHT11_TEMPERATURE_ID != -1) {
        temp->data_f32[DHT11_TEMPERATURE_ID] /= DHT11_OVERSAMPLING_RATIO;
    }

    if(DHT11_HUMIDITY_ID != -1) {
        humidity->data_f32[DHT11_HUMIDITY_ID] /= DHT11_OVERSAMPLING_RATIO;
    }
    #endif

    #ifdef DEVICE_DHT21
    tempSuccess = 0;
    humiditySuccess = 0;
    for (uint8_t i = 0; i < DHT21_OVERSAMPLING_RATIO; i ++) {
        dht21.temperature().getEvent(&event);
        if (isnan(event.temperature)) {
            MY_DEBUG_PRINTLN(F("Error reading temperature from DHT21!"));
        } else {
            if(DHT21_TEMPERATURE_ID != -1) {
                temp->data_f32[DHT21_TEMPERATURE_ID] = event.temperature;
            }
            tempSuccess += 1;
        }

        dht21.humidity().getEvent(&event);
        if (isnan(event.relative_humidity)) {
            MY_DEBUG_PRINTLN(F("Error reading humidity from DHT21!"));
        } else {
            if(DHT21_HUMIDITY_ID != -1) {
                humidity->data_f32[DHT21_HUMIDITY_ID] = event.relative_humidity;
            }
            humiditySuccess += 1;
        }
    }

    // Scale back down oversampling
    if(DHT21_TEMPERATURE_ID != -1) {
        temp->data_f32[DHT21_TEMPERATURE_ID] /= DHT21_OVERSAMPLING_RATIO;
    }

    if(DHT21_HUMIDITY_ID != -1) {
        humidity->data_f32[DHT21_HUMIDITY_ID] /= DHT21_OVERSAMPLING_RATIO;
    }
    #endif

    #ifdef DEVICE_DHT22
    tempSuccess = 0;
    humiditySuccess = 0;
    for (uint8_t i = 0; i < DHT21_OVERSAMPLING_RATIO; i ++) {
        dht22.temperature().getEvent(&event);
        if (isnan(event.temperature)) {
            MY_DEBUG_PRINTLN(F("Error reading temperature from DHT22!"));
        } else {
            if(DHT22_TEMPERATURE_ID != -1) {
                temp->data_f32[DHT22_TEMPERATURE_ID] = event.temperature;
            }
            tempSuccess += 1;
        }

        dht22.humidity().getEvent(&event);
        if (isnan(event.relative_humidity)) {
            MY_DEBUG_PRINTLN(F("Error reading humidity from DHT22!"));
        } else {
            if(DHT22_HUMIDITY_ID != -1) {
                humidity->data_f32[DHT22_HUMIDITY_ID] = event.relative_humidity;
            }
            humiditySuccess += 1;
        }
    }

    // Scale back down oversampling
    if(DHT22_TEMPERATURE_ID != -1) {
        temp->data_f32[DHT22_TEMPERATURE_ID] /= DHT22_OVERSAMPLING_RATIO;
    }

    if(DHT22_HUMIDITY_ID != -1) {
        humidity->data_f32[DHT22_HUMIDITY_ID] /= DHT22_OVERSAMPLING_RATIO;
    }
    #endif
}

#endif // DHT sensors