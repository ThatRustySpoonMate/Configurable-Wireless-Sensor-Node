#include "MQTTTasks.hpp"
#include "configuration.h"
#include "commands.hpp"

WiFiClient espClient;
PubSubClient client(espClient);

// Dynamic topic storage
char mqtt_topic_moisture[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_temperature[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_humidity[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_aqi[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_tvoc[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_eco2[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_pressure[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_altitude[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_supply_voltage[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_analog_pins[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_uptime[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_wifi_rssi[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_firmware_version[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_errors[MQTT_TOPIC_LENGTH_MAX];
// Management
char mqtt_topic_management_interval[MQTT_TOPIC_LENGTH_MAX];
char mqtt_topic_management_location[MQTT_TOPIC_LENGTH_MAX];
// Queries
char mqtt_topic_query_firmware_version[MQTT_TOPIC_LENGTH_MAX];

// Pointers to the topics (for compatibility with existing code)
const char *MQTT_TOPIC_MOISTURE = mqtt_topic_moisture;
const char *MQTT_TOPIC_TEMPERATURE = mqtt_topic_temperature;
const char *MQTT_TOPIC_HUMIDITY = mqtt_topic_humidity;
const char *MQTT_TOPIC_AQI = mqtt_topic_aqi;
const char *MQTT_TOPIC_TVOC = mqtt_topic_tvoc;
const char *MQTT_TOPIC_ECO2 = mqtt_topic_eco2;
const char *MQTT_TOPIC_PRESSURE = mqtt_topic_pressure;
const char *MQTT_TOPIC_ALTITUDE = mqtt_topic_altitude;
const char *MQTT_TOPIC_SUPPLY_VOLTAGE = mqtt_topic_supply_voltage;
const char *MQTT_TOPIC_ANALOG_PINS = mqtt_topic_analog_pins;
const char *MQTT_TOPIC_UPTIME = mqtt_topic_uptime;
const char *MQTT_TOPIC_WIFI_RSSI = mqtt_topic_wifi_rssi;
const char *MQTT_TOPIC_FIRMWARE_VERSION = mqtt_topic_firmware_version;
const char *MQTT_TOPIC_ERRORS = mqtt_topic_errors;
// Management
const char *MQTT_TOPIC_MANAGEMENT_INTERVAL = mqtt_topic_management_interval;
const char *MQTT_TOPIC_MANAGEMENT_LOCATION = mqtt_topic_management_location;
// Queries
const char *MQTT_TOPIC_QUERY_FIRMWARE_VERSION = mqtt_topic_query_firmware_version;

const char *BROKER_IP;
int BROKER_PORT;
const char *DEVICE_NAME;

void build_mqtt_topics() {
  extern char location_slug[];
  
  snprintf(mqtt_topic_moisture, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, MOISTURE_TOPIC_SUFFIX);
  snprintf(mqtt_topic_temperature, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, TEMPERATURE_TOPIC_SUFFIX);
  snprintf(mqtt_topic_humidity, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, HUMIDITY_TOPIC_SUFFIX);
  snprintf(mqtt_topic_aqi, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, AQI_TOPIC_SUFFIC);
  snprintf(mqtt_topic_tvoc, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, TVOC_TOPIC_SUFFIC);
  snprintf(mqtt_topic_eco2, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, ECO2_TOPIC_SUFFIC);
  snprintf(mqtt_topic_pressure, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, PRESSURE_TOPIC_SUFFIX);
  snprintf(mqtt_topic_altitude, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, ALTITUDE_TOPIC_SUFFIX);
  snprintf(mqtt_topic_supply_voltage, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, SUPPLY_VOLTAGE_TOPIC_SUFFIX);
  snprintf(mqtt_topic_analog_pins, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, ANALOG_PINS_TOPIC_SUFFIX);
  snprintf(mqtt_topic_uptime, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, UPTIME_TOPIC_SUFFIX);
  snprintf(mqtt_topic_wifi_rssi, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, WIFI_RSSI_TOPIC_SUFFIX);
  snprintf(mqtt_topic_firmware_version, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, FIRMWARE_VERSION_TOPIC_SUFFIX);
  snprintf(mqtt_topic_errors, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, ERRORS_TOPIC_SUFFIX);
  snprintf(mqtt_topic_management_interval, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, MANAGEMENT_OUTPUT_INTERVAL_TOPIC_SUFFIX);
  snprintf(mqtt_topic_management_location, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, MANAGEMENT_LOCATION_TOPIC_SUFFIX);
  snprintf(mqtt_topic_query_firmware_version, MQTT_TOPIC_LENGTH_MAX, "%s%s", location_slug, QUERY_FIRMWARE_VERSION_TOPIC_SUFFIX);
}

void setup_mqtt(const char *MQTT_BROKER_IP, const int MQTT_BROKER_PORT, const char *DEV_NAME) {

  #ifdef DATA_OUTPUT_OVER_MQTT

  // Build all MQTT topics from the location slug
  build_mqtt_topics();

  assert(strlen(MQTT_TOPIC_MOISTURE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_TEMPERATURE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_HUMIDITY) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_AQI) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_TVOC) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_ECO2) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_PRESSURE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_ALTITUDE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_SUPPLY_VOLTAGE) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_ANALOG_PINS) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_UPTIME) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_WIFI_RSSI) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_FIRMWARE_VERSION) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_ERRORS) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_MANAGEMENT_INTERVAL) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_MANAGEMENT_LOCATION) < MQTT_TOPIC_LENGTH_MAX);
  assert(strlen(MQTT_TOPIC_QUERY_FIRMWARE_VERSION) < MQTT_TOPIC_LENGTH_MAX);

  BROKER_IP = MQTT_BROKER_IP;
  BROKER_PORT = MQTT_BROKER_PORT;
  DEVICE_NAME = DEV_NAME;

  client.setServer(BROKER_IP, BROKER_PORT);

  client.setCallback(management_message_receive);

  #endif

  return;
}


void mqtt_transmit(const char *topic, const char *payload) {
  #ifdef DATA_OUTPUT_OVER_MQTT

  mqtt_keep_alive();
  client.publish(topic, payload);

  #endif

  return;
}


void mqtt_keep_alive() {
  #ifdef DATA_OUTPUT_OVER_MQTT

  if (!client.connected()) {
    mqtt_reconnect();
  }

  client.loop();

  #endif

  return;
}

void mqtt_log_error(const char* err_message) {
  #ifdef DATA_OUTPUT_OVER_MQTT

  mqtt_transmit(MQTT_TOPIC_ERRORS, err_message);

  #endif

  return;
}


void mqtt_reconnect() {

  #ifdef DATA_OUTPUT_OVER_MQTT

  // Loop until we're reconnected
  while (!client.connected()) {
    MY_DEBUG_PRINT("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(DEVICE_NAME)) {
      MY_DEBUG_PRINTLN("connected");
      // Subscribe to management topics
      client.subscribe(MQTT_TOPIC_MANAGEMENT_INTERVAL);
      client.subscribe(MQTT_TOPIC_MANAGEMENT_LOCATION);
      client.subscribe(MQTT_TOPIC_QUERY_FIRMWARE_VERSION);
    } else {
      MY_DEBUG_PRINT("failed, rc=");
      MY_DEBUG_PRINT(client.state());
      MY_DEBUG_PRINTLN(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  #endif

  return;
}

bool mqtt_reconnect_with_timeout(uint32_t timeout_ms) {

  #ifdef DATA_OUTPUT_OVER_MQTT

  uint32_t start_time = millis();

  if(client.connected()) {
    return true;
  }
  
  while (!client.connected() && (millis() - start_time < timeout_ms)) {
    MY_DEBUG_PRINTLN("Attempting MQTT connection...");
    
    if (client.connect(DEVICE_NAME)) {
      MY_DEBUG_PRINTLN("MQTT connected");
      // Subscribe to management topics
      client.subscribe(MQTT_TOPIC_MANAGEMENT_INTERVAL);
      client.subscribe(MQTT_TOPIC_MANAGEMENT_LOCATION);
      client.subscribe(MQTT_TOPIC_QUERY_FIRMWARE_VERSION);
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

  #endif

  #ifndef DATA_OUTPUT_OVER_MQTT
    MY_DEBUG_PRINTLN("Skipping connecting to MQTT");
    return true; // Skip MQTT setup
  #endif

  return false; // Timeout reached
}


void management_message_receive(char* topic, byte* message, unsigned int length) {

  #ifdef DATA_OUTPUT_OVER_MQTT

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
    commands_set_interval(time_to_sleep);
  }
  
  // Check and set location slug - will take effect on next wake from deep sleep
  if (String(topic) == MQTT_TOPIC_MANAGEMENT_LOCATION) {
    // Validate message length
    if (receivedMessage.length() > 0 && receivedMessage.length() < LOCATION_SLUG_MAX_LENGTH) {
      commands_set_location(receivedMessage);
    } else {
      mqtt_transmit(MQTT_TOPIC_ERRORS, "Invalid location slug length - ignoring");
      MY_DEBUG_PRINTLN("Invalid location slug length - ignoring");
    }
  }

  // Check for firmware version query
  if (String(topic) == MQTT_TOPIC_QUERY_FIRMWARE_VERSION) {
    commands_get_firmware_version();
  }

  #endif

  return;
}


void mqtt_disconnect() {

  #ifdef DATA_OUTPUT_OVER_MQTT

  client.disconnect();

  #endif
  
  return;
}