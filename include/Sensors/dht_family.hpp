#ifndef DHT_FAMILY_H
#define DHT_FAMILY_H

#include "configuration.h"
#include "SensorTasks.hpp"

#if defined(DEVICE_DHT11) || defined(DEVICE_DHT21) || defined(DEVICE_DHT22)

// Function declarations
uint32_t init_dht_unified();
void read_dht_unified(transmit_data_t *temp, transmit_data_t *humidity);

#endif // DHT sensors

#endif // DHT_FAMILY_H