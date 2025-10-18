#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "main.hpp"



uint8_t commands_set_interval(uint32_t interval);

uint8_t commands_set_location(String location);

uint8_t commands_get_firmware_version();

#endif // COMMANDS_HPP