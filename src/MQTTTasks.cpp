#include "MQTTTasks.hpp"

WiFiClient espClient;
PubSubClient client(espClient);

const char *BROKER_IP;
int BROKER_PORT;
const char *DEVICE_NAME;
const char *RX_TOPIC;

extern bool debug_log;

void setup_mqtt(const char *MQTT_BROKER_IP, const int MQTT_BROKER_PORT, const char *DEV_NAME, const char *MANAGEMENT_TOPIC) {
    BROKER_IP = MQTT_BROKER_IP;
    BROKER_PORT = MQTT_BROKER_PORT;
    DEVICE_NAME = DEV_NAME;
    RX_TOPIC = MANAGEMENT_TOPIC;

    client.setServer(BROKER_IP, BROKER_PORT);

    client.setCallback(message_rx_callback);
}


void mqtt_transmit(const char *topic, const char *payload) {
    client.publish(topic, payload);

    return;
}

void mqtt_keep_alive() {
  if (!client.connected()) {
    mqtt_reconnect();
  }

  client.loop();
}


void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(DEVICE_NAME)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(RX_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void message_rx_callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;

  if(debug_log) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
  }
  
  
  for (int i = 0; i < length; i++) {
    if(debug_log) {
      Serial.print((char)message[i]);
    }
    messageTemp += (char)message[i];
  }
  if(debug_log) {
    Serial.println();
  }
  

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  // if (String(topic) == RX_TOPIC) {
  //   Serial.print("Changing output to ");
  //   if(messageTemp == "on"){
  //     Serial.println("on");
  //   }
  //   else if(messageTemp == "off"){
  //     Serial.println("off");
  //   }
  // }
}

