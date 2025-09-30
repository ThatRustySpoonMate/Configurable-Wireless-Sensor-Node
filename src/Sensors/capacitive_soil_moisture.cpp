#include "Sensors/capacitive_soil_moisture.hpp"

#ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR

#include "main.hpp"

uint8_t init_soil_sensor() {
    MY_DEBUG_PRINTLN("Detected Soil Sensor");

    // Initialize soil moisture sensors
    pinMode(CAPACITIVE_SOIL_MOISTURE_SENS_VCC_PIN, OUTPUT);  // Fixed: should be OUTPUT, not PULLDOWN
    digitalWrite(CAPACITIVE_SOIL_MOISTURE_SENS_VCC_PIN, LOW); // Start powered off
    pinMode(CAPACITIVE_SOIL_MOISTURE_SENS_DIN_PIN, INPUT);

    MY_DEBUG_PRINTLN("Soil sensor successfully initialized");
    return 1; // Success
}

void read_capacitive_soil_moisture_sensor(transmit_data_t *moisture) {
    // Power up Soil sensor
    digitalWrite(CAPACITIVE_SOIL_MOISTURE_SENS_VCC_PIN, HIGH);
    delay(CAPACITIVE_SOIL_MOISTURE_SETTLE_TIME_MS);

    // Take reading - TODO: Use averaged function with CAPACITIVE_SOIL_MOISTURE_SAMPLES_TO_AVERAGE?
    moisture->data_u16[SOIL_MOISTURE_ID] = analogRead(CAPACITIVE_SOIL_MOISTURE_SENS_DIN_PIN);

    // Power down soil sensor
    digitalWrite(CAPACITIVE_SOIL_MOISTURE_SENS_VCC_PIN, LOW);
}

#endif // DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR