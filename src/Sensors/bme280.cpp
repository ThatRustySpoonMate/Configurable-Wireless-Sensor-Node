#include "Sensors/bme280.hpp"

#ifdef DEVICE_BME280

#include <Adafruit_BME280.h>
#include "main.hpp"

// Global sensor instance
static Adafruit_BME280 bme; // I2C

uint32_t init_bme280() {
    MY_DEBUG_PRINTLN("Detected BME280");
    
    uint32_t status = bme.begin(BME280_ADDR);
    if (!status) {
        MY_DEBUG_PRINTLN("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        MY_DEBUG_PRINT("SensorID was: 0x"); 
        MY_DEBUG_PRINTLN(bme.sensorID());
        return 0; // Fail
    }

    MY_DEBUG_PRINTLN("BME280 successfully Initialized");
    return 1; // Success
}

void read_bme280(transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude) {
    temp->data_f32[BME280_TEMPERATURE_ID] = bme.readTemperature();
    humidity->data_f32[BME280_HUMIDITY_ID] = bme.readHumidity();
    baroPres->data_f32[BME280_PRESSURE_ID] = bme.readPressure();
    altitude->data_f32[BME280_ALTITUDE_ID] = bme.readAltitude(SEALEVELPRESSURE_HPA);
}

#endif // DEVICE_BME280
