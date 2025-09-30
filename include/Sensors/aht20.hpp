#ifndef AHT20_H
#define AHT20_H

#include "configuration.h"
#include "SensorTasks.hpp"

#ifdef DEVICE_AHT20

// Function declarations
uint8_t init_aht20();
void read_aht20(transmit_data_t *temp, transmit_data_t *humidity);

#endif // DEVICE_AHT20

#endif // AHT20_H