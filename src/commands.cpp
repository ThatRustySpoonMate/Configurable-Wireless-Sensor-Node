#include "commands.hpp"
#include "MQTTTasks.hpp"



uint8_t commands_set_interval(uint32_t interval) {
    uint8_t status = 0;

    device_state.time_to_sleep = interval;

    preferences.putULong(INTERVAL_KEY, device_state.time_to_sleep);
    MY_DEBUG_PRINT("Updated sleep interval to: ");
    MY_DEBUG_PRINTLN(device_state.time_to_sleep);

    return status;
}


uint8_t commands_set_location(String location) {
    uint8_t status = 0;

    // Save to flash - will be loaded on next wake
    preferences.putString(LOCATION_SLUG_KEY, location);
    
    MY_DEBUG_PRINT("Location slug saved to flash: ");
    MY_DEBUG_PRINTLN(location);
    MY_DEBUG_PRINTLN("Will take effect on next wake from deep sleep.");

    return status;
}


uint8_t commands_factory_reset() {
    uint8_t status = 0;

    preferences.putBool(FIRST_SETUP_KEY, false);
    
    MY_DEBUG_PRINT("Factory reset command received.");
    MY_DEBUG_PRINTLN("Will take effect on next wake from deep sleep.");

    return status;
}

uint8_t commands_identify_mode(uint32_t duration) {
    uint8_t status = 0;

    MY_DEBUG_PRINT("Entering identify mode for ");
    MY_DEBUG_PRINT(duration);
    MY_DEBUG_PRINTLN(" milliseconds.");

    device_state.current_state = STATE_IDENTIFY;
    device_state.identify_duration = duration;
    device_state.identify_entered_time = millis();

    return status;
}

uint8_t commands_debug_mode(uint32_t state) {
    uint8_t status = 0;

    device_state.debug_log = state;

    preferences.putBool(DEBUG_MODE_KEY, state);

    MY_DEBUG_PRINT("Debug mode: ");
    MY_DEBUG_PRINT(state);


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


uint8_t commands_get_device_name() {
    uint8_t status = 0;

    String deviceName = preferences.getString(DEVICE_NAME_KEY, "");
    
    mqtt_transmit(MQTT_TOPIC_DEVICE_NAME, deviceName.c_str());

    return status;
}

