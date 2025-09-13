#include "Sensors/supply_monitor.hpp"

#ifdef INTERNAL_SUPPLY_MONITORING

#include "main.hpp"

uint32_t init_supply_monitoring() {
    pinMode(SUPPLY_MON_ADC_PIN, INPUT);
    analogReadResolution(12);
    return 1; // Success
}

void read_supply_voltage(transmit_data_t *voltage) {
    uint32_t batteryVoltageRaw = 0;

    batteryVoltageRaw = readADCAveraged(SUPPLY_MON_ADC_PIN, BATTERY_ADC_SAMPLES);

    // Convert to millivolts
    voltage->data_u16[INTERNAL_SUPPLY_MONITORING_ID] = (batteryVoltageRaw * 10000 / ADC_MAX_VALUE * ESP32_ADC_REFERENCE_VOLTAGE * SUPPLY_MON_RDIV_RATIO / 10) / SUPPLY_MON_FUDGE_FACTOR_DIV;
}

float get_battery_voltage_calibrated() {
    extern uint32_t time_to_sleep; // Access global sleep time - no longer needed?
    
    uint32_t raw_reading = readADCAveraged(SUPPLY_MON_ADC_PIN, BATTERY_ADC_SAMPLES);
    
    // Improved calibration
    float voltage = ((float)raw_reading / 4095.0f) * 3.3f * SUPPLY_MON_RDIV_RATIO;
    voltage /= SUPPLY_MON_FUDGE_FACTOR_DIV;
    
    // Add low battery detection and power management
    if (voltage < AGGRESSIVE_POWER_SAVE_THRESHOLD_V) {
        MY_DEBUG_PRINTLN("LOW BATTERY WARNING!");
        MY_DEBUG_PRINT("Battery voltage: ");
        MY_DEBUG_PRINTLN(voltage);
        #ifdef ENABLE_POWER_SAVING_MODE_ON_LOW_BATTERY
        // Extend sleep time to conserve power
        if (time_to_sleep < VERY_LOW_BATTERY_SLEEP_TIME_SECONDS) {
            time_to_sleep = VERY_LOW_BATTERY_SLEEP_TIME_SECONDS;
            MY_DEBUG_PRINTLN("Extended sleep time due to low battery");
        }
        #endif
    } else if (voltage < MODERATE_POWER_SAVE_THRESHOLD_V) {
        MY_DEBUG_PRINTLN("Battery getting low");

        #ifdef ENABLE_POWER_SAVING_MODE_ON_LOW_BATTERY
        // Moderate power saving
        if (time_to_sleep < LOW_BATTERY_SLEEP_TIME_SECONDS) {
            time_to_sleep = LOW_BATTERY_SLEEP_TIME_SECONDS;
        }
        #endif
    }
    
    return voltage;
}

#endif // INTERNAL_SUPPLY_MONITORING