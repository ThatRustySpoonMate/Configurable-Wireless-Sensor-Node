#ifndef SCD4X_H
#define SCD4X_H

#include "configuration.h"
#include "SensorTasks.hpp"

#ifdef DEVICE_SCD4X

// Function declarations
uint8_t init_scd4x();

void read_scd4x(transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *eco2, transmit_data_t *baroPres);

#endif // DEVICE_SCD4X

#endif // SCD4X_H