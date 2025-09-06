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
#define MQTT_TOPIC_MANAGEMENT_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, MANAGEMENT_TOPIC_SUFFIX)

// Define constants for the topics
const char *MQTT_TOPIC_MOISTURE = MQTT_TOPIC_MOISTURE_STR;
const char *MQTT_TOPIC_TEMPERATURE = MQTT_TOPIC_TEMPERATURE_STR;
const char *MQTT_TOPIC_HUMIDITY = MQTT_TOPIC_HUMIDITY_STR;
const char *MQTT_TOPIC_PRESSURE = MQTT_TOPIC_PRESSURE_STR;
const char *MQTT_TOPIC_ALTITUDE = MQTT_TOPIC_ALTITUDE_STR;
const char *MQTT_TOPIC_SUPPLY_VOLTAGE = MQTT_TOPIC_SUPPLY_VOLTAGE_STR;
const char *MQTT_TOPIC_ANALOG_PINS = MQTT_TOPIC_ANALOG_PINS_STR;
const char *MQTT_TOPIC_UPTIME = MQTT_TOPIC_UPTIME_STR;
const char *MQTT_TOPIC_MANAGEMENT = MQTT_TOPIC_MANAGEMENT_STR;

const char *BROKER_IP;
int BROKER_PORT;
const char *DEVICE_NAME;
const char *RX_TOPIC;

void setup_mqtt(const char *MQTT_BROKER_IP, const int MQTT_BROKER_PORT, const char *DEV_NAME, const char *MANAGEMENT_TOPIC) {

  assert(strlen(MQTT_TOPIC_MOISTURE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_TEMPERATURE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_HUMIDITY) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_PRESSURE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_ALTITUDE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_SUPPLY_VOLTAGE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_ANALOG_PINS) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_UPTIME) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_MANAGEMENT) < MQTT_TOPIC_LENGTH_MAX);

  BROKER_IP = MQTT_BROKER_IP;
  BROKER_PORT = MQTT_BROKER_PORT;
  DEVICE_NAME = DEV_NAME;
  RX_TOPIC = MANAGEMENT_TOPIC;

  client.setServer(BROKER_IP, BROKER_PORT);

  client.setCallback(message_rx_callback);

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
      client.subscribe(RX_TOPIC);
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
      client.subscribe(RX_TOPIC);
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


void message_rx_callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;

  MY_DEBUG_PRINT("Message arrived on topic: ");
  MY_DEBUG_PRINT(topic);
  MY_DEBUG_PRINT(". Message: ");

  
  for (int i = 0; i < length; i++) {
    MY_DEBUG_PRINT((char)message[i]);
    
    messageTemp += (char)message[i];
  }
  MY_DEBUG_PRINTLN("");
  
  

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  // if (String(topic) == RX_TOPIC) {
  //   MY_DEBUG_PRINT("Changing output to ");
  //   if(messageTemp == "on"){
  //     MY_DEBUG_PRINTLN("on");
  //   }
  //   else if(messageTemp == "off"){
  //     MY_DEBUG_PRINTLN("off");
  //   }
  // }

  return;
}


void mqtt_disconnect() {
  client.disconnect();
  
  return;
}