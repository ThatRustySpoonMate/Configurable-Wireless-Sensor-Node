#ifndef MAIN_HPP
#define MAIN_HPP

#include <Arduino.h>

extern bool debug_log;

#define DEBUG_PRINT(_STRING_) if(debug_log) { Serial.print(_STRING_); }
#define DEBUG_PRINTLN(_STRING_) if(debug_log) { Serial.println(_STRING_); }

void upon_wake();

#endif