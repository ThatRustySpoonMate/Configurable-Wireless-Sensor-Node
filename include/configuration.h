#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// ========== SIMULATION MODE ==========
#define SIMULATION_MODE false                   // Change to true to simulate connected devices (the ones you specify down below)

// ========== POWER MANAGEMENT ==========
#define DEFAULT_SLEEP_TIME_SECONDS 30           // Normal sleep time - time between readings
#define LOW_BATTERY_SLEEP_TIME_SECONDS 300      // Sleep time when battery is low (5 minutes) - currently unimplemented
#define VERY_LOW_BATTERY_SLEEP_TIME_SECONDS 600 // Sleep time when battery is very low (10 minutes) - currently unimplemented
#define CPU_FREQUENCY_MHZ 80                    // Lower frequency saves power

// ========== BATTERY MONITORING ==========
#define ENABLE_POWER_SAVING_MODE_ON_LOW_BATTERY // Enables the modification of sleep timer when battery crosses below thresholds
#define MODERATE_POWER_SAVE_THRESHOLD_V 3.0f    // Voltage to trigger power saving
#define AGGRESSIVE_POWER_SAVE_THRESHOLD_V 2.7f  // Voltage to trigger aggressive power saving
#define BATTERY_ADC_SAMPLES 16                  // Number of ADC samples to average

// ========== CONNECTION TYPES ==========
#define DATA_OUTPUT_OVER_MQTT                   // ENABLE DATA OUT OVER MQTT
//#define DATA_OUTPUT_OVER_SERIAL                 // ENABLE DATA OUT OVER SERIAL

#ifdef DATA_OUTPUT_OVER_MQTT 
    #define WIFI_REQUIRED
    #define MQTT_REQUIRED
#endif

// ========== CONNECTION TIMEOUTS ==========
#define WIFI_CONNECT_TIMEOUT_MS 30000           // 30 seconds
#define MQTT_CONNECT_TIMEOUT_MS 10000           // 10 seconds
#define WATCHDOG_TIMEOUT_SECONDS 30             // 30 seconds before watchdog triggers

// ========== MQTT CONFIGURATION ==========
#define MQTT_TOPIC_LOCATION_SLUG "home/new-device"  // Default location slug (can be changed via MQTT)
#define MQTT_TOPIC_LENGTH_MAX 100
#define MQTT_TRANSMIT_TIME_BUFFER 3000                    // Amount of time after queueing last message to be sent before disconnecting and sleeping, this is also the window of time that you have to issue commands to the device over MQTT, decrease to increase battery life if you don't need to configure it once deployed. Increase if your MQTT messages are not reliably coming through (especially on slower networks)

// MQTT Data topic suffixes - Transmit
#define MOISTURE_TOPIC_SUFFIX "/moisture"
#define TEMPERATURE_TOPIC_SUFFIX "/temperature"
#define HUMIDITY_TOPIC_SUFFIX "/humidity"
#define AQI_TOPIC_SUFFIC "/aqi"
#define TVOC_TOPIC_SUFFIC "/tvoc"
#define CO2_TOPIC_SUFFIC "/CO2"
#define PRESSURE_TOPIC_SUFFIX "/pressure"
#define ALTITUDE_TOPIC_SUFFIX "/altitude"
#define SUPPLY_VOLTAGE_TOPIC_SUFFIX "/supply-voltage"
#define ANALOG_PINS_TOPIC_SUFFIX "/analog-pins"
#define UPTIME_TOPIC_SUFFIX "/uptime"
#define WIFI_RSSI_TOPIC_SUFFIX "/wifi-rssi"
#define ERRORS_TOPIC_SUFFIX "/errors"
#define FIRMWARE_VERSION_TOPIC_SUFFIX "/firmwareversion"
// MQTT Management topic suffixes - Receive
#define MANAGEMENT_OUTPUT_INTERVAL_TOPIC_SUFFIX "/manage/interval"
#define MANAGEMENT_LOCATION_TOPIC_SUFFIX "/manage/location"
// MQTT Query topic suffixes - Receive 
#define QUERY_FIRMWARE_VERSION_TOPIC_SUFFIX "/query/firmwareversion"
// MQTT Acknowledge topic suffix
#define ACKNOWLEDGE_TOPIC_SUFFIX "/acknowledge"

// ========== SERIAL CONFIGURATION ==========
#define SERIAL_BAUD_RATE 115200
#define DEBUG_DEFAULT_STATE false           // Start with debug off
//#define SERIAL_DATA_OUT_FORMAT_CSV          // RECCOMEND SELECTING 1
//#define SERIAL_DATA_OUT_FORMAT_JSON         // RECCOMEND SELECTING 1
//#define SERIAL_DATA_OUT_FORMAT_HUMAN        // RECCOMEND SELECTING 1

// ========== PREFERENCES (FLASH-EMULATED-EEPROM) CONFIGURATION ==========
#define PREFS_NAMESPACE "device"
#define UPTIME_KEY "uptime"
#define INTERVAL_KEY "interval"
#define LOCATION_SLUG_KEY "location"


// ========== DEVICE CONFIGURATION ==========
// Uncomment the devices you have connected
#define DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR
//#define DEVICE_BME280
//#define DEVICE_SCD4X
#define DEVICE_AHT20                    // Use this for all AHT2x sensors
//#define DEVICE_ENS160
//#define DEVICE_DHT11
//#define DEVICE_DHT21
//#define DEVICE_DHT22
#define INTERNAL_SUPPLY_MONITORING      // Resistor divider from supply to ADC Pin
//#define INTERNAL_ADC_SAMPLING 

// Software Features
#define UPTIME_MONITORING            // Software feature
#define WAKE_LED                     // Toggle an LED on during wake and off during sleep
#define WIFI_RSSI                    // Output the WiFi RSSI 


// ========== SENSOR CONSTANTS ==========
#define SEALEVELPRESSURE_HPA 1013.25f          // Standard sea level pressure hPa
#define SENSOR_ALTITUDE 17                     // Metres above sea level


// Soil Moisture Sensor
#ifdef DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR
#define CAPACITIVE_SOIL_MOISTURE_SENS_VCC_PIN 7
#define CAPACITIVE_SOIL_MOISTURE_SENS_DIN_PIN 8           // ADC1_CH4 for WEMOS S2 Mini
#define CAPACITIVE_SOIL_MOISTURE_SETTLE_TIME_MS 1000      // Time to wait for sensor to stabilize
#define CAPACITIVE_SOIL_MOISTURE_SAMPLES_TO_AVERAGE 4
#endif

// Supply Voltage Monitoring
#ifdef INTERNAL_SUPPLY_MONITORING
#define SUPPLY_MON_ADC_PIN 3                   // ADC1_CH2 on WEMOS S2 Mini
#define SUPPLY_MON_R1 100000.0f                // 100k ohm - upper resistor
#define SUPPLY_MON_R2 100000.0f                // 100k ohm - lower resistor
#define SUPPLY_MON_RDIV_RATIO (1.0f / (SUPPLY_MON_R2 / (SUPPLY_MON_R1 + SUPPLY_MON_R2)))
#define SUPPLY_MON_FUDGE_FACTOR_DIV 1.25200f   // Calibration factor
#define ADC_RESOLUTION_BITS 12
#define ADC_MAX_VALUE 4095
#define ESP32_ADC_REFERENCE_VOLTAGE 3.3f
#endif

// Wake LED configuration
#ifdef WAKE_LED
#define WAKE_LED_PIN LED_BUILTIN
#endif

// Internal ADC configuration
#ifdef INTERNAL_ADC_SAMPLING                    
#define INTERNAL_ADC_PINS {0, 1, 3, 5}          // Currently supports a maximum of 4 readings
#define INTERNAL_ADC_PIN_COUNT 4                // Currently supports a maximum of 4 readings
#define INTERNAL_ADC_SAMPLES_TO_AVERAGE 8       // Per-pin, Acceptable values: 1-255
#endif

// BME280 Sensor Configuration
#ifdef DEVICE_BME280
#define BME280_ADDR 0x76            // 0x76 or 0x77
#endif

// ENS160
#ifdef DEVICE_ENS160
#define ENS160_I2C_ADDRESS 0x53     // 0X52: ADDR low, 0x53: ADDR high
#define ENS160_IDLE_AFTER_READING false // Set to false to disable. If true, will enter idle state after a reading is taken, this presumably lowers power consumption, but results in a longer startup time for next reading (~3m from my testing). Only set to true if you have a large value for DEFAULT_SLEEP_TIME_SECONDS (e.g. 10+ minutes)
#define ENS160_SLEEP_AFTER_READING false // Set to false to disable. If true, will enter sleep state after a reading is taken, this lowers power consumption, but results in a longer startup time for next reading (~3m from my testing). Only set to true if you have a large value for DEFAULT_SLEEP_TIME_SECONDS (e.g. 10+ minutes)
#define ENS160_STATE_FIRST_STARTUP_POLL_INTERVAL 25000 // ms
#define ENS160_STATE_WARMING_POLL_INTERVAL 3000 // ms
#define ENS160_STATE_INVALID_POLL_INTERVAL 50 // ms
#endif

// DHT11 Sensor Configuration
#ifdef DEVICE_DHT11
#define DHT11_OUT_PIN 13            // Data Out pin of DHT11
#endif

// DHT21 Sensor Configuration
#ifdef DEVICE_DHT21
#define DHT21_OUT_PIN 13            // Data Out pin of DHT21
#endif

// DHT22 Sensor Configuration
#ifdef DEVICE_DHT22
#define DHT22_OUT_PIN 13            // Data Out pin of DHT22
#endif

// ========== OUTPUT PACKET CONFIGURATION ==========
// YOU MUST GIVE ALL DEVICES A UNIQUE PACKET ID FOR EACH DATA CLASS THEY PROVIDE (INCREMENT IN 1's)
// These will be the values you see in the MQTT topic outputs (e.g. .../temperature/0)
#if defined(DEVICE_BME280)
#define BME280_TEMPERATURE_ID 0
#define BME280_HUMIDITY_ID 0
#define BME280_PRESSURE_ID 0
#define BME280_ALTITUDE_ID 0
#endif

#if defined(DEVICE_SCD4X)
#define SCD4X_CO2_ID 0
#define SCD4X_TEMPERATURE_ID 0
#define SCD4X_HUMIDITY_ID 0
#endif

#if defined(DEVICE_AHT20)
#define AHT20_TEMPERATURE_ID 0
#define AHT20_HUMIDITY_ID 0
#endif

#if defined(DEVICE_ENS160)
#define ENS160_AQI_ID 0
#define ENS160_TVOC_ID 0
#define ENS160_CO2_ID 0
#endif

#if defined(DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR)
#define SOIL_MOISTURE_ID 0
#endif

#if defined(INTERNAL_SUPPLY_MONITORING)
#define INTERNAL_SUPPLY_MONITORING_ID 0
#endif

#if defined(INTERNAL_ADC_SAMPLING)
// Packet config derived from number of pins and order of the array
#endif

#if defined(DEVICE_DHT11)
#define DHT11_TEMPERATURE_ID 0
#define DHT11_HUMIDITY_ID 0
#endif

#if defined(DEVICE_DHT21)
#define DHT21_TEMPERATURE_ID 0
#define DHT21_HUMIDITY_ID 0
#endif

#if defined(DEVICE_DHT22)
#define DHT22_TEMPERATURE_ID 0
#define DHT22_HUMIDITY_ID 0
#endif

// ========== DERIVED CONFIGURATIONS ==========
// These are calculated from the above settings

// Microseconds conversion
#define uS_TO_S_FACTOR 1000000
#define mS_TO_S_Factor 1000

// ADC calculation helpers
#ifdef INTERNAL_SUPPLY_MONITORING
#define ADC_TO_VOLTAGE_FACTOR (ESP32_ADC_REFERENCE_VOLTAGE / ADC_MAX_VALUE)
#define VOLTAGE_DIVIDER_FACTOR (SUPPLY_MON_RDIV_RATIO / SUPPLY_MON_FUDGE_FACTOR_DIV)
#endif


// ========== SENSOR CLASS ENUMERATIONS ==========
#if defined(DEVICE_BME280)
#define TEMPERATURE_SENSOR_CONNECTED
#define HUMIDITY_SENSOR_CONNECTED
#define PRESSURE_SENSOR_CONNECTED
#define ALTITUDE_SENSOR_CONNECTED
#define HAS_BME280 1
#else
#define HAS_BME280 0
#endif

#if defined(DEVICE_SCD4X)
#define TEMPERATURE_SENSOR_CONNECTED
#define HUMIDITY_SENSOR_CONNECTED
#define CO2_SENSOR_CONNECTED
#define HAS_SCD4X 1
#else
#define HAS_SCD4X 0
#endif

#if defined(DEVICE_AHT20)
#define TEMPERATURE_SENSOR_CONNECTED
#define HUMIDITY_SENSOR_CONNECTED
#define HAS_AHT20 1
#else
#define HAS_AHT20 0
#endif

#if defined(DEVICE_ENS160)
#define AQI_SENSOR_CONNECTED
#define TVOC_SENSOR_CONNECTED
#define CO2_SENSOR_CONNECTED
#define HAS_ENS160 1
#else
#define HAS_ENS160 0
#endif

#if defined(DEVICE_DHT11)
#define TEMPERATURE_SENSOR_CONNECTED
#define HUMIDITY_SENSOR_CONNECTED
#define HAS_DHT11 1
#else
#define HAS_DHT11 0
#endif

#if defined(DEVICE_DHT21)
#define TEMPERATURE_SENSOR_CONNECTED
#define HUMIDITY_SENSOR_CONNECTED
#define HAS_DHT21 1
#else
#define HAS_DHT21 0
#endif

#if defined(DEVICE_DHT22)
#define TEMPERATURE_SENSOR_CONNECTED
#define HUMIDITY_SENSOR_CONNECTED
#define HAS_DHT22 1
#else
#define HAS_DHT22 0
#endif

#if defined(DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR)
#define SOIL_MOISTURE_SENSOR_CONNECTED
#define HAS_CAPACITIVE_SOIL_MOISTURE 1
#else
#define HAS_CAPACITIVE_SOIL_MOISTURE 0
#endif

#if defined(INTERNAL_SUPPLY_MONITORING)
#define SUPPLY_MONITORING_CONNECTED
#define HAS_INTERNAL_SUPPLY_MONITORING 1
#else
#define HAS_INTERNAL_SUPPLY_MONITORING 0
#endif

#if defined(WAKE_LED)
#define WAKE_LED_CONNECTED
#endif

#if defined(INTERNAL_ADC_SAMPLING)
#define INTERNAL_ADC_CONNECTED
#endif

#if defined(WIFI_RSSI)
#define WIFI_RSSI_MONITORING_CONNECTED
#endif


// ========== SENSOR COUNT CALCULATIONS ==========
#define TEMPERATURE_SENSOR_COUNT (HAS_BME280 + HAS_AHT20 + HAS_DHT11 + HAS_DHT21 + HAS_DHT22 + HAS_SCD4X)
#define HUMIDITY_SENSOR_COUNT (HAS_BME280 + HAS_AHT20 + HAS_DHT11 + HAS_DHT21 + HAS_DHT22 + HAS_SCD4X)
#define PRESSURE_SENSOR_COUNT (HAS_BME280)
#define ALTITUDE_SENSOR_COUNT (HAS_BME280)
#define SOIL_MOISTURE_SENSOR_COUNT (HAS_CAPACITIVE_SOIL_MOISTURE)
#define SUPPLY_MONITORING_SENSOR_COUNT (HAS_INTERNAL_SUPPLY_MONITORING)
#define AQI_SENSOR_COUNT (HAS_ENS160)
#define TVOC_SENSOR_COUNT (HAS_ENS160)
#define CO2_SENSOR_COUNT (HAS_ENS160 + HAS_SCD4X)
// Internal ADC count is derived from from number of pins
#endif // CONFIGURATION_H