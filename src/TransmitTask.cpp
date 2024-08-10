#include "TransmitTask.hpp"



void transmitTask_init() {

    return;
}


void transmitTask_run(transmit_data_entry_t transmitData[DATAPOINTS_NUM]) {

  static String moistureReadingStr, tempReadingStr, humidityReadingStr, pressureReadingStr, altitudeReadingStr;
  /* Convert readings to string objects */
  moistureReadingStr = String(transmitData[SOIL_MOISTURE].data.data_u16);
  tempReadingStr     = String(transmitData[TEMPERATURE].data.data_f32);
  humidityReadingStr = String(transmitData[HUMIDITY].data.data_f32);
  pressureReadingStr = String(transmitData[PRESSURE].data.data_f32);
  altitudeReadingStr = String(transmitData[ALTITUDE].data.data_f32);

//   /* Transmit Soil moisture reading to MQTT Broker */
//   mqtt_transmit(MQTT_TOPIC_MOISTURE, moistureReadingStr.c_str());

//   /* Transmit BME280 Readings to MQTT Broker */
//   mqtt_transmit(MQTT_TOPIC_TEMPERATURE, tempReadingStr.c_str()); 
//   mqtt_transmit(MQTT_TOPIC_HUMIDITY, humidityReadingStr.c_str()); 
//   mqtt_transmit(MQTT_TOPIC_PRESSURE, pressureReadingStr.c_str()); 
//   mqtt_transmit(MQTT_TOPIC_ALTITUDE, altitudeReadingStr.c_str()); 

  mqtt_transmit(transmitData[SOIL_MOISTURE].topic, moistureReadingStr.c_str());

  /* Transmit BME280 Readings to MQTT Broker */
  mqtt_transmit(transmitData[TEMPERATURE].topic, tempReadingStr.c_str()); 
  mqtt_transmit(transmitData[HUMIDITY].topic, humidityReadingStr.c_str()); 
  mqtt_transmit(transmitData[PRESSURE].topic, pressureReadingStr.c_str()); 
  mqtt_transmit(transmitData[ALTITUDE].topic, altitudeReadingStr.c_str()); 

  // Allow time to transmit message before disconnecting from MQTT
  delay(500); 
  mqtt_disconnect();
  //wifi_disconnect();
}


/* Called once core wakes up from sleep */
// void transmitTask() {
//   static uint32_t lastTransmitTime = 0;

//   uint32_t currentTime = millis();

//   mqtt_keep_alive(); // Keep MQTT session alive


//   // Only run once per x timeframe 
//   if(millis() > lastTransmitTime + TRANSMIT_INTERVAL) {
//     // Transmit payload via MQTT
//     char temperatureString[8], humidityString[8], heatIndexString[8];

//     // Convert floats to strings
//     dtostrf(payload[Payload_Temperature], 2, 2, temperatureString);
//     dtostrf(payload[Payload_Humidity], 2, 2, humidityString);
//     dtostrf(payload[Payload_Heat_Index], 2, 2, heatIndexString);

//     // Transmit strings
//     mqtt_transmit(MQTT_TOPIC_TEMP, temperatureString);
//     mqtt_transmit(MQTT_TOPIC_HUMI, humidityString);
//     mqtt_transmit(MQTT_TOPIC_HEATINDEX, heatIndexString);

//     if(debug_log) {
//       Serial.println("Transmitted the following values to MQTT broker:"); Serial.println(temperatureString); Serial.println(humidityString); Serial.println(heatIndexString); 
//     }
    
//     lastTransmitTime = currentTime;
//   }
  
// }