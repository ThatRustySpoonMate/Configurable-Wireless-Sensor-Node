#include "SensorTasks.hpp"


#ifdef BME280
Adafruit_BME280 bme; // I2C
#endif

void initSensors() {
  
  #ifdef SOIL_MOISTURE_SENSOR
  init_soil_sensor();
  #endif

  #ifdef BME280
  init_bme280();
  #endif


  return;
}

void readSensors(uint16_t *moistureReading, float *temp, float *humidity, float *baroPres, float *altitude) {

  // Power up Soil sensor
  #ifdef SOIL_MOISTURE_SENSOR
  digitalWrite(SOIL_MOISUTRE_SENS_VCC, HIGH);
  #endif

  /* Read values from BME280 */
  #ifdef BME280
  *temp = bme.readTemperature();
  *humidity = bme.readHumidity();
  *baroPres = bme.readPressure();
  *altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  #endif

  // Allow time for startup  of soil moisture sensor before taking reading
  #ifdef SOIL_MOISTURE_SENSOR
  delay(SOIL_MOISTURE_SETTLE_TIME); 
  *moistureReading = analogRead(SOIL_MOISTURE_SENS_DIN);
  digitalWrite(SOIL_MOISUTRE_SENS_VCC, LOW);  // Power down soil sensor
  #endif
  
  return;
}

uint32_t init_soil_sensor() {
  /* Initialize soil moisture sensors */ 
  pinMode(SOIL_MOISUTRE_SENS_VCC, PULLDOWN);

  return 0;
}



uint32_t init_bme280() {
  uint32_t status;
  /* Initialize BME280 */
  status = bme.begin();  
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
  }

  //bme.init();

  return status;
}