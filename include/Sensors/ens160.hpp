#ifndef ENS160_H
#define ENS160_H

#include "configuration.h"
#include "SensorTasks.hpp"

#ifdef DEVICE_ENS160

// Function declarations
uint8_t init_ens160();
void read_ens160(transmit_data_t *aqi, transmit_data_t *tvoc, transmit_data_t *CO2, transmit_data_t *temp, transmit_data_t *humidity);

#endif // DEVICE_ENS160

#endif // ENS160_H