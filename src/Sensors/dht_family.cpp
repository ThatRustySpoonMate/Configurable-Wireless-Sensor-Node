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

uint32_t init_dht_unified() {
    uint32_t status = 2; // Uninitialized

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

    #ifdef DEVICE_DHT11
    dht11.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        MY_DEBUG_PRINTLN(F("Error reading temperature from DHT11!"));
    } else {
        temp->data_f32[DHT11_TEMPERATURE_ID] = event.temperature;
    }

    dht11.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        MY_DEBUG_PRINTLN(F("Error reading humidity from DHT11!"));
    } else {
        humidity->data_f32[DHT11_HUMIDITY_ID] = event.relative_humidity;
    }
    #endif

    #ifdef DEVICE_DHT21
    dht21.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        MY_DEBUG_PRINTLN(F("Error reading temperature from DHT21!"));
    } else {
        temp->data_f32[DHT21_TEMPERATURE_ID] = event.temperature;
    }

    dht21.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        MY_DEBUG_PRINTLN(F("Error reading humidity from DHT21!"));
    } else {
        humidity->data_f32[DHT21_HUMIDITY_ID] = event.relative_humidity;
    }
    #endif

    #ifdef DEVICE_DHT22
    dht22.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        MY_DEBUG_PRINTLN(F("Error reading temperature from DHT22!"));
    } else {
        temp->data_f32[DHT22_TEMPERATURE_ID] = event.temperature;
    }

    dht22.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        MY_DEBUG_PRINTLN(F("Error reading humidity from DHT22!"));
    } else {
        humidity->data_f32[DHT22_HUMIDITY_ID] = event.relative_humidity;
    }
    #endif
}

#endif // DHT sensors