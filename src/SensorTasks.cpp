#include "SensorTasks.hpp"


#ifdef DEVICE_BME280
Adafruit_BME280 bme; // I2C
uint32_t init_bme280();
void read_bme280(transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude);
#endif

#ifdef SUPPLY_MONITORING
void init_supply_monitoring();
void read_supply_voltage(transmit_data_t *voltage);
#endif

void sensorTask_init() {
  
  #ifdef DEVICE_SOIL_MOISTURE_SENSOR
  init_soil_sensor();
  #endif

  #ifdef DEVICE_BME280
  init_bme280();
  #endif

  #ifdef SUPPLY_MONITORING
  init_supply_monitoring();
  #endif


  return;
}

// Stub function allows for testing without any connected sensors
void stubReadSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v) {

  moistureReading->data_u16 = random() % 65535;
  supply_v->data_u32 = random() % 4200;
  temp->data_f32 = random() % 65535;
  humidity->data_f32 = random() % 65535;
  baroPres->data_f32 = random() % 65535;
  altitude->data_f32 = random() % 65535;

  return;
}

// Function that calls the read function of all connected sensors and packs the data correctly into the given transmit_data struct pointers
void readSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v) {

  // Power up Soil sensor
  #ifdef DEVICE_SOIL_MOISTURE_SENSOR
  digitalWrite(SOIL_MOISUTRE_SENS_VCC, HIGH);
  #endif

  /* Read values from DEVICE_BME280 */
  #ifdef DEVICE_BME280
  read_bme280(temp, humidity, baroPres, altitude);
  #endif

  /* Read supply voltage*/
  #ifdef SUPPLY_MONITORING
  read_supply_voltage(supply_v);
  #endif

  // Allow time for startup of soil moisture sensor before taking reading
  #ifdef DEVICE_SOIL_MOISTURE_SENSOR
  delay(SOIL_MOISTURE_SETTLE_TIME); 
  moistureReading->data_u16 = analogRead(SOIL_MOISTURE_SENS_DIN);
  digitalWrite(SOIL_MOISUTRE_SENS_VCC, LOW);  // Power down soil sensor
  #endif
  
  return;
}

uint32_t init_soil_sensor() {
  #ifdef DEVICE_SOIL_MOISTURE_SENSOR

  Serial.println("Detected Soil Sensor");

  /* Initialize soil moisture sensors */ 
  pinMode(SOIL_MOISUTRE_SENS_VCC, PULLDOWN);

  Serial.println("Soil sensor successfully initialized");

  #endif

  return 0;
}



uint32_t init_bme280() {
  uint32_t status = 0;

  #ifdef DEVICE_BME280

  Serial.println("Detected BME280");
  /* Initialize DEVICE_BME280 */
  status = bme.begin();  
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
  }

  Serial.println("BME280 successfully Initialized");

  //bme.init();

  #endif

  return status;
}


void read_bme280(transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude) {
  #ifdef DEVICE_BME280

  temp->data_f32 = bme.readTemperature();
  humidity->data_f32 = bme.readHumidity();
  baroPres->data_f32 = bme.readPressure();
  altitude->data_f32 = bme.readAltitude(SEALEVELPRESSURE_HPA);

  #endif

  return;
}


void init_supply_monitoring() {

  #ifdef SUPPLY_MONITORING

  pinMode(SUPPLY_MON_ADC_PIN, INPUT);

  analogReadResolution(12);

  #endif

  return;

}


void read_supply_voltage(transmit_data_t *voltage) {
  //static float supply_reading_counts_to_mv = 10000.0 / 4095.0 * 3.3 * SUPPLY_MON_RDIV_RATIO / 10;
  

  #ifdef SUPPLY_MONITORING

  uint32_t batteryVoltageRaw = 0;

  batteryVoltageRaw = readADCAveraged(SUPPLY_MON_ADC_PIN, 10);

  voltage->data_u16 = (batteryVoltageRaw * 10000 / 4095 * 3.3 * SUPPLY_MON_RDIV_RATIO / 10) / SUPPLY_MON_FUDGE_FACTOR_DIV; // returns value in mV
  //voltage->data_f32 = ((float) batteryVoltageRaw / 4095.0) * 3.3 * 2;
  #endif

  return;
}


uint16_t readADCAveraged(uint8_t adc_pin, uint8_t samples) {
  uint32_t total;
  for(uint8_t i = 0; i < samples; i++) {
    total += analogRead(adc_pin);
  }

  return total / samples;
}