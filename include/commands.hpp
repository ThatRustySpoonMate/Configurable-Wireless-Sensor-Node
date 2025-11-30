#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "main.hpp"


uint8_t commands_set_interval(uint32_t interval);

uint8_t commands_set_location(String location);

uint8_t commands_identify_mode(uint32_t duration);

uint8_t commands_debug_mode(uint32_t state);

uint8_t commands_factory_reset();

uint8_t commands_get_firmware_version();

uint8_t commands_get_device_name();


#endif // COMMANDS_HPP