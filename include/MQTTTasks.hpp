#ifndef MQTT_TASKS_HPP
#define MQTT_TASKS_HPP

#include "WifiTasks.hpp"
#include <PubSubClient.h>
#include "main.hpp"

#define MQTT_TOPIC_LENGTH_MAX 100

// Declarations
extern const char *MQTT_TOPIC_MOISTURE;
extern const char *MQTT_TOPIC_TEMPERATURE;
extern const char *MQTT_TOPIC_HUMIDITY;
extern const char *MQTT_TOPIC_PRESSURE;
extern const char *MQTT_TOPIC_ALTITUDE;
extern const char *MQTT_TOPIC_MANAGEMENT;

// Example:
// const char *MQTT_TOPIC_MOISTURE = "home/outside/garden/moisture";
// const char *MQTT_TOPIC_TEMPERATURE = "home/outside/garden/temperature";
// const char *MQTT_TOPIC_HUMIDITY = "home/outside/garden/humidity";
// const char *MQTT_TOPIC_PRESSURE = "home/outside/garden/pressure";
// const char *MQTT_TOPIC_ALTITUDE = "home/outside/garden/altitude";
// const char *MQTT_TOPIC_MANAGEMENT = "manage/garden";

void setup_mqtt(const char *MQTT_BROKER_IP, const int MQTT_BROKER_PORT, const char *DEV_NAME, const char *MANAGE_PORT);

void mqtt_transmit(const char *topic, const char *payload);

void mqtt_keep_alive();

void mqtt_reconnect();

void message_rx_callback(char* topic, byte* message, unsigned int length);

void mqtt_disconnect();

#endif