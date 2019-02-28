#include "adc.h"

#include <avr/io.h>

void adc_init(uint8_t options) {
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__)
	ADMUX |= (1<<REFS0); // reference is AVcc with cap on AREF
#endif

	if (options & ADC_RIGHT_ADJUST) {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__)
		ADMUX |= (1<<ADLAR);
#elif defined(__AVR_ATtiny841__)
		ADCSRB |= (1<<ADCSRB);
#endif
	}
	
	ADCSRA |= (1<<ADEN);

}

void adc_start(uint8_t *number) {
#if defined(__AVR_ATmega328P__)
	ADMUX = (ADMUX & 0b11111000) | *number;
#elif defined(__AVR_ATtiny841__)
	ADMUXA = (ADMUXA & 0b11111000) | *number;
#endif
	ADCSRA |= (1<<ADSC);
	while (bit_is_set(ADCSRA, ADSC));

}

uint8_t adc_read8(uint8_t number) {
	adc_start(&number);
	return ADCH;
}

uint16_t adc_read16(uint8_t number) {
	adc_start(&number);
	uint16_t v = ADC; // get compiler to merge ADCH & ADCL
	return v;
}
