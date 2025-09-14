#include "TransmitTask.hpp"

// Global variables for transmission timing
static unsigned long transmission_start_time = 0;
static bool transmission_in_progress = false;
static bool ready_for_sleep = false;

void transmitTask_init() {
    return;
}

void transmitTask_run(transmit_data_entry_t transmitData[DATAPOINTS_NUM]) {
  static String readingStr;
  static String topicWithIndex;

  // Record when we started transmission
  transmission_start_time = millis();
  transmission_in_progress = true;
  ready_for_sleep = false;
  
  /* Convert readings to string objects then transmit them - only send data points for connected devices */

  #ifdef UPTIME_MONITORING
    readingStr = String(transmitData[UPTIME_IDX].data.data_u32[0]);
    mqtt_transmit(transmitData[UPTIME_IDX].topic, readingStr.c_str());
  #endif
  
  #ifdef SUPPLY_MONITORING_CONNECTED
  // Transmit all supply monitoring sensor readings
    for (int i = 0; i < SUPPLY_MONITORING_SENSOR_COUNT; i++) {
        readingStr = String(transmitData[SUPPLY_VOLTAGE_IDX].data.data_u16[i]);
        topicWithIndex = String(transmitData[SUPPLY_VOLTAGE_IDX].topic) + "/" + String(i);
        mqtt_transmit(topicWithIndex.c_str(), readingStr.c_str());
    }
  #endif

  #ifdef SOIL_MOISTURE_SENSOR_CONNECTED
    // Transmit all soil moisture sensor readings
    for (int i = 0; i < SOIL_MOISTURE_SENSOR_COUNT; i++) {
        readingStr = String(transmitData[SOIL_MOISTURE_IDX].data.data_u16[i]);
        topicWithIndex = String(transmitData[SOIL_MOISTURE_IDX].topic) + "/" + String(i);
        mqtt_transmit(topicWithIndex.c_str(), readingStr.c_str());
    }
  #endif

  #ifdef TEMPERATURE_SENSOR_CONNECTED
    // Transmit all temperature sensor readings
    for (int i = 0; i < TEMPERATURE_SENSOR_COUNT; i++) {
        readingStr = String(transmitData[TEMPERATURE_IDX].data.data_f32[i]);
        topicWithIndex = String(transmitData[TEMPERATURE_IDX].topic) + "/" + String(i);
        mqtt_transmit(topicWithIndex.c_str(), readingStr.c_str());
    }
  #endif

  #ifdef HUMIDITY_SENSOR_CONNECTED
    // Transmit all humidity sensor readings
    for (int i = 0; i < HUMIDITY_SENSOR_COUNT; i++) {
        readingStr = String(transmitData[HUMIDITY_IDX].data.data_f32[i]);
        topicWithIndex = String(transmitData[HUMIDITY_IDX].topic) + "/" + String(i);
        mqtt_transmit(topicWithIndex.c_str(), readingStr.c_str());
    }
  #endif

  #ifdef PRESSURE_SENSOR_CONNECTED
    // Transmit all pressure sensor readings
    for (int i = 0; i < PRESSURE_SENSOR_COUNT; i++) {
        readingStr = String(transmitData[PRESSURE_IDX].data.data_f32[i]);
        topicWithIndex = String(transmitData[PRESSURE_IDX].topic) + "/" + String(i);
        mqtt_transmit(topicWithIndex.c_str(), readingStr.c_str());
    }
  #endif

  #ifdef ALTITUDE_SENSOR_CONNECTED
    // Transmit all altitude sensor readings
    for (int i = 0; i < ALTITUDE_SENSOR_COUNT; i++) {
        readingStr = String(transmitData[ALTITUDE_IDX].data.data_f32[i]);
        topicWithIndex = String(transmitData[ALTITUDE_IDX].topic) + "/" + String(i);
        mqtt_transmit(topicWithIndex.c_str(), readingStr.c_str());
    }
  #endif

  #ifdef INTERNAL_ADC_CONNECTED
    // Transmit all internal ADC readings
    uint8_t adc_pins[] = INTERNAL_ADC_PINS;
    for (int i = 0; i < INTERNAL_ADC_PIN_COUNT; i++) {
        readingStr = String(transmitData[ANALOG_PINS_IDX].data.data_u16[i]);
        topicWithIndex = String(transmitData[ANALOG_PINS_IDX].topic) + "/" + String(adc_pins[i]);
        mqtt_transmit(topicWithIndex.c_str(), readingStr.c_str());
    }
  #endif

  #ifdef WIFI_RSSI_MONITORING_CONNECTED
  readingStr = String(transmitData[WIFI_RSSI_IDX].data.data_i8[0]);
  mqtt_transmit(transmitData[WIFI_RSSI_IDX].topic, readingStr.c_str());
  #endif

  MY_DEBUG_PRINTLN("All data queued for transmission");

}

// Check if enough time has passed since transmission started
bool transmitTask_isReadyForSleep() {
    if (!transmission_in_progress) {
        return false; // Haven't started transmission yet
    }
    
    if (ready_for_sleep) {
        return true; // Already determined we're ready
    }
    
    // Check if enough time has elapsed
    unsigned long elapsed = millis() - transmission_start_time;
    if (elapsed >= MQTT_TRANSMIT_TIME_BUFFER) {
        MY_DEBUG_PRINTLN("Transmission buffer time elapsed");
        ready_for_sleep = true;
        
        // Disconnect from MQTT now that we're ready to sleep
        mqtt_disconnect();
        
        return true;
    }
    
    return false;
}

// Reset transmission state (call this when waking up or starting new cycle)
void transmitTask_reset() {
    transmission_start_time = 0;
    transmission_in_progress = false;
    ready_for_sleep = false;
}