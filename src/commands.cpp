#include "commands.hpp"
#include "MQTTTasks.hpp"


uint8_t commands_set_interval(uint32_t interval) {
    uint8_t status = 0;

    preferences.putULong(INTERVAL_KEY, time_to_sleep);
    MY_DEBUG_PRINT("Updated sleep interval to: ");
    MY_DEBUG_PRINTLN(time_to_sleep);

    return status;
}


uint8_t commands_set_location(String location) {
    uint8_t status = 0;

    // Save to flash - will be loaded on next wake
    preferences.putString(LOCATION_SLUG_KEY, location);
    
    MY_DEBUG_PRINT("Location slug saved to flash: ");
    MY_DEBUG_PRINTLN(location);
    MY_DEBUG_PRINTLN("Will take effect on next wake from deep sleep");

    return status;
}


uint8_t commands_get_firmware_version() {
    uint8_t status = 0;

    String version = String(FIRMWARE_VERSION_MAJOR) + "." + 
                    String(FIRMWARE_VERSION_MINOR) + "." + 
                    String(FIRMWARE_VERSION_MICRO);
    
    mqtt_transmit(MQTT_TOPIC_FIRMWARE_VERSION, version.c_str());

    return status;
}