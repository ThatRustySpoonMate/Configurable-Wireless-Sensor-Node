#include "WifiTasks.hpp"


void setup_wifi(const char *ssid, const char *password) {
  // We start by connecting to a WiFi network
  MY_DEBUG_PRINTLN();
  MY_DEBUG_PRINT("Connecting to ");
  MY_DEBUG_PRINTLN(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    MY_DEBUG_PRINT(".");
  }

  MY_DEBUG_PRINTLN("");
  MY_DEBUG_PRINTLN("WiFi connected");
  MY_DEBUG_PRINTLN("IP address: ");
  MY_DEBUG_PRINTLN(WiFi.localIP());
}

bool setup_wifi_with_timeout(const char *ssid, const char *password, uint32_t timeout_ms) {
  MY_DEBUG_PRINTLN();
  MY_DEBUG_PRINT("Connecting to ");
  MY_DEBUG_PRINTLN(ssid);

  WiFi.begin(ssid, password);
  
  uint32_t start_time = millis();
  
  while (WiFi.status() != WL_CONNECTED && (millis() - start_time < timeout_ms)) {
    delay(500);
    MY_DEBUG_PRINT(".");
    
    // Feed watchdog during connection attempt
    extern void pat_watchdog();
    pat_watchdog();
  }

  if (WiFi.status() == WL_CONNECTED) {
    MY_DEBUG_PRINTLN("");
    MY_DEBUG_PRINTLN("WiFi connected");
    MY_DEBUG_PRINTLN("IP address: ");
    MY_DEBUG_PRINTLN(WiFi.localIP());
    return true;
  } else {
    MY_DEBUG_PRINTLN("");
    MY_DEBUG_PRINTLN("WiFi connection timeout");
    return false;
  }
}

void wifi_disconnect() {
  WiFi.disconnect();

  return;
}
