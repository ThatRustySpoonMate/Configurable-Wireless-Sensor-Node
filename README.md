# Configurable Wireless Sensor Node

Description
A low-power, highly-configurable, wireless IoT sensor platform for long-term environmental monitoring. Built on the ESP32, it supports various sensors, offers flexible setup and remote management, and is ideal for residential, agricultural, industrial, and research use. It has been tested to around a 100 day battery life with a standard loadout (this will depend on connected sensors and settings).

This project is highly configurable, here are some of the options it supports:
- Adjustable data output rate from continuous polling to timer-based deep sleep for low-power use
- Adjustable data output format from MQTT to Serial (csv/json/raw) - possibly HTTP request in the future
- Remote device configuration via MQTT
- Ability to interface with a good selection of environmental sensors over I2C
- Uptime monitoring
- WiFi RSSI monitoring
- Features not used are turned off to save power
- Supply voltage monitoring
- Battery SOC monitoring

## Supported sensors
- BME280
- SCD4X
- ENS160
- AHT2x
- DHT11
- DHT21
- DHT22
- Internal ADC
- Resistor-Divider Supply Voltage monitoring
- WiFi-RSSI
- Battery SOC
### Sensor combinations
Supports a maximum of:
- Two temperature sensors
- Two Humidity sensors
- Two Pressure sensors
- Two Altitude sensors
- Four CO2 sensors
- Four TVOC sensors
- Four AQI sensors
- Four Soil Moisture sensors
- Four Analog Pin readings @ 16 bits per reading
- One Uptime reading
- One Supply reading
- One Battery SOC reading
- One WiFi-RSSI reading

# Usage

## First time setup

### Prerequisites
To use these weather nodes, you require the following:

*For MQTT Connection:*
- 2.4GHz WiFi Network
- MQTT broker accessible over the WiFi network
- Something connected to the MQTT broker in order to view the data
- A PC and Serial Software (Such as Putty) to perform the first time setup
- A way to send commands over the MQTT network (***Optional***) <br>

*For serial connection:*
- A PC or other Serial host to receive the data and do first time setup

### Powering on the device

The first time the device is powered on, the LED (if configured) will be illuminated for 5 seconds before flashing in a heartbeat pattern. The 5 second phase is to give you time to connect a serial terminal, once the heartbeat pattern begins, the device is in configuration mode and you will be guided through the configuration in your serial terminal. You will be asked for the following information: 
 - Device Name
 - WiFI SSID
 - WiFi Password
 - MQTT Broker IP Address
 - MQTT Broker Port
 - MQTT topic location name

## MQTT Usage
The device will automatically output it's sensor data on various MQTT topics all starting at the configured device location and ending with the sensor and sensor ID. For example, the first temperature reading for a sensor with location 'home/downstairs' will be at 'home/downstairs/temperature/0'.  

After sending all of the readings, the device will wait for a configurable amount of time for you to be able to send it commands and for it to reply.

You can send messages on certain topics of the device in order to change it's behaviour or query variables. Here is a list of all topics it is setup to receive:  
```
home/downstairs/testing/query/firmware-version
home/downstairs/testing/query/device-name

home/downstairs/testing/manage/interval
home/downstairs/testing/manage/location
home/downstairs/testing/manage/factory-reset
home/downstairs/testing/manage/identify
home/downstairs/testing/manage/debug
```
'Manage' messages expect a payload. After receiving a 'manage' message, the device will acknowledge it by publishing  on the '/acknowledge' topic 
```
home/downstairs/testing/acknowledge
```
'Query' messages do not expect a payload. After receiving a query message, the device will respond on the topic it received the message on without the '/query' portion, the response will include a message. 

## MQTT Commands
<img width="765" height="535" alt="image" src="https://github.com/user-attachments/assets/96ce2eb1-a2f2-4a87-9512-f7d4a3e9ebd4" />



## LED Blink Codes
**Rapid blinking** (~10hz) - Error handler - critical failure <br>
**Slow blinking** (~2hz) - Identify state <br>
**Heartbeat pattern** (two short pulses on, one long pulse off) - serial config mode <br>

### Continuous sampling mode
If you set the interval to 0, the device will never enter deep sleep and will read and transmit data as fast as it can via it's configured channels. This will persist until the device is manually rebooted.


## Factory Reset
Can either be triggered via MQTT or a shorting a pin to 3v3
MQTT:
```
Send a message (content doesn't matter) to the nodes /manage/factory-reset topic
home/downstairs/testing/manage/factory-reset
```

Alternatively if you have access to the hardware, the factory-reset pin is configured in the firmware's configuration.h file, however the defaults is GPIO pin 2, which is the 2nd pin down from the top on the right-column of the left-bank of pins (i.e. Left-Bank - right column - second row) when the USB-C connector is oriented at the bottom of the board.  Short this pin to 3v3 (left bank - left column - last pin) while the device is awake to initiate a factory reset.

Upon successful factory reset, the device will power off, then power back on, if configured the wake LED will stay on for a few seconds, then begin flashing in a heartbeat pattern while it awaits serial configuration. 


## Debugging
If you are encountering strange issues, you can enable debug mode by either compiling the firmware with it enabled or by sending a payload of 1 to MQTT Topic home/downstairs/testing/manage/debug. After you have done this, the Weather node will begin spitting our verbose debug info over the serial port. To read this, plug in your PC to the Weather Node and open a serial monitor at the configured baud rate.
