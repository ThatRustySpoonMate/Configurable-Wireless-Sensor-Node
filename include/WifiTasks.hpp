#ifndef WIFI_TASKS_HPP
#define WIFI_TASKS_HPP

#include "main.hpp"
#include <WiFi.h>


void setup_wifi(const char *ssid, const char *password);

bool setup_wifi_with_timeout(const char *ssid, const char *password, uint32_t timeout_ms);

void wifi_disconnect();

int8_t wifi_get_rssi();

#endif