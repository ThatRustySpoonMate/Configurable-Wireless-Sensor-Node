#include "WifiTasks.hpp"


void setup_wifi(const char *ssid, const char *password) {
  #ifdef WIFI_REQUIRED
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

  #endif
}

bool setup_wifi_with_timeout(const char *ssid, const char *password, uint32_t timeout_ms) {

  #ifdef WIFI_REQUIRED

  MY_DEBUG_PRINTLN();
  MY_DEBUG_PRINT("Connecting to ");
  MY_DEBUG_PRINTLN(ssid);

  if(WiFi.status() == WL_CONNECTED) {
    return true;
  }

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

  #else
    MY_DEBUG_PRINT("Wifi: Not Required ");
    return 1;
  #endif
}

void wifi_disconnect() {
  #ifdef WIFI_REQUIRED

  WiFi.disconnect();

  #endif
  return;
}


int8_t wifi_get_rssi() {
  #ifdef WIFI_REQUIRED
  
  if(WiFi.isConnected()) {
    return WiFi.RSSI();
  } else {
    return 0;
  }

  #endif
}