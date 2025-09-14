#include "MQTTTasks.hpp"
#include "configuration.h"

WiFiClient espClient;
PubSubClient client(espClient);

// Helper macros to concatenate strings
#define CONCATENATE(a, b) a b

// Create full topic strings using macros
#define MQTT_TOPIC_MOISTURE_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, MOISTURE_TOPIC_SUFFIX)
#define MQTT_TOPIC_TEMPERATURE_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, TEMPERATURE_TOPIC_SUFFIX)
#define MQTT_TOPIC_HUMIDITY_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, HUMIDITY_TOPIC_SUFFIX)
#define MQTT_TOPIC_PRESSURE_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, PRESSURE_TOPIC_SUFFIX)
#define MQTT_TOPIC_ALTITUDE_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, ALTITUDE_TOPIC_SUFFIX)
#define MQTT_TOPIC_SUPPLY_VOLTAGE_STR  CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, SUPPLY_VOLTAGE_TOPIC_SUFFIX)
#define MQTT_TOPIC_ANALOG_PINS_STR  CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, ANALOG_PINS_TOPIC_SUFFIX)
#define MQTT_TOPIC_UPTIME_STR  CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, UPTIME_TOPIC_SUFFIX)
#define MQTT_TOPIC_WIFI_RSSI_STR  CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, WIFI_RSSI_TOPIC_SUFFIX)
#define MQTT_TOPIC_MANAGEMENT_INTERVAL_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, MANAGEMENT_OUTPUT_INTERVAL_TOPIC_SUFFIX)

// Define constants for the topics
const char *MQTT_TOPIC_MOISTURE = MQTT_TOPIC_MOISTURE_STR;
const char *MQTT_TOPIC_TEMPERATURE = MQTT_TOPIC_TEMPERATURE_STR;
const char *MQTT_TOPIC_HUMIDITY = MQTT_TOPIC_HUMIDITY_STR;
const char *MQTT_TOPIC_PRESSURE = MQTT_TOPIC_PRESSURE_STR;
const char *MQTT_TOPIC_ALTITUDE = MQTT_TOPIC_ALTITUDE_STR;
const char *MQTT_TOPIC_SUPPLY_VOLTAGE = MQTT_TOPIC_SUPPLY_VOLTAGE_STR;
const char *MQTT_TOPIC_ANALOG_PINS = MQTT_TOPIC_ANALOG_PINS_STR;
const char *MQTT_TOPIC_UPTIME = MQTT_TOPIC_UPTIME_STR;
const char *MQTT_TOPIC_WIFI_RSSI = MQTT_TOPIC_WIFI_RSSI_STR;
const char *MQTT_TOPIC_MANAGEMENT_INTERVAL = MQTT_TOPIC_MANAGEMENT_INTERVAL_STR;

const char *BROKER_IP;
int BROKER_PORT;
const char *DEVICE_NAME;

void setup_mqtt(const char *MQTT_BROKER_IP, const int MQTT_BROKER_PORT, const char *DEV_NAME) {

  assert(strlen(MQTT_TOPIC_MOISTURE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_TEMPERATURE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_HUMIDITY) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_PRESSURE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_ALTITUDE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_SUPPLY_VOLTAGE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_ANALOG_PINS) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_UPTIME) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_WIFI_RSSI) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_MANAGEMENT_INTERVAL) < MQTT_TOPIC_LENGTH_MAX);

  BROKER_IP = MQTT_BROKER_IP;
  BROKER_PORT = MQTT_BROKER_PORT;
  DEVICE_NAME = DEV_NAME;

  client.setServer(BROKER_IP, BROKER_PORT);

  client.setCallback(management_message_receive);

  return;
}


void mqtt_transmit(const char *topic, const char *payload) {
  mqtt_keep_alive();
  client.publish(topic, payload);

  return;
}


void mqtt_keep_alive() {
  if (!client.connected()) {
    mqtt_reconnect();
  }

  client.loop();

  return;
}


void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    MY_DEBUG_PRINT("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(DEVICE_NAME)) {
      MY_DEBUG_PRINTLN("connected");
      // Subscribe
      client.subscribe(MQTT_TOPIC_MANAGEMENT_INTERVAL);
    } else {
      MY_DEBUG_PRINT("failed, rc=");
      MY_DEBUG_PRINT(client.state());
      MY_DEBUG_PRINTLN(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  return;
}

bool mqtt_reconnect_with_timeout(uint32_t timeout_ms) {
  uint32_t start_time = millis();
  
  while (!client.connected() && (millis() - start_time < timeout_ms)) {
    MY_DEBUG_PRINTLN("Attempting MQTT connection...");
    
    if (client.connect(DEVICE_NAME)) {
      MY_DEBUG_PRINTLN("MQTT connected");
      client.subscribe(MQTT_TOPIC_MANAGEMENT_INTERVAL);
      return true;
    }
    
    MY_DEBUG_PRINT("MQTT failed, rc=");
    MY_DEBUG_PRINTLN(client.state());
    delay(1000);
    
    // Feed watchdog during connection attempt
    extern void pat_watchdog();
    pat_watchdog();
  }
  
  MY_DEBUG_PRINTLN("MQTT connection timeout");
  return false; // Timeout reached
}


void management_message_receive(char* topic, byte* message, unsigned int length) {
  String receivedMessage;

  MY_DEBUG_PRINT("Message arrived on topic: ");
  MY_DEBUG_PRINT(topic);
  MY_DEBUG_PRINT(". Message: ");

  // Reassemble message
  for (int i = 0; i < length; i++) {
    MY_DEBUG_PRINT((char)message[i]);
    
    receivedMessage += (char)message[i];
  }
  MY_DEBUG_PRINTLN("");
  
  // Check and set data interval
  if (String(topic) == MQTT_TOPIC_MANAGEMENT_INTERVAL) { 
    time_to_sleep = receivedMessage.toInt(); 
    preferences.putULong(INTERVAL_KEY, time_to_sleep);
  }

  return;
}


void mqtt_disconnect() {
  client.disconnect();
  
  return;
}