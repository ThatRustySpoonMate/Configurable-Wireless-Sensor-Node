

# Battery Life
75mA when awake and reading sensors/publishing data and wake_led with AHT20 connected
4mA when in deep sleep


# Connections

## AHT20:
AHT20-VIN  : WEMOS 3V3
AHTO20-GND : WEMOS GND
AHT20-SCL  : WEMOS 35
AHT20-SDA  : WEMOS 33


# Configuration File
Supports a maximum of:
 - Two temperature readings
 - Two Humidity Readings
 - Two Pressure Sensor Readings
 - Two Altitude Sensor Readings
 - Four Soil Moisture Sensor Readings
 - Four Analog Pin readings @ 16 bits per reading
 - One Uptime reading
 - One Supply reading


## // ========== OUTPUT PACKET CONFIGURATION EXAMPLE ==========
#if defined(DEVICE_BME280)
#define BME280_TEMPERATURE_ID 1
#define BME280_HUMIDITY_ID 1
#define BME280_PRESSURE_ID 0
#define BME280_ALTITUDE_ID 0
#endif

#if defined(DEVICE_AHT20)
#define AHT20_TEMPERATURE_ID 0
#define AHT20_HUMIDITY_ID 0
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