#include "Sensors/adc_sampling.hpp"

#ifdef INTERNAL_ADC_SAMPLING

uint32_t init_internal_adc_sampling() {
    uint8_t adc_pins[] = INTERNAL_ADC_PINS;

    // Set prescribed monitored pins to INPUT mode
    for(uint8_t i = 0; i < INTERNAL_ADC_PIN_COUNT; i++) {
        pinMode(adc_pins[i], INPUT);
    }

    analogReadResolution(12);
    return 1; // Success
}

void read_internal_adc_pins(transmit_data_t *analog_pins) {
    // Get prescribed ADC pins
    uint8_t adc_pins[] = INTERNAL_ADC_PINS;

    // Read each prescribed ADC pin
    uint16_t thisReading = 0;
    for(uint8_t i = 0; i < INTERNAL_ADC_PIN_COUNT; i++) {
        thisReading = readADCAveraged(adc_pins[i], INTERNAL_ADC_SAMPLES_TO_AVERAGE);  // Fixed: use adc_pins[i] not i
        analog_pins->data_u16[i] = thisReading;
    }
}

#endif // INTERNAL_ADC_SAMPLING

// Utility function available to all sensors
uint16_t readADCAveraged(uint8_t adc_pin, uint8_t samples) {
    uint32_t total = 0;
    for(uint8_t i = 0; i < samples; i++) {
        total += analogRead(adc_pin);
    }

    return total / samples;
}