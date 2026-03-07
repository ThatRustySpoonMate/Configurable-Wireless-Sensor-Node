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

  return 0;
}

// Fast reconnect using cached channel and BSSID - skips full AP scan (~1-2s saving)
bool setup_wifi_fast(const char *ssid, const char *password, uint8_t channel, uint8_t *bssid, uint32_t timeout_ms) {
  #ifdef WIFI_REQUIRED

  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }

  MY_DEBUG_PRINTLN("Attempting fast WiFi reconnect...");

  WiFi.begin(ssid, password, channel, bssid, true);

  uint32_t start_time = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start_time < timeout_ms)) {
    delay(100);
    extern void pat_watchdog();
    pat_watchdog();
  }

  if (WiFi.status() == WL_CONNECTED) {
    MY_DEBUG_PRINTLN("Fast WiFi reconnect succeeded");
    return true;
  }

  MY_DEBUG_PRINTLN("Fast WiFi reconnect failed");
  return false;

  #else
  return true;
  #endif
}

// Cache the current connection's channel and BSSID into caller-provided buffers
void wifi_cache_connection_info(uint8_t *bssid_out, uint8_t *channel_out) {
  #ifdef WIFI_REQUIRED

  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  *channel_out = WiFi.channel();
  memcpy(bssid_out, WiFi.BSSID(), 6);

  MY_DEBUG_PRINT("Cached WiFi channel: ");
  MY_DEBUG_PRINTLN(*channel_out);

  #endif
}