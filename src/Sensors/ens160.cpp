#include "Sensors/ens160.hpp"

#ifdef DEVICE_ENS160

#include "ScioSense_ENS160.h"
#include "main.hpp"

// Sensor instance
ScioSense_ENS160 ens160(ENS160_I2C_ADDRESS);

uint32_t init_ens160() {
    MY_DEBUG_PRINTLN("Detected ens160");
    
    bool status = ens160.begin();
    if (!status) {
        MY_DEBUG_PRINTLN("Could not find a valid ENS160 sensor, check wiring or address");
        mqtt_log_error("Error initializing ens160 sensor.");
        return 0; // Fail
    }

    ens160.setMode(ENS160_OPMODE_STD);

    MY_DEBUG_PRINTLN("ENS160 successfully Initialized");
    return 1; // Success
}

void read_ens160(transmit_data_t *aqi, transmit_data_t *tvoc, transmit_data_t *eCO2, transmit_data_t *temp, transmit_data_t *humidity) {
    uint16_t new_AQI;
    uint16_t new_TVOC;
    uint16_t new_eCO2;
    
    #if defined(TEMPERATURE_SENSOR_CONNECTED) && defined(HUMIDITY_SENSOR_CONNECTED)
    ens160.set_envdata(temp->data_f32[0], humidity->data_f32[0]);
    #endif
    
    ens160.measure();

    new_TVOC = (uint16_t)ens160.getTVOC();
    //if (new_TVOC < 500) new_TVOC = 0; 
    //else if (new_TVOC > 5000) new_TVOC = 5000;
    //else new_TVOC = new_TVOC - 500;

    new_eCO2 = (uint16_t)ens160.geteCO2();
    //if (new_eCO2 < 400) new_eCO2 = 400;
    //else if (new_eCO2 > 5000) new_eCO2 = 5000;

    new_AQI = ens160.getAQI();

    aqi->data_u16[ENS160_AQI_ID] = new_AQI;
    tvoc->data_u16[ENS160_TVOC_ID] = new_TVOC;
    eCO2->data_u16[ENS160_ECO2_ID] = new_eCO2;


    return;
}

#endif // DEVICE_ENS160
