#include "Sensors/ens160.hpp"

#ifdef DEVICE_ENS160

#include "SparkFun_ENS160.h"
#include "main.hpp"

// Sensor instance
SparkFun_ENS160 ens160;

uint8_t init_ens160() {
    MY_DEBUG_PRINTLN("Detected ens160");
    
    bool status = ens160.begin(ENS160_I2C_ADDRESS);
    if (!status) {
        MY_DEBUG_PRINTLN("Could not find a valid ENS160 sensor, check wiring or address");
        return 0; // Fail
    }

    ens160.setOperatingMode(SFE_ENS160_STANDARD);

    MY_DEBUG_PRINTLN("ENS160 successfully Initialized");
    return 1; // Success
}

void read_ens160(transmit_data_t *aqi, transmit_data_t *tvoc, transmit_data_t *CO2, transmit_data_t *temp, transmit_data_t *humidity) {
    uint16_t new_AQI;
    uint16_t new_TVOC;
    uint16_t new_eCO2;

    // Check if temperature and humidity sensors are connected
    #if defined(TEMPERATURE_SENSOR_CONNECTED) && defined(HUMIDITY_SENSOR_CONNECTED)
    ens160.setTempCompensationCelsius(temp->data_f32[0]); 
    ens160.setRHCompensationFloat(humidity->data_f32[0]); 
    #endif

    // Read sensor status and handle waiting for valid state
    uint8_t ensStatus = ens160.getFlags();
    int retryCount = 0;
    const int maxRetries = 100;
    const int maxRetriesFirstStartup = 5184;

    // Handle First time startup
    while(ensStatus == 2 && retryCount < maxRetriesFirstStartup) { // Loop for up to two days
        if(retryCount % 24 == 0){
            mqtt_log_error("ENS160 Performing first time startup - This may take a few hours.");
        }
        
        delay(ENS160_STATE_FIRST_STARTUP_POLL_INTERVAL); // Delay for 25 seconds
        ensStatus = ens160.getFlags();
        retryCount++;
        pat_watchdog();

        // Prevent infinite loop
        if (retryCount >= maxRetriesFirstStartup) {
            mqtt_log_error("ENS160 timeout waiting for first time startup");
            return; // Exit early if we can't get valid data
        }

    }
    retryCount = 0;

    // Handle warm-up and state
    while(ensStatus == 1 && retryCount < maxRetries) {
        if(retryCount % 5 == 0){
            mqtt_log_error("ENS160 Warming Up State");
        }
        delay(ENS160_STATE_WARMING_POLL_INTERVAL);
        pat_watchdog();
        ensStatus = ens160.getFlags();
        retryCount++;
        
        // Prevent infinite loop
        if (retryCount >= maxRetries) {
            mqtt_log_error("ENS160 timeout waiting for valid state");
            return; // Exit early if we can't get valid data
        }
    }
    retryCount = 0;

    // Handle data invalid state
    while(ensStatus == 3 && retryCount < maxRetries) {
        if(retryCount % 30 == 0){
            mqtt_log_error("ENS160 Invalid Output State");
        }
        delay(ENS160_STATE_INVALID_POLL_INTERVAL);
        ensStatus = ens160.getFlags();
        retryCount++;
        pat_watchdog();
        
        // Prevent infinite loop
        if (retryCount >= maxRetries) {
            mqtt_log_error("ENS160 timeout waiting for valid state");
            return; // Exit early if we can't get valid data
        }
    }

    // Check if sensor is ready before reading values
    new_TVOC = (uint16_t)ens160.getTVOC();
    new_eCO2 = (uint16_t)ens160.getECO2();
    new_AQI = ens160.getAQI();

    // Update the transmit data structures
    aqi->data_u16[ENS160_AQI_ID] = new_AQI;
    tvoc->data_u16[ENS160_TVOC_ID] = new_TVOC;
    CO2->data_u16[ENS160_CO2_ID] = new_eCO2;

    if(ENS160_IDLE_AFTER_READING){
        ens160.setOperatingMode(SFE_ENS160_IDLE);
    }

    return;
}


#endif // DEVICE_ENS160
