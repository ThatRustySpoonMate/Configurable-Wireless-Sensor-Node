#ifndef MQTT_TASKS_HPP
#define MQTT_TASKS_HPP

#include "WifiTasks.hpp"
#include <PubSubClient.h>
#include "main.hpp"

#define MQTT_TOPIC_LENGTH_MAX 100

// Declarations
extern const char *MQTT_TOPIC_MOISTURE;         // Example: "home/outside/garden/moisture";
extern const char *MQTT_TOPIC_TEMPERATURE;      // Example: "home/outside/garden/temperature";
extern const char *MQTT_TOPIC_HUMIDITY;         // Example: "home/outside/garden/humidity"; 
extern const char *MQTT_TOPIC_AQI;              // Example: "home/outside/garden/aqi"; 
extern const char *MQTT_TOPIC_TVOC;             // Example: "home/outside/garden/tvoc"; 
extern const char *MQTT_TOPIC_ECO2;             // Example: "home/outside/garden/eCO2"; 
extern const char *MQTT_TOPIC_PRESSURE;         // Example: "home/outside/garden/pressure"; 
extern const char *MQTT_TOPIC_ALTITUDE;         // Example: "home/outside/garden/altitude";
extern const char *MQTT_TOPIC_SUPPLY_VOLTAGE;   // Example: "home/outside/garden/supply-voltage";
extern const char *MQTT_TOPIC_ANALOG_PINS;      // Example: "home/outside/garden/analog-pins";
extern const char *MQTT_TOPIC_UPTIME;           // Example: "home/outside/garden/uptime";
extern const char *MQTT_TOPIC_WIFI_RSSI;        // Example: "home/outside/garden/wifi-rssi"
extern const char *MQTT_TOPIC_FIRMWARE_VERSION; // Example: "home/outside/garden/firmwareversion"
extern const char *MQTT_TOPIC_ERRORS;           // Example: "home/outside/garden/errors"
extern const char *MQTT_TOPIC_MANAGEMENT_INTERVAL;       // Example: "home/outside/garden/manage/interval";
extern const char *MQTT_TOPIC_MANAGEMENT_LOCATION;       // Example: "home/outside/garden/manage/location";
extern const char *MQTT_TOPIC_QUERY_FIRMWARE_VERSION;    // Example: "home/outside/garden/query/firmwareversion";

void setup_mqtt(const char *MQTT_BROKER_IP, const int MQTT_BROKER_PORT, const char *DEV_NAME);

void mqtt_transmit(const char *topic, const char *payload);

void mqtt_keep_alive();

void mqtt_reconnect();

bool mqtt_reconnect_with_timeout(uint32_t timeout_ms);

void management_message_receive(char* topic, byte* message, unsigned int length);

void mqtt_log_error(const char* err_message);

void mqtt_disconnect();

void build_mqtt_topics();

#endif