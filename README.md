

# Battery Life
75mA when awake and reading sensors/publishing data and wake_led with AHT20 connected
4mA when in deep sleep

# Supported Sensors
 - BME280
 - ENS160
 - AHT2x
 - DHT11
 - DHT21
 - DHT22
 - Internal ADC
 - Resistor-Divider Supply Voltage monitoring
 - WiFi-RSSI


Supports a maximum of:
 - Two temperature readings
 - Two Humidity Readings
 - Two Pressure Sensor Readings
 - Two Altitude Sensor Readings
 - Four eCO2 Sensor Readings
 - Four TVOC Sensor Readings
 - Four AQI sensor Readings
 - Four Soil Moisture Sensor Readings
 - Four Analog Pin readings @ 16 bits per reading
 - One Uptime reading
 - One Supply reading
 - One WiFi-RSSI reading


# Connections

## AHT20:
 - AHT20-VIN  : WEMOS 3V3
 - AHTO20-GND : WEMOS GND
 - AHT20-SCL  : WEMOS 35
 - AHT20-SDA  : WEMOS 33


# Configuration File

## OUTPUT PACKET CONFIGURATION EXAMPLE
#if defined(DEVICE_BME280)<br>
#define BME280_TEMPERATURE_ID 1<br>
#define BME280_HUMIDITY_ID 1<br>
#define BME280_PRESSURE_ID 0<br>
#define BME280_ALTITUDE_ID 0<br>
#endif<br>

#if defined(DEVICE_AHT20)<br>
#define AHT20_TEMPERATURE_ID 0<br>
#define AHT20_HUMIDITY_ID 0<br>
#endif<br>

#if defined(DEVICE_CAPACITIVE_SOIL_MOISTURE_SENSOR)<br>
#define SOIL_MOISTURE_ID 0<br>
#endif<br>

#if defined(INTERNAL_SUPPLY_MONITORING)<br>
#define INTERNAL_SUPPLY_MONITORING_ID 0<br>
#endif<br>

#if defined(INTERNAL_ADC_SAMPLING)<br>
// Packet config derived from number of pins and order of the array<br>
#endif<br>