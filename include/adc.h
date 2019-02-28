#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#define ADC_FACTORY_OPTIONS 0
#define ADC_RIGHT_ADJUST    0b0001

#define ADC_DEFAULT_OPTIONS ADC_RIGHT_ADJUST

void adc_init(uint8_t options);
uint8_t adc_read8(uint8_t number);
uint16_t adc_read16(uint8_t number);

#endif
