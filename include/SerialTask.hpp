#ifndef INPUT_TASK_HPP
#define INPUT_TASK_HPP

#include <HardwareSerial.h>

void serial_setup();

void serial_handleInput();

void serial_transmit(const char* topic, const char* value);

void serial_transmit_start();

void serial_transmit_end();

#endif