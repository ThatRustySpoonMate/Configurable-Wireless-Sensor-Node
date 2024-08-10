#include "SensorTasks.hpp"


#ifdef BME280
Adafruit_BME280 bme; // I2C
#endif

void sensorTask_init() {
  
  #ifdef SOIL_MOISTURE_SENSOR
  init_soil_sensor();
  #endif

  #ifdef BME280
  init_bme280();
  #endif


  return;
}

void readSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude) {

  // Power up Soil sensor
  #ifdef SOIL_MOISTURE_SENSOR
  digitalWrite(SOIL_MOISUTRE_SENS_VCC, HIGH);
  #endif




  /* Read values from BME280 */
  #ifdef BME280
  read_bme280(temp, humidity, baroPres, altitude);
  #endif




  // Allow time for startup of soil moisture sensor before taking reading
  #ifdef SOIL_MOISTURE_SENSOR
  delay(SOIL_MOISTURE_SETTLE_TIME); 
  moistureReading->data_u16 = analogRead(SOIL_MOISTURE_SENS_DIN);
  digitalWrite(SOIL_MOISUTRE_SENS_VCC, LOW);  // Power down soil sensor
  #endif
  
  return;
}

uint32_t init_soil_sensor() {

  Serial.println("Detected Soil Sensor");

  /* Initialize soil moisture sensors */ 
  pinMode(SOIL_MOISUTRE_SENS_VCC, PULLDOWN);

  Serial.println("Soil sensor successfully initialized");

  return 0;
}



uint32_t init_bme280() {
  uint32_t status;

  Serial.println("Detected BME280");
  /* Initialize BME280 */
  status = bme.begin();  
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
  }

  Serial.println("BME280 successfully Initialized");

  //bme.init();

  return status;
}


void read_bme280(transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude) {
  temp->data_f32 = bme.readTemperature();
  humidity->data_f32 = bme.readHumidity();
  baroPres->data_f32 = bme.readPressure();
  altitude->data_f32 = bme.readAltitude(SEALEVELPRESSURE_HPA);

  return;
}