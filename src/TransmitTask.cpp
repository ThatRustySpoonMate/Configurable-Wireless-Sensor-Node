#include "TransmitTask.hpp"

void transmitTask_init() {
    return;
}

void transmitTask_run(transmit_data_entry_t transmitData[DATAPOINTS_NUM]) {
  static String readingStr;
  static String topicWithIndex;
  
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

  // Allow time to transmit message before disconnecting from MQTT
  delay(MQTT_TRANSMIT_TIME_BUFFER);
  mqtt_disconnect();
  //wifi_disconnect();
}