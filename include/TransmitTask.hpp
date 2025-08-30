#ifndef TRANSMIT_TASK_HPP
#define TRANSMIT_TASK_HPP

#include "main.hpp"
#include "MQTTTasks.hpp"
#include "SensorTasks.hpp"
#include "configuration.h"

void transmitTask_init();

void transmitTask_run(transmit_data_entry_t transmitData[DATAPOINTS_NUM]);

#endif