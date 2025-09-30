#ifndef SOIL_MOISTURE_H
#define SOIL_MOISTURE_H

#include "configuration.h"
#include "SensorTasks.hpp"

#ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR

// Function declarations
uint8_t init_soil_sensor();
void read_capacitive_soil_moisture_sensor(transmit_data_t *moisture);

#endif // DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR

#endif // SOIL_MOISTURE_H