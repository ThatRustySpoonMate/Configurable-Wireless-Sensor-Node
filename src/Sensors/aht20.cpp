#include "Sensors/aht20.hpp"

#ifdef DEVICE_AHT20

#include <Adafruit_AHTX0.h>
#include <Adafruit_Sensor.h>
#include "main.hpp"

// Global sensor instance
static Adafruit_AHTX0 aht20;

uint8_t init_aht20() {
    MY_DEBUG_PRINTLN("Detected AHT20");
    
    uint8_t status = aht20.begin();  
    if (!status) {
        MY_DEBUG_PRINTLN("Could not find a valid AHT20 sensor, check wiring, address, sensor ID!");
        MY_DEBUG_PRINT("SensorID status: "); 
        MY_DEBUG_PRINTLN(aht20.getStatus());
        return 0; // Fail
    }

    MY_DEBUG_PRINTLN("AHT20 successfully Initialized");
    return 1; // Success
}

void read_aht20(transmit_data_t *temp, transmit_data_t *humidity) {
    sensors_event_t humidityEvent, tempEvent;
    aht20.getEvent(&humidityEvent, &tempEvent);
    
    if(AHT20_TEMPERATURE_ID != -1) {
        temp->data_f32[AHT20_TEMPERATURE_ID] = tempEvent.temperature;
    }

    if(AHT20_HUMIDITY_ID != -1) {
        humidity->data_f32[AHT20_HUMIDITY_ID] = humidityEvent.relative_humidity;
    }
}

#endif // DEVICE_AHT20
