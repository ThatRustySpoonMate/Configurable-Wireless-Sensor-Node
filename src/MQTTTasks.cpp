#include "MQTTTasks.hpp"

WiFiClient espClient;
PubSubClient client(espClient);

// Define the location slug
#define MQTT_TOPIC_LOCATION_SLUG "home/unconfigured"

// Define topic suffixes
#define MOISTURE_TOPIC "/moisture"
#define TEMPERATURE_TOPIC "/temperature"
#define HUMIDITY_TOPIC "/humidity"
#define PRESSURE_TOPIC "/pressure"
#define ALTITUDE_TOPIC "/altitude"
#define MANAGE_TOPIC "manage/unconfigured"

// Helper macros to concatenate strings
#define CONCATENATE(a, b) a b

// Create full topic strings using macros
#define MQTT_TOPIC_MOISTURE_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, MOISTURE_TOPIC)
#define MQTT_TOPIC_TEMPERATURE_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, TEMPERATURE_TOPIC)
#define MQTT_TOPIC_HUMIDITY_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, HUMIDITY_TOPIC)
#define MQTT_TOPIC_PRESSURE_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, PRESSURE_TOPIC)
#define MQTT_TOPIC_ALTITUDE_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, ALTITUDE_TOPIC)
#define MQTT_TOPIC_MANAGEMENT_STR CONCATENATE(MQTT_TOPIC_LOCATION_SLUG, MANAGE_TOPIC)

// Define constants for the topics
const char *MQTT_TOPIC_MOISTURE = MQTT_TOPIC_MOISTURE_STR;
const char *MQTT_TOPIC_TEMPERATURE = MQTT_TOPIC_TEMPERATURE_STR;
const char *MQTT_TOPIC_HUMIDITY = MQTT_TOPIC_HUMIDITY_STR;
const char *MQTT_TOPIC_PRESSURE = MQTT_TOPIC_PRESSURE_STR;
const char *MQTT_TOPIC_ALTITUDE = MQTT_TOPIC_ALTITUDE_STR;
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
    DEBUG_PRINT("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(DEVICE_NAME)) {
      DEBUG_PRINTLN("connected");
      // Subscribe
      client.subscribe(RX_TOPIC);
    } else {
      DEBUG_PRINT("failed, rc=");
      DEBUG_PRINT(client.state());
      DEBUG_PRINTLN(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  return;
}

void message_rx_callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;

  DEBUG_PRINT("Message arrived on topic: ");
  DEBUG_PRINT(topic);
  DEBUG_PRINT(". Message: ");

  
  for (int i = 0; i < length; i++) {
    DEBUG_PRINT((char)message[i]);
    
    messageTemp += (char)message[i];
  }
  DEBUG_PRINTLN("");
  
  

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  // if (String(topic) == RX_TOPIC) {
  //   DEBUG_PRINT("Changing output to ");
  //   if(messageTemp == "on"){
  //     DEBUG_PRINTLN("on");
  //   }
  //   else if(messageTemp == "off"){
  //     DEBUG_PRINTLN("off");
  //   }
  // }

  return;
}


void mqtt_disconnect() {
  client.disconnect();
  
  return;
}