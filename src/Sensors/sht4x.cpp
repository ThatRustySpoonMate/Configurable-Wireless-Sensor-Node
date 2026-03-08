#include "Sensors/sht4x.hpp"

#ifdef DEVICE_SHT4X

#include "Adafruit_SHT4x.h"
#include "main.hpp"

// Global sensor instance
static Adafruit_SHT4x sht4 = Adafruit_SHT4x();

// TODO: Make heater run every once in a while to remove built-up condensation

uint8_t init_sht4x() {
    MY_DEBUG_PRINTLN("Detected SHT4X");
    
    uint8_t status = sht4.begin();  
    if (!status) {
        MY_DEBUG_PRINTLN("Could not find a valid SHT4X sensor, check wiring and address");
        return 0; // Fail
    }

    sht4.setPrecision(SHT4X_HIGH_PRECISION);

    MY_DEBUG_PRINTLN("SHT4X successfully Initialized");
    return 1; // Success
}

void read_sht4x(transmit_data_t *temp, transmit_data_t *humidity) {
    MY_DEBUG_PRINTLN("Starting read from SHT4X");
    sensors_event_t humidityEvent, tempEvent;

    for (uint8_t i = 0; i < SHT4X_OVERSAMPLING_RATIO; i ++) {

        sht4.getEvent(&humidityEvent, &tempEvent);
        
        if(SHT4X_TEMPERATURE_ID != -1) {
            temp->data_f32[SHT4X_TEMPERATURE_ID] += tempEvent.temperature;
        }

        if(SHT4X_HUMIDITY_ID != -1) {
            humidity->data_f32[SHT4X_HUMIDITY_ID] += humidityEvent.relative_humidity;
        }

        if (i < SHT4X_OVERSAMPLING_RATIO - 1) {  // Don't delay after last reading
            delay(10);
        }

    }

    // Scale back down oversampling
    if(SHT4X_TEMPERATURE_ID != -1) {
        temp->data_f32[SHT4X_TEMPERATURE_ID] /= SHT4X_OVERSAMPLING_RATIO;
    }

    if(SHT4X_HUMIDITY_ID != -1) {
        humidity->data_f32[SHT4X_HUMIDITY_ID] /= SHT4X_OVERSAMPLING_RATIO;
    }

    // Put sensor into lowest power state after reading - no heater, shortest pulse
    // SHT4X draws ~800uA when idle; this minimises current until deep sleep
    sht4.setPrecision(SHT4X_LOW_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);
}



#endif