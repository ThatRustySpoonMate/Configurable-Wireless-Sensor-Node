#ifndef TRANSMIT_TASK_HPP
#define TRANSMIT_TASK_HPP

#include "main.hpp"
#include "MQTTTasks.hpp"
#include "SensorTasks.hpp"

#define MQTT_TRANSMIT_TIME_BUFFER 500 // Amount of time after queueing last message to be sent before disconnecting and sleeping

void transmitTask_init();

void transmitTask_run(transmit_data_entry_t transmitData[DATAPOINTS_NUM]);

#endif