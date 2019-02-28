#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

#include "pin.h"

int main() {

	/* pin_create(led_pin, A, 7, OUTPUT); */
	pin_create(led_pin, C, 3, OUTPUT);
	
	for (;;) {

		pin_set(led_pin);
		_delay_ms(500);
		pin_clear(led_pin);
		_delay_ms(500);

		/* PORTB |= (1<<5); */
		/* _delay_ms(200); */
		/* PORTB &= ~(1<<5); */
		/* _delay_ms(100); */
	}
	
	return 0;
}
