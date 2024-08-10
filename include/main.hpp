#include <Arduino.h>

#include "credentials.h"
#include "MQTTTasks.hpp"
#include "SensorTasks.hpp"

#define debug

// Example:
// const char *MQTT_TOPIC_MOISTURE = "home/outside/garden/moisture";
// const char *MQTT_TOPIC_TEMPERATURE = "home/outside/garden/temperature";
// const char *MQTT_TOPIC_HUMIDITY = "home/outside/garden/humidity";
// const char *MQTT_TOPIC_PRESSURE = "home/outside/garden/pressure";
// const char *MQTT_TOPIC_ALTITUDE = "home/outside/garden/altitude";
// const char *MQTT_MANAGEMENT_TOPIC = "manage/garden";

const char *MQTT_TOPIC_MOISTURE = "home/unconfigured";
const char *MQTT_TOPIC_TEMPERATURE = "home/unconfigured";
const char *MQTT_TOPIC_HUMIDITY = "home/unconfigured";
const char *MQTT_TOPIC_PRESSURE = "home/unconfigured";
const char *MQTT_TOPIC_ALTITUDE = "home/unconfigured";
const char *MQTT_MANAGEMENT_TOPIC = "manage/unconfigured";

void upon_wake();