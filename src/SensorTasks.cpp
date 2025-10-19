#include "SensorTasks.hpp"
#include "main.hpp"
#include "esp_sleep.h"
#include "WifiTasks.hpp"


// Include individual sensor headers
#ifdef DEVICE_BME280
#include "Sensors/bme280.hpp"
#endif

#ifdef DEVICE_AHT20
#include "Sensors/aht20.hpp"
#endif

#ifdef DEVICE_ENS160
#include "Sensors/ens160.hpp"
#endif

#if defined(DEVICE_DHT11) || defined(DEVICE_DHT21) || defined(DEVICE_DHT22)
#include "Sensors/dht_family.hpp"
#endif

#ifdef INTERNAL_SUPPLY_MONITORING
#include "Sensors/supply_monitor.hpp"
#endif

#ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR
#include "Sensors/capacitive_soil_moisture.hpp"
#endif

#ifdef INTERNAL_ADC_SAMPLING
#include "Sensors/adc_sampling.hpp"
#endif

#ifdef UPTIME_MONITORING
void setup_uptime();
#endif


void sensorTask_init() {
  
  uint8_t sensorStatus = 0;

  #ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR
  sensorStatus = init_soil_sensor();
  #endif

  #ifdef DEVICE_BME280
  sensorStatus = init_bme280();
  if(0 == sensorStatus) {
    mqtt_log_error("Error initializing BME280 sensor.");
  }
  #endif

  #ifdef DEVICE_AHT20
  sensorStatus = init_aht20();
  if(0 == sensorStatus) {
    mqtt_log_error("Error initializing AHT2x sensor.");
  }
  #endif

  #ifdef DEVICE_ENS160
  sensorStatus = init_ens160();
  if(0 == sensorStatus) {
    mqtt_log_error("Error initializing ENS160 sensor.");
  }
  #endif

  #if defined(DEVICE_DHT11) ||  defined(DEVICE_DHT21) || defined(DEVICE_DHT22)
  sensorStatus = init_dht_unified();
  if(0 == sensorStatus) {
    mqtt_log_error("Error initializing DHT sensor.");
  }
  #endif

  #ifdef INTERNAL_SUPPLY_MONITORING
  sensorStatus = init_supply_monitoring();
  #endif

  #ifdef INTERNAL_ADC_SAMPLING
  sensorStatus = init_internal_adc_sampling();
  #endif

  #ifdef UPTIME_MONITORING
  setup_uptime();
  #endif

  return;
}

// Stub function allows for testing without any connected sensors
void stubReadSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v, transmit_data_t *uptime, transmit_data_t *analog_pins, transmit_data_t *wifi_rssi, transmit_data_t *aqi, transmit_data_t *tvoc, transmit_data_t *eCO2) {
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

  #ifdef DEVICE_DHT11
  temp->data_f32[DHT11_TEMPERATURE_ID] = random() % 65535;
  humidity->data_f32[DHT11_HUMIDITY_ID] = random() % 65535;
  #endif

  #ifdef DEVICE_DHT21
  temp->data_f32[DHT21_TEMPERATURE_ID] = random() % 65535;
  humidity->data_f32[DHT21_HUMIDITY_ID] = random() % 65535;
  #endif

  #ifdef DEVICE_DHT22
  temp->data_f32[DHT22_TEMPERATURE_ID] = random() % 65535;
  humidity->data_f32[DHT22_HUMIDITY_ID] = random() % 65535;
  #endif

  /* Read supply voltage*/
  #ifdef INTERNAL_SUPPLY_MONITORING
  supply_v->data_u32[INTERNAL_SUPPLY_MONITORING_ID] = random() % 4200;
  #endif

  // Read Capacitive Soil Moisture Sensor
  #ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR
  moistureReading->data_u16[SOIL_MOISTURE_ID] = random() % 65535;
  #endif

  #ifdef INTERNAL_ADC_SAMPLING
  for(uint8_t i = 0; i < INTERNAL_ADC_PIN_COUNT; i++) {
    analog_pins->data_u16[i] = random() % 4096;
  }
  #endif

  // Read this sensor last as it has an optional dependency on temp/humidity data
  #ifdef DEVICE_ENS160
  aqi->data_u16[ENS160_AQI_ID] = random() % 65535;
  tvoc->data_u16[ENS160_TVOC_ID] = random() % 65535;
  eCO2->data_u16[ENS160_ECO2_ID] = random() % 65535;
  #endif

  /* Get Uptime */
  #ifdef UPTIME_MONITORING
  device_uptime += (millis() / 1000);
  uptime->data_u32[0] = device_uptime;
  #endif

  #ifdef WIFI_RSSI
  wifi_rssi->data_i8[0] = wifi_get_rssi();
  #endif

  return;
}

// Function that calls the read function of all connected sensors and packs the data correctly into the given transmit_data struct pointers
void readSensors(transmit_data_t *moistureReading, transmit_data_t *temp, transmit_data_t *humidity, transmit_data_t *baroPres, transmit_data_t *altitude, transmit_data_t *supply_v, transmit_data_t *uptime, transmit_data_t *analog_pins, transmit_data_t *wifi_rssi, transmit_data_t *aqi, transmit_data_t *tvoc, transmit_data_t *eCO2) {

  /* Read values from DEVICE_BME280 */
  #ifdef DEVICE_BME280
  read_bme280(temp, humidity, baroPres, altitude);
  #endif

  /* Read values from DEVICE_AHT20 */
  #ifdef DEVICE_AHT20
  read_aht20(temp, humidity);
  #endif

  #if defined(DEVICE_DHT11) ||  defined(DEVICE_DHT21) || defined(DEVICE_DHT22)
  read_dht_unified(temp, humidity);
  #endif

  /* Read supply voltage*/
  #ifdef INTERNAL_SUPPLY_MONITORING
  read_supply_voltage(supply_v);
  #endif

  /* Read internal ADC pins */
  #ifdef INTERNAL_ADC_SAMPLING
  read_internal_adc_pins(analog_pins);
  #endif

  // Allow time for startup of soil moisture sensor before taking reading
  #ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR
  read_capacitive_soil_moisture_sensor(moistureReading);
  #endif

  // Read this sensor last as it has an optional dependency on temp/humidity data
  #ifdef DEVICE_ENS160
  read_ens160(aqi, tvoc, eCO2, temp, humidity);
  #endif

  /* Get Uptime */
  #ifdef UPTIME_MONITORING
  device_uptime += (millis() / 1000);
  uptime->data_u32[0] = device_uptime;
  #endif

  #ifdef WIFI_RSSI
  wifi_rssi->data_i8[0] = wifi_get_rssi();
  #endif

  return;
}


/*
*   ---------------------
*   SENSOR INIT FUNCTIONS
*   --------------------- 
*/

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
    MY_DEBUG_PRINTLN("Power-on reset detected - uptime reset to 0");
  } else {
    // This is a wake from deep sleep - load saved uptime
    device_uptime = preferences.getULong(UPTIME_KEY, 0);
    MY_DEBUG_PRINT("Wake from deep sleep - loaded uptime: ");
    MY_DEBUG_PRINT(device_uptime);
    MY_DEBUG_PRINTLN(" seconds");
  }
}

