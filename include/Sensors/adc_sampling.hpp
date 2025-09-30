#ifndef ADC_SAMPLING_H
#define ADC_SAMPLING_H

#include "configuration.h"
#include "SensorTasks.hpp"

#ifdef INTERNAL_ADC_SAMPLING

// Function declarations
uint8_t init_internal_adc_sampling();
void read_internal_adc_pins(transmit_data_t *analog_pins);

#endif // INTERNAL_ADC_SAMPLING

// Utility function available to all sensors
uint16_t readADCAveraged(uint8_t adc_pin, uint8_t samples);

#endif // ADC_SAMPLING_H