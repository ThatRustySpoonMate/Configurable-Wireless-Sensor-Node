#ifndef SHT4X_H
#define SHT4X_H

#include "configuration.h"
#include "SensorTasks.hpp"

#ifdef DEVICE_SHT4X

uint8_t init_sht4x();

void read_sht4x(transmit_data_t *temp, transmit_data_t *humidity);

#endif

#endif
