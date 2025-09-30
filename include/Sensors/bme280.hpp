#ifndef BME280_H
#define BME280_H

#include "configuration.h"
#include "SensorTasks.hpp"

#ifdef DEVICE_BME280

// Function declarations
uint8_t init_bme280();
void read_bme280(transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude);

#endif // DEVICE_BME280

#endif // BME280_H