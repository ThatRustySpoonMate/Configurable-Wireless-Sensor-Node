#ifndef SUPPLY_MONITOR_H
#define SUPPLY_MONITOR_H

#include "configuration.h"
#include "SensorTasks.hpp"

#ifdef INTERNAL_SUPPLY_MONITORING

// Function declarations
uint8_t init_supply_monitoring();
void read_supply_voltage(transmit_data_t *voltage);
float get_battery_voltage_calibrated();

#endif // INTERNAL_SUPPLY_MONITORING

#endif // SUPPLY_MONITOR_H