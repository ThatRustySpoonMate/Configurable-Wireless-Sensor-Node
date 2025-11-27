#include "dev_config.hpp"
#include "main.hpp"
#include "configuration.h"


String readLineFromSerial() {
  while (Serial.available()) {
    Serial.read(); // Clear any leftover data
  }

  String input = "";
  while (true) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n' || c == '\r') {
        break;
      }

      input += c;
    }
    delay(100);
    digitalWrite(WAKE_LED_PIN, !digitalRead(WAKE_LED_PIN));
  }
  return input;
}

void first_time_setup() {
  digitalWrite(WAKE_LED_PIN, HIGH);
  delay(5000); // Give time for serial monitor to connect


  Serial.println("\n=== First Time Setup ===");
  Serial.println("Please enter the following configuration:");
  
  String input;
  String deviceName, wifiSSID, wifiPassword, mqttBrokerIP, deviceLocation;
  uint32_t wakeInterval;
  uint16_t mqttBrokerPort;

  
  // Read Device Name
  Serial.print("\nDevice Name: ");
  input = readLineFromSerial();
  deviceName = input;
  preferences.putString(DEVICE_NAME_KEY, input);
  Serial.println("Stored: " + input);
  
  // Read WiFi SSID
  Serial.print("\nWiFi SSID: ");
  input = readLineFromSerial();
  wifiSSID = input;
  preferences.putString(WIFI_SSID_KEY, input);
  Serial.println("Stored: " + input);
  
  // Read WiFi Password
  Serial.print("\nWiFi Password: ");
  input = readLineFromSerial();
  wifiPassword = input;
  preferences.putString(WIFI_PWD_KEY, input);
  Serial.println("Stored: " + input);
  
  // Read MQTT Broker IP Address
  Serial.print("\nMQTT Broker IP Address: ");
  input = readLineFromSerial();
  mqttBrokerIP = input;
  preferences.putString(MQTT_BROKER_IP_KEY, input);
  Serial.println("Stored: " + input);
  
  // Read MQTT Broker Port
  Serial.print("\nMQTT Broker Port: ");
  input = readLineFromSerial();
  mqttBrokerPort = input.toInt();
  preferences.putUShort(MQTT_BROKER_PORT_KEY, mqttBrokerPort);
  Serial.println("Stored: " + String(mqttBrokerPort));
  
  // Read Wake Interval
  Serial.print("\nWake Interval (seconds): ");
  input = readLineFromSerial();
  wakeInterval = input.toInt();
  preferences.putULong(INTERVAL_KEY, wakeInterval);
  Serial.println("Stored: " + String(wakeInterval));
  
  // Read Device Location
  Serial.print("\nDevice Location: ");
  input = readLineFromSerial();
  deviceLocation = input;
  preferences.putString(LOCATION_SLUG_KEY, input);
  Serial.println("Stored: " + input);
  
  // Display all configuration and ask for confirmation
  Serial.println("\n=== Configuration Summary ===");
  Serial.println("Device Name: " + deviceName);
  Serial.println("WiFi SSID: " + wifiSSID);
  Serial.println("WiFi Password: " + wifiPassword);
  Serial.println("MQTT Broker IP: " + mqttBrokerIP);
  Serial.println("MQTT Broker Port: " + String(mqttBrokerPort));
  Serial.println("Wake Interval: " + String(wakeInterval) + " seconds");
  Serial.println("Device Location: " + deviceLocation);
  Serial.println("\nIs this configuration correct? (y/n): ");
  
  input = readLineFromSerial();
  input.toLowerCase();
  
  if (input == "y" || input == "yes") {
    // Flag first-time-setup complete - we can skip this function on future startup
    preferences.putBool(FIRST_SETUP_KEY, true);
    digitalWrite(WAKE_LED_PIN, LOW);
    Serial.println("\n=== Setup Complete! ===");
  } else {
    // Clear the preferences and restart
    Serial.println("\n=== Restarting setup... ===");
    digitalWrite(WAKE_LED_PIN, LOW);
    delay(1000);
    ESP.restart();
  }

  return;
}


void load_config(String *DEVICE_NAME, uint32_t *time_to_sleep, String *WIFI_SSID, String *WIFI_PASSWORD, String *MQTT_BROKER_IP, uint16_t * MQTT_BROKER_PORT) {

extern char location_slug[];

  bool firstTimeSetupComplete = preferences.getBool(FIRST_SETUP_KEY, false);

  if(firstTimeSetupComplete == false) {
    first_time_setup();
  } else {
    MY_DEBUG_PRINT("Skipping first time setup.");
  }

  // Load fts
  MY_DEBUG_PRINT("Loaded first time setup from flash: ");
  MY_DEBUG_PRINTLN(preferences.getBool(FIRST_SETUP_KEY, false));

  // Load Device Name
  *DEVICE_NAME = preferences.getString(DEVICE_NAME_KEY);
  MY_DEBUG_PRINT("Loaded device name from flash: ");
  MY_DEBUG_PRINTLN(*DEVICE_NAME);

  // Load interval
  uint32_t interval_temp = preferences.getULong(INTERVAL_KEY, DEFAULT_SLEEP_TIME_SECONDS_BACKUP);
  MY_DEBUG_PRINT("Loaded interval from flash: ");
  MY_DEBUG_PRINTLN(interval_temp);
  *time_to_sleep = interval_temp;
  
  // Load location slug from preferences or use default
  String stored_slug = preferences.getString(LOCATION_SLUG_KEY, MQTT_TOPIC_LOCATION_SLUG_BACKUP);
  
  // Location slug has been stored in flash
  strncpy(location_slug, stored_slug.c_str(), LOCATION_SLUG_MAX_LENGTH - 1);
  location_slug[LOCATION_SLUG_MAX_LENGTH - 1] = '\0'; // Ensure null termination
  MY_DEBUG_PRINT("Loaded location slug from flash: ");
  MY_DEBUG_PRINTLN(location_slug);

  // Load WiFI Credentials
  *WIFI_SSID = preferences.getString(WIFI_SSID_KEY, "");
  MY_DEBUG_PRINT("Loaded WiFi SSID from flash: ");
  MY_DEBUG_PRINTLN(*WIFI_SSID);

  *WIFI_PASSWORD = preferences.getString(WIFI_PWD_KEY, "");

  // Load MQTT Credentials
  *MQTT_BROKER_IP = preferences.getString(MQTT_BROKER_IP_KEY, "");
  MY_DEBUG_PRINT("Loaded MQTT Broker IP from flash: ");
  MY_DEBUG_PRINTLN(*MQTT_BROKER_IP);

  *MQTT_BROKER_PORT = preferences.getUShort(MQTT_BROKER_PORT_KEY, 1883);
  MY_DEBUG_PRINT("Loaded MQTT Broker Port from flash: ");
  MY_DEBUG_PRINTLN(*MQTT_BROKER_PORT);

  return;

}
