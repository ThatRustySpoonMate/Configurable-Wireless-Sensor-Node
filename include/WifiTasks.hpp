#ifndef WIFI_TASKS_HPP
#define WIFI_TASKS_HPP

#include "main.hpp"
#include <WiFi.h>


void setup_wifi(const char *ssid, const char *password);

void wifi_disconnect();

#endif