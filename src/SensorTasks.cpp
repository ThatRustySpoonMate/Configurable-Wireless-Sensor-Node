#include "SensorTasks.hpp"

Adafruit_BME280 bme; // I2C

void initSensors() {
    unsigned status;

    /* Initialize soil moisture sensors */ 
    pinMode(SOIL_MOISUTRE_SENS_VCC, PULLDOWN);

    /* Initialize BME280 */
    status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
    }

    //bme.init();


    return;
}

void readSensors(uint16_t *moistureReading, float *temp, float *humidity, float *baroPres, float *altitude) {
  // Power up Soil sensor
  digitalWrite(SOIL_MOISUTRE_SENS_VCC, HIGH);

  /* Read values from BME280 */
  *temp = bme.readTemperature();
  *humidity = bme.readHumidity();
  *baroPres = bme.readPressure();
  *altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  // Allow time for startup of soil moisture sensor
  delay(500); 
  // Take Reading from Soil moisture sensor
  *moistureReading = analogRead(SOIL_MOISTURE_SENS_DIN);

    // Power down soil sensor
  digitalWrite(SOIL_MOISUTRE_SENS_VCC, LOW);
  

  return;
}