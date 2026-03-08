#include "Sensors/scd4x.hpp"

#ifdef DEVICE_SCD4X

#include <Wire.h>
#include <SensirionI2cScd4x.h>
#include "main.hpp"

// Global sensor instance
static SensirionI2cScd4x scd4x;
uint16_t previous_uptime_mod = 0;


uint8_t init_scd4x() {
    MY_DEBUG_PRINTLN("Detected SCD4X");

    Wire.begin();
    scd4x.begin(Wire, SCD4X_I2C_ADDRESS);
    delay(50);

    int16_t error = 0;

    // Only write altitude to EEPROM periodically to preserve write endurance
    if(device_state.device_uptime % 1000 > previous_uptime_mod) {
        error = scd4x.setSensorAltitude(SENSOR_ALTITUDE);
        if (error) {
            MY_DEBUG_PRINTLN("SCD4X setSensorAltitude failed - continuing anyway");
        }
        previous_uptime_mod = device_state.device_uptime % 1000;
    }

    // Kick off measurement - sensor may not ACK this immediately after power-on
    // but read_scd4x() will poll getDataReadyStatus() and wait regardless
    scd4x.measureSingleShot(); // Return value intentionally ignored

    MY_DEBUG_PRINTLN("SCD4X successfully Initialized");
    return 1; // Presence is confirmed when read_scd4x() gets valid data
}

void read_scd4x(transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *CO2, transmit_data_t *baroPres, uint8_t initSkipped) {
    MY_DEBUG_PRINTLN("Starting read from SCD4X");
    
    int16_t error;

    // Apply ambient pressure compensation if a barometric sensor is present
    #ifdef PRESSURE_SENSOR_CONNECTED
    if (baroPres != 0) {
        // Sensirion library takes pressure in Pa as uint32_t
        error = scd4x.setAmbientPressure((uint32_t)baroPres->data_f32[0]);
        if (error) {
            MY_DEBUG_PRINTLN("SCD4X setAmbientPressure failed");
        }
    }
    #endif

    // If sensors were not re-initialised this wake (continuous polling mode),
    // we need to trigger a fresh measurement
    if (initSkipped) {
        error = scd4x.measureSingleShot();
        if (error) {
            MY_DEBUG_PRINTLN("SCD4X measureSingleShot failed");
        }
    }

    for (uint8_t i = 0; i < SCD4X_OVERSAMPLING_RATIO; i++) {

        // Poll until data is ready rather than assuming timing
        bool dataReady = false;
        uint8_t readyRetries = 0;
        const uint8_t maxReadyRetries = 20;
        while (!dataReady && readyRetries < maxReadyRetries) {
            error = scd4x.getDataReadyStatus(dataReady);
            if (error) {
                MY_DEBUG_PRINTLN("SCD4X getDataReadyStatus failed");
                break;
            }
            if (!dataReady) {
                MY_DEBUG_PRINTLN("Waiting for SCD4X");
                delay(500);
                readyRetries++;
            }
        }

        if (!dataReady) {
            MY_DEBUG_PRINTLN("SCD4X data never became ready - skipping reading");
            break;
        }

        uint16_t co2 = 0;
        float temperature = 0.0f;
        float relativeHumidity = 0.0f;

        error = scd4x.readMeasurement(co2, temperature, relativeHumidity);
        if (error) {
            MY_DEBUG_PRINTLN("SCD4X readMeasurement failed");
            break;
        }

        if (SCD4X_TEMPERATURE_ID != -1) {
            temp->data_f32[SCD4X_TEMPERATURE_ID] += temperature;
        }

        if (SCD4X_HUMIDITY_ID != -1) {
            humidity->data_f32[SCD4X_HUMIDITY_ID] += relativeHumidity;
        }

        if (SCD4X_CO2_ID != -1) {
            CO2->data_u16[SCD4X_CO2_ID] += co2;
        }

        // Trigger next measurement for all iterations except the last
        if (i < (SCD4X_OVERSAMPLING_RATIO - 1)) {
            error = scd4x.measureSingleShot();
            if (error) {
                MY_DEBUG_PRINTLN("SCD4X measureSingleShot failed");
                break;
            }
        }
    }

    // Scale back down oversampling
    if (SCD4X_TEMPERATURE_ID != -1) {
        temp->data_f32[SCD4X_TEMPERATURE_ID] /= SCD4X_OVERSAMPLING_RATIO;
    }

    if (SCD4X_HUMIDITY_ID != -1) {
        humidity->data_f32[SCD4X_HUMIDITY_ID] /= SCD4X_OVERSAMPLING_RATIO;
    }

    if (SCD4X_CO2_ID != -1) {
        CO2->data_u16[SCD4X_CO2_ID] /= SCD4X_OVERSAMPLING_RATIO;
    }

    // Power down sensor - draws ~0.5mA vs ~15mA active
    // wakeUp() + stopPeriodicMeasurement() are called on next init_scd4x()
    error = scd4x.powerDown();
    if (error) {
        MY_DEBUG_PRINTLN("SCD4X powerDown failed");
    } else {
        MY_DEBUG_PRINTLN("SCD4X powered down");
    }
}

#endif // DEVICE_SCD4X