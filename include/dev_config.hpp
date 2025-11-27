#ifndef DEV_CONFIG_H
#define DEV_CONFIG_H

#include "main.hpp"

void load_config(String *DEVICE_NAME, uint32_t *time_to_sleep, String *WIFI_SSID, String *WIFI_PASSWORD, String *MQTT_BROKER_IP, uint16_t *MQTT_BROKER_PORT);

#endif