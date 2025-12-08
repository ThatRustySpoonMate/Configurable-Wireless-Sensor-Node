#include "Sensors/scd4x.hpp"

#ifdef DEVICE_SCD4X

#include <Wire.h>

#include "SparkFun_SCD4x_Arduino_Library.h"
#include "main.hpp"

// Global sensor instance
SCD4x scd4x; // I2C

uint8_t init_scd4x() {
    MY_DEBUG_PRINTLN("Detected SCD4X");

    Wire.begin();
    uint32_t status = scd4x.begin(false, true, false);
    if (!status) {
        MY_DEBUG_PRINTLN("Could not find a valid SCD4X sensor, check wiring and address");
        return 0; // Fail
    }

    scd4x.setSensorAltitude(SENSOR_ALTITUDE);

    bool success = scd4x.measureSingleShot();
    if (success == false)
    {
        MY_DEBUG_PRINTLN("measureSingleShot failed. Are you sure you have a SCD41 connected?");
    }

    MY_DEBUG_PRINTLN("SCD4X successfully Initialized");
    return 1; // Success
}

void read_scd4x(transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *CO2, transmit_data_t *baroPres, uint8_t initSkipped) {

    // Give it first barometric pressure reading
    #ifdef PRESSURE_SENSOR_CONNECTED
    if(baroPres != 0) {
        scd4x.setAmbientPressure(baroPres->data_f32[0]);
    }
    #endif

    if(initSkipped){
        scd4x.measureSingleShot();
    }

    for (uint8_t i = 0; i < SCD4X_OVERSAMPLING_RATIO; i ++) {

        while(!scd4x.readMeasurement()){
            MY_DEBUG_PRINTLN("Waiting for SCD4X");
            delay(500);
        }

        if(SCD4X_TEMPERATURE_ID != -1) {
            temp->data_f32[SCD4X_TEMPERATURE_ID] += scd4x.getTemperature();
        }

        if(SCD4X_HUMIDITY_ID != -1) {
            humidity->data_f32[SCD4X_HUMIDITY_ID] += scd4x.getHumidity();
        }

        if(SCD4X_CO2_ID != -1) {
            CO2->data_u16[SCD4X_CO2_ID] += scd4x.getCO2();
        }

        scd4x.measureSingleShot();
    }

    // Scale back down oversampling
    if(SCD4X_TEMPERATURE_ID != -1) {
        temp->data_f32[SCD4X_TEMPERATURE_ID] /= SCD4X_OVERSAMPLING_RATIO;
    }

    if(SCD4X_HUMIDITY_ID != -1) {
        humidity->data_f32[SCD4X_HUMIDITY_ID] /= SCD4X_OVERSAMPLING_RATIO;
    }

    if(SCD4X_CO2_ID != -1) {
        CO2->data_f32[SCD4X_CO2_ID] /= SCD4X_OVERSAMPLING_RATIO;
    }

    

}

#endif // DEVICE_BME280
