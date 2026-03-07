#ifndef WIFI_TASKS_HPP
#define WIFI_TASKS_HPP

#include "main.hpp"
#include <WiFi.h>


void setup_wifi(const char *ssid, const char *password);

bool setup_wifi_with_timeout(const char *ssid, const char *password, uint32_t timeout_ms);

void wifi_disconnect();

int8_t wifi_get_rssi();

bool setup_wifi_fast(const char *ssid, const char *password, uint8_t channel, uint8_t *bssid, uint32_t timeout_ms);

void wifi_cache_connection_info(uint8_t *bssid_out, uint8_t *channel_out);

#endif