#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// ========== POWER MANAGEMENT ==========
#define DEFAULT_SLEEP_TIME_SECONDS 60           // Normal sleep time - time between readings
#define LOW_BATTERY_SLEEP_TIME_SECONDS 300      // Sleep time when battery is low (5 minutes) - currently unimplemented
#define VERY_LOW_BATTERY_SLEEP_TIME_SECONDS 600 // Sleep time when battery is very low (10 minutes) - currently unimplemented
#define CPU_FREQUENCY_MHZ 80                    // Lower frequency saves power

// ========== BATTERY MONITORING ==========
#define LOW_BATTERY_THRESHOLD_V 3.4f           // Voltage to trigger power saving
#define CRITICAL_BATTERY_THRESHOLD_V 3.2f      // Voltage to trigger aggressive power saving
#define BATTERY_ADC_SAMPLES 16                 // Number of ADC samples to average

// ========== CONNECTION TIMEOUTS ==========
#define WIFI_CONNECT_TIMEOUT_MS 30000          // 30 seconds
#define MQTT_CONNECT_TIMEOUT_MS 10000          // 10 seconds
#define WATCHDOG_TIMEOUT_SECONDS 30            // 30 seconds before watchdog triggers

// ========== MQTT CONFIGURATION ==========
#define MQTT_TOPIC_LOCATION_SLUG "home/unconfigured"
#define MQTT_TOPIC_LENGTH_MAX 100
#define MQTT_TRANSMIT_TIME_BUFFER 750          // Amount of time after queueing last message to be sent before disconnecting and sleeping

// MQTT Topic suffixes
#define MOISTURE_TOPIC_SUFFIX "/moisture"
#define TEMPERATURE_TOPIC_SUFFIX "/temperature"
#define HUMIDITY_TOPIC_SUFFIX "/humidity"
#define PRESSURE_TOPIC_SUFFIX "/pressure"
#define ALTITUDE_TOPIC_SUFFIX "/altitude"
#define SUPPLY_VOLTAGE_TOPIC_SUFFIX "/supply-voltage"
#define UPTIME_TOPIC_SUFFIX "/uptime"
#define MANAGEMENT_TOPIC_SUFFIX "/manage/unconfigured"

// ========== PREFERENCES (FLASH-EMULATED-EEPROM) CONFIGURATION ==========
#define PREFS_NAMESPACE "device"
#define UPTIME_KEY "uptime"



// ========== DEVICE CONFIGURATION ==========
// Uncomment the devices you have connected
//#define DEVICE_SOIL_MOISTURE_SENSOR
//#define DEVICE_BME280
#define DEVICE_AHT20
#define UPTIME_MONITORING   // Software feature
#define WAKE_LED
//#define SUPPLY_MONITORING // Resistor divider from supply to ADC Pin


// ========== SENSOR CONFIGURATION ==========
#define SEALEVELPRESSURE_HPA 1013.25f          // Standard sea level pressure hPa

// Soil Moisture Sensor
#ifdef DEVICE_SOIL_MOISTURE_SENSOR
#define SOIL_MOISTURE_SENS_VCC_PIN 7
#define SOIL_MOISTURE_SENS_DIN_PIN 8           // ADC1_CH4 for ESP32 Dev Board
#define SOIL_MOISTURE_SETTLE_TIME_MS 1000      // Time to wait for sensor to stabilize
#endif

// Supply Voltage Monitoring
#ifdef SUPPLY_MONITORING
#define SUPPLY_MON_ADC_PIN 3                   // ADC1_CH2 on WEMOS
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

// ========== DEBUG CONFIGURATION ==========
#define SERIAL_BAUD_RATE 115200
#define DEBUG_DEFAULT_STATE false              // Start with debug off

// ========== DERIVED CONFIGURATIONS ==========
// These are calculated from the above settings

// Microseconds conversion
#define uS_TO_S_FACTOR 1000000
#define mS_TO_S_Factor 1000

// Power saving voltage thresholds
#define MODERATE_POWER_SAVE_THRESHOLD_V 3.4f
#define AGGRESSIVE_POWER_SAVE_THRESHOLD_V 3.2f

// ADC calculation helpers
#ifdef SUPPLY_MONITORING
#define ADC_TO_VOLTAGE_FACTOR (ESP32_ADC_REFERENCE_VOLTAGE / ADC_MAX_VALUE)
#define VOLTAGE_DIVIDER_FACTOR (SUPPLY_MON_RDIV_RATIO / SUPPLY_MON_FUDGE_FACTOR_DIV)
#endif

#endif // CONFIGURATION_H