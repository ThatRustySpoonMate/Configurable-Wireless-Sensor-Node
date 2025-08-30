#include "TransmitTask.hpp"



void transmitTask_init() {
  
  return;
}


void transmitTask_run(transmit_data_entry_t transmitData[DATAPOINTS_NUM]) {

  static String moistureReadingStr, tempReadingStr, humidityReadingStr, pressureReadingStr, altitudeReadingStr, supplyVStr, uptimeStr;

  /* Convert readings to string objects then transmit them - only send data points for connected devices */

  #ifdef SUPPLY_MONITORING_CONNECTED
  supplyVStr = String(transmitData[SUPPLY_VOLTAGE_IDX].data.data_u16);
  mqtt_transmit(transmitData[SUPPLY_VOLTAGE_IDX].topic, supplyVStr.c_str());
  #endif

  #ifdef UPTIME_MONITORING
  uptimeStr = String(transmitData[UPTIME_IDX].data.data_u32);
  mqtt_transmit(transmitData[UPTIME_IDX].topic, uptimeStr.c_str());
  #endif

  #ifdef SOIL_MOISTURE_SENSOR_CONNECTED
  moistureReadingStr = String(transmitData[SOIL_MOISTURE_IDX].data.data_u16);
  mqtt_transmit(transmitData[SOIL_MOISTURE_IDX].topic, moistureReadingStr.c_str());
  #endif

  #ifdef TEMPERATURE_SENSOR_CONNECTED
  tempReadingStr = String(transmitData[TEMPERATURE_IDX].data.data_f32);
  mqtt_transmit(transmitData[TEMPERATURE_IDX].topic, tempReadingStr.c_str()); 
  #endif

  #ifdef HUMIDITY_SENSOR_CONNECTED
  humidityReadingStr = String(transmitData[HUMIDITY_IDX].data.data_f32);
  mqtt_transmit(transmitData[HUMIDITY_IDX].topic, humidityReadingStr.c_str()); 
  #endif

  #ifdef PRESSURE_SENSOR_CONNECTED
  pressureReadingStr = String(transmitData[PRESSURE_IDX].data.data_f32);
  mqtt_transmit(transmitData[PRESSURE_IDX].topic, pressureReadingStr.c_str()); 
  #endif

  #ifdef ALTITUDE_SENSOR_CONNECTED
  altitudeReadingStr = String(transmitData[ALTITUDE_IDX].data.data_f32);
  mqtt_transmit(transmitData[ALTITUDE_IDX].topic, altitudeReadingStr.c_str()); 
  #endif

  // Allow time to transmit message before disconnecting from MQTT
  delay(MQTT_TRANSMIT_TIME_BUFFER); 
  mqtt_disconnect();
  //wifi_disconnect();
}