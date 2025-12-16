#include "Sensors/supply_monitor.hpp"

#ifdef INTERNAL_SUPPLY_MONITORING

#include "main.hpp"


#ifdef BATTERY_CHEMISTRY_LIFEPO4
#define TABLE_SIZE 16
static const uint16_t lifepo4_voltage[] = {2500, 2540, 2800, 3000, 3050, 3200, 3230, 3250, 3260, 3280, 3300, 3330, 3350, 3380, 3450, 3650};
static const float lifepo4_soc[] =        {0.0, 0.50, 5.0, 9.50, 15.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 99.0, 99.50, 100.0};

#endif

#ifdef BATTERY_CHEMISTRY_NMC
// NMC voltage-to-SOC lookup table (per cell)

#define TABLE_SIZE 66

static const uint16_t nmc_voltage[] = {
    2830, 2921, 2997, 3068, 3131, 3186, 3230, 3269, 3302, 3328,
    3348, 3370, 3385, 3400, 3412, 3426, 3436, 3452, 3468, 3478,
    3491, 3503, 3514, 3525, 3540, 3550, 3560, 3576, 3586, 3601,
    3612, 3627, 3643, 3657, 3674, 3685, 3700, 3715, 3730, 3741,
    3756, 3766, 3779, 3792, 3802, 3814, 3826, 3838, 3854, 3870,
    3885, 3901, 3921, 3936, 3955, 3971, 3983, 3994, 4006, 4014,
    4021, 4030, 4039, 4053, 4071, 4117
};
static const float nmc_soc[] = {
    1.22, 2.47, 3.75, 5.07, 6.40, 7.76, 9.14, 10.53, 11.93, 13.35,
    14.77, 16.20, 17.63, 19.08, 20.52, 21.98, 23.44, 24.90, 26.37, 27.85,
    29.33, 30.81, 32.30, 32.98, 35.30, 36.80, 38.31, 39.83, 41.35, 42.88,
    44.41, 45.96, 47.50, 49.05, 50.61, 52.17, 53.74, 55.31, 56.90, 58.48,
    60.08, 60.17, 63.28, 64.88, 66.50, 68.11, 69.74, 71.36, 72.99, 74.64,
    76.29, 77.94, 79.61, 81.28, 82.96, 84.64, 86.33, 88.02, 89.72, 91.42,
    93.13, 94.83, 96.55, 98.27, 100.0, 100.0
};

#endif


float estimate_soc(uint16_t vbat_mv);

uint8_t init_supply_monitoring() {
    pinMode(SUPPLY_MON_ADC_PIN, INPUT);
    analogReadResolution(ADC_RESOLUTION_BITS);
    return 1; // Success
}

void read_supply_voltage(transmit_data_t *voltage) {
    uint32_t totalMillivolts = 0;
    
    // Average the millivolt readings directly
    for(uint8_t i = 0; i < BATTERY_ADC_SAMPLES; i++) {
        totalMillivolts += analogReadMilliVolts(SUPPLY_MON_ADC_PIN);
    }
    
    uint16_t avgMillivolts = totalMillivolts / BATTERY_ADC_SAMPLES;
    
    // Account for voltage divider
    voltage->data_u16[INTERNAL_SUPPLY_MONITORING_ID] = 
        (uint16_t)(avgMillivolts * SUPPLY_MON_DIVIDER_RATIO);

    
    #ifdef BATTERY_SOC_MONITORING
    device_state.battery_soc = estimate_soc(*voltage->data_u16);
    #endif 
}

float estimate_soc(uint16_t vbat_mv) {
    
    #ifdef BATTERY_CHEMISTRY_LIFEPO4
        const uint16_t *voltage_table = lifepo4_voltage;
        const float *soc_table = lifepo4_soc;
    #else
        const uint16_t *voltage_table = nmc_voltage;
        const float *soc_table = nmc_soc;
    #endif
    
    // Handle out-of-range voltages
    if (vbat_mv <= voltage_table[0]) {
        return soc_table[0];
    }
    if (vbat_mv >= voltage_table[TABLE_SIZE - 1]) {
        return soc_table[TABLE_SIZE - 1];
    }
    
    // Linear interpolation between lookup table points
    for (uint8_t i = 0; i < TABLE_SIZE - 1; i++) {
        if (vbat_mv >= voltage_table[i] && vbat_mv <= voltage_table[i + 1]) {
            // Linear interpolation formula
            float voltage_range = voltage_table[i + 1] - voltage_table[i];
            float soc_range = soc_table[i + 1] - soc_table[i];
            float voltage_offset = vbat_mv - voltage_table[i];
            
            return soc_table[i] + (voltage_offset / voltage_range) * soc_range;
        }
    }
    
    return 0.0; // Fallback (should never reach here)
}

// float get_battery_voltage_calibrated() {
    
//     uint32_t raw_reading = readADCAveraged(SUPPLY_MON_ADC_PIN, BATTERY_ADC_SAMPLES);
     
//     // Improved calibration
//     float voltage = ((float)raw_reading / 4095.0f) * 3.3f * SUPPLY_MON_RDIV_RATIO;
//     voltage /= SUPPLY_MON_FUDGE_FACTOR_DIV;
    
//     // Add low battery detection and power management
//     if (voltage < AGGRESSIVE_POWER_SAVE_THRESHOLD_V) {
//         MY_DEBUG_PRINTLN("LOW BATTERY WARNING!");
//         MY_DEBUG_PRINT("Battery voltage: ");
//         MY_DEBUG_PRINTLN(voltage);
//         #ifdef ENABLE_POWER_SAVING_MODE_ON_LOW_BATTERY
//         // Extend sleep time to conserve power
//         if (device_state.time_to_sleep < VERY_LOW_BATTERY_SLEEP_TIME_SECONDS) {
//             device_state.time_to_sleep = VERY_LOW_BATTERY_SLEEP_TIME_SECONDS;
//             MY_DEBUG_PRINTLN("Extended sleep time due to low battery");
//         }
//         #endif
//     } else if (voltage < MODERATE_POWER_SAVE_THRESHOLD_V) {
//         MY_DEBUG_PRINTLN("Battery getting low");

//         #ifdef ENABLE_POWER_SAVING_MODE_ON_LOW_BATTERY
//         // Moderate power saving
//         if (device_state.time_to_sleep < LOW_BATTERY_SLEEP_TIME_SECONDS) {
//             device_state.time_to_sleep = LOW_BATTERY_SLEEP_TIME_SECONDS;
//         }
//         #endif
//     }
    
//     return voltage;
// }

#endif // INTERNAL_SUPPLY_MONITORING