#include "SensorTasks.hpp"
#include "configuration.h"
#include "main.hpp"
#include "esp_sleep.h"


#ifdef DEVICE_BME280
Adafruit_BME280 bme; // I2C
uint32_t init_bme280();
void read_bme280(transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude);
#endif

#ifdef DEVICE_AHT20
Adafruit_AHTX0 aht20;
uint32_t init_aht20();
void read_aht20(transmit_data_t *temp, transmit_data_t *humidity);
#endif

#ifdef INTERNAL_SUPPLY_MONITORING
uint32_t init_supply_monitoring();
void read_supply_voltage(transmit_data_t *voltage);
#endif

#ifdef UPTIME_MONITORING
void setup_uptime();
#endif

#ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR
uint32_t init_soil_sensor();
#endif


void sensorTask_init() {
  
  #ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR
  init_soil_sensor();
  #endif

  #ifdef DEVICE_BME280
  init_bme280();
  #endif

  #ifdef DEVICE_AHT20
  init_aht20();
  #endif

  #ifdef INTERNAL_SUPPLY_MONITORING
  init_supply_monitoring();
  #endif

  #ifdef UPTIME_MONITORING
  setup_uptime();
  #endif


  return;
}

// Stub function allows for testing without any connected sensors
void stubReadSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v, transmit_data_t *uptime) {
  randomSeed(analogRead(0));

  /* Read values from DEVICE_BME280 */
  #ifdef DEVICE_BME280
  temp->data_f32[BME280_TEMPERATURE_ID] = random() % 65535;
  humidity->data_f32[BME280_HUMIDITY_ID] = random() % 65535;
  baroPres->data_f32[BME280_PRESSURE_ID] = random() % 65535;
  altitude->data_f32[BME280_ALTITUDE_ID] = random() % 65535;
  #endif

  /* Read values from DEVICE_AHT20 */
  #ifdef DEVICE_AHT20
  temp->data_f32[AHT20_TEMPERATURE_ID] = random() % 65535;
  humidity->data_f32[AHT20_HUMIDITY_ID] = random() % 65535;
  #endif

  /* Read supply voltage*/
  #ifdef INTERNAL_SUPPLY_MONITORING
  supply_v->data_u32[INTERNAL_SUPPLY_MONITORING_ID] = random() % 4200;
  #endif

  /* Get Uptime */
  #ifdef UPTIME_MONITORING
  device_uptime += (millis() / 1000);
  uptime->data_u32[0] = device_uptime;
  #endif

  // Read Capacitive Soil Moisture Sensor
  #ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR
  moistureReading->data_u16[SOIL_MOISTURE_ID] = random() % 65535;
  #endif

  return;
}

// Function that calls the read function of all connected sensors and packs the data correctly into the given transmit_data struct pointers
void readSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v, transmit_data_t *uptime) {

  // Power up Soil sensor
  #ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR
  digitalWrite(CAPACITIVE_SOIL_MOISTURE_SENS_VCC_PIN, HIGH);
  #endif

  /* Read values from DEVICE_BME280 */
  #ifdef DEVICE_BME280
  read_bme280(temp, humidity, baroPres, altitude);
  #endif

  /* Read values from DEVICE_AHT20 */
  #ifdef DEVICE_AHT20
  read_aht20(temp, humidity);
  #endif

  /* Read supply voltage*/
  #ifdef INTERNAL_SUPPLY_MONITORING
  read_supply_voltage(supply_v);
  #endif

  /* Get Uptime */
  #ifdef UPTIME_MONITORING
  device_uptime += (millis() / 1000);
  uptime->data_u32[0] = device_uptime;
  #endif

  // Allow time for startup of soil moisture sensor before taking reading
  #ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR
  delay(CAPACITIVE_SOIL_MOISTURE_SETTLE_TIME_MS); 
  moistureReading->data_u16[SOIL_MOISTURE_ID] = analogRead(CAPACITIVE_SOIL_MOISTURE_SENS_DIN_PIN);
  digitalWrite(CAPACITIVE_SOIL_MOISTURE_SENS_VCC_PIN, LOW);  // Power down soil sensor
  #endif
  
  return;
}

/*
*   ---------------------
*   SENSOR INIT FUNCTIONS
*   --------------------- 
*/

uint32_t init_soil_sensor() {
  #ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR

  Serial.println("Detected Soil Sensor");

  /* Initialize soil moisture sensors */ 
  pinMode(CAPACITIVE_SOIL_MOISTURE_SENS_VCC_PIN, PULLDOWN);

  Serial.println("Soil sensor successfully initialized");

  #endif

  return 0;
}


// Returns 0 - Fail, 1 - Success, 2 - Uninitialized
uint32_t init_bme280() {
  uint32_t status = 2;

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

// Returns 0 - Fail, 1 - Success, 2 - Uninitialized
uint32_t init_aht20() {
  uint32_t status = 2;

  #ifdef DEVICE_AHT20

  Serial.println("Detected AHT20");
  /* Initialize DEVICE_BME280 */

  status = aht20.begin();  
  if (!status) {
      Serial.println("Could not find a valid AHT20 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID status: "); Serial.println(aht20.getStatus(),16);
      return status;
  }

  Serial.println("AHT20 successfully Initialized");

  #endif

  return status;
}



uint32_t init_supply_monitoring() {
  uint32_t status = 2;

  #ifdef INTERNAL_SUPPLY_MONITORING

  pinMode(SUPPLY_MON_ADC_PIN, INPUT);

  analogReadResolution(12);

  status = 1;

  #endif

  return status;

}

// Function that is called in setup (whenever core resets or wakes up)
void setup_uptime() {
  // Check if this is a power-on reset or wake from deep sleep
  esp_reset_reason_t reset_reason = esp_reset_reason();
  esp_sleep_wakeup_cause_t wakeup_cause = esp_sleep_get_wakeup_cause();

  if (reset_reason == ESP_RST_POWERON || 
      reset_reason == ESP_RST_BROWNOUT || 
      reset_reason == ESP_RST_EXT ||
      (wakeup_cause != ESP_SLEEP_WAKEUP_TIMER && wakeup_cause != ESP_SLEEP_WAKEUP_EXT0 && wakeup_cause != ESP_SLEEP_WAKEUP_EXT1)) {
    // This is a power-on reset, brownout, or external reset - reset uptime
    device_uptime = 0;
    preferences.putULong(UPTIME_KEY, 0);
    DEBUG_PRINTLN("Power-on reset detected - uptime reset to 0");
  } else {
    // This is a wake from deep sleep - load saved uptime
    device_uptime = preferences.getULong(UPTIME_KEY, 0);
    DEBUG_PRINT("Wake from deep sleep - loaded uptime: ");
    DEBUG_PRINT(device_uptime);
    DEBUG_PRINTLN(" seconds");
  }
}


/*
*   ---------------------
*   SENSOR READ FUNCTIONS
*   --------------------- 
*/


void read_bme280(transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude) {
  #ifdef DEVICE_BME280

  temp->data_f32[BME280_TEMPERATURE_ID] = bme.readTemperature();
  humidity->data_f32[BME280_HUMIDITY_ID] = bme.readHumidity();
  baroPres->data_f32[BME280_PRESSURE_ID] = bme.readPressure();
  altitude->data_f32[BME280_ALTITUDE_ID] = bme.readAltitude(SEALEVELPRESSURE_HPA);

  #endif

  return;
}

void read_aht20(transmit_data_t *temp, transmit_data_t *humidity) {
  #ifdef DEVICE_AHT20

  sensors_event_t humidityEvent, tempEvent;

  aht20.getEvent(&humidityEvent, &tempEvent);

  temp->data_f32[AHT20_TEMPERATURE_ID] = tempEvent.temperature;
  humidity->data_f32[AHT20_HUMIDITY_ID] = humidityEvent.relative_humidity; 

  #endif

  return;
}


void read_supply_voltage(transmit_data_t *voltage) {
  //static float supply_reading_counts_to_mv = 10000.0 / 4095.0 * 3.3 * SUPPLY_MON_RDIV_RATIO / 10;
  

  #ifdef INTERNAL_SUPPLY_MONITORING

  uint32_t batteryVoltageRaw = 0;

  batteryVoltageRaw = readADCAveraged(SUPPLY_MON_ADC_PIN, BATTERY_ADC_SAMPLES);

  voltage->data_u16[INTERNAL_SUPPLY_MONITORING_ID] = (batteryVoltageRaw * 10000 / ADC_MAX_VALUE * ESP32_ADC_REFERENCE_VOLTAGE * SUPPLY_MON_RDIV_RATIO / 10) / SUPPLY_MON_FUDGE_FACTOR_DIV; // returns value in mV
  //voltage->data_f32[INTERNAL_SUPPLY_MONITORING_ID] = ((float) batteryVoltageRaw / 4095.0) * 3.3 * 2;
  #endif

  return;
}

float get_battery_voltage_calibrated() {
  extern uint32_t time_to_sleep; // Access global sleep time
  
  #ifdef INTERNAL_SUPPLY_MONITORING
  uint32_t raw_reading = readADCAveraged(SUPPLY_MON_ADC_PIN, BATTERY_ADC_SAMPLES); // More samples for accuracy
  
  // Improved calibration
  float voltage = ((float)raw_reading / 4095.0f) * 3.3f * SUPPLY_MON_RDIV_RATIO;
  voltage /= SUPPLY_MON_FUDGE_FACTOR_DIV;
  
  // Add low battery detection and power management
  if (voltage < AGGRESSIVE_POWER_SAVE_THRESHOLD_V) {
    DEBUG_PRINTLN("LOW BATTERY WARNING!");
    DEBUG_PRINT("Battery voltage: ");
    DEBUG_PRINTLN(voltage);
    
    // Extend sleep time to conserve power
    if (time_to_sleep < VERY_LOW_BATTERY_SLEEP_TIME_SECONDS) {
      time_to_sleep = VERY_LOW_BATTERY_SLEEP_TIME_SECONDS;
      DEBUG_PRINTLN("Extended sleep time due to low battery");
    }
  } else if (voltage < MODERATE_POWER_SAVE_THRESHOLD_V) {
    DEBUG_PRINTLN("Battery getting low");
    // Moderate power saving
    if (time_to_sleep < LOW_BATTERY_SLEEP_TIME_SECONDS) {
      time_to_sleep = LOW_BATTERY_SLEEP_TIME_SECONDS;
    }
  }
  
  return voltage;
  #else
  return 0.0f;
  #endif
}


uint16_t readADCAveraged(uint8_t adc_pin, uint8_t samples) {
  uint32_t total = 0;
  for(uint8_t i = 0; i < samples; i++) {
    total += analogRead(adc_pin);
  }

  return total / samples;
}