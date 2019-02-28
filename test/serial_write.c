#define F_CPU 16000000ul

#include <avr/io.h>
#include <util/delay.h>

#include "pin.h"
#include "serial.h"

int main() {

	pin_create(led_pin, A, 7, OUTPUT);

	uart0.init(57600);

	println_str(&uart0, "Hello world!");
	
	for (;;) {
		_delay_ms(1000);
		static uint32_t counter = 0;
		pli0(++counter);

		pin_set(led_pin);
		_delay_ms(50);
		pin_clear(led_pin);

	}
	
	return 0;
}
