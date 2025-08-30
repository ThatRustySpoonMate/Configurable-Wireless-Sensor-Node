#include "WifiTasks.hpp"


void setup_wifi(const char *ssid, const char *password) {
  // We start by connecting to a WiFi network
  DEBUG_PRINTLN();
  DEBUG_PRINT("Connecting to ");
  DEBUG_PRINTLN(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DEBUG_PRINT(".");
  }

  DEBUG_PRINTLN("");
  DEBUG_PRINTLN("WiFi connected");
  DEBUG_PRINTLN("IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());
}

bool setup_wifi_with_timeout(const char *ssid, const char *password, uint32_t timeout_ms) {
  DEBUG_PRINTLN();
  DEBUG_PRINT("Connecting to ");
  DEBUG_PRINTLN(ssid);

  WiFi.begin(ssid, password);
  
  uint32_t start_time = millis();
  
  while (WiFi.status() != WL_CONNECTED && (millis() - start_time < timeout_ms)) {
    delay(500);
    DEBUG_PRINT(".");
    
    // Feed watchdog during connection attempt
    extern void feed_watchdog();
    feed_watchdog();
  }

  if (WiFi.status() == WL_CONNECTED) {
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("WiFi connected");
    DEBUG_PRINTLN("IP address: ");
    DEBUG_PRINTLN(WiFi.localIP());
    return true;
  } else {
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("WiFi connection timeout");
    return false;
  }
}

void wifi_disconnect() {
  WiFi.disconnect();

  return;
}
