#include "WifiTasks.hpp"
#include <PubSubClient.h>

void setup_mqtt(const char *MQTT_BROKER_IP, const int MQTT_BROKER_PORT, const char *DEV_NAME);

void mqtt_transmit(const char *topic, const char *payload);

void mqtt_keep_alive();

void mqtt_reconnect();

void message_rx_callback(char* topic, byte* message, unsigned int length);

