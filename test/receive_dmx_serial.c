#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "serial.h"
#include "dmx.h"

int main() {

	uart1.init(57600);
	dmx_init();

	sei();
	uint8_t value = 0;
	for (;;) {
		uint8_t v = dmx_get_value();
		if (v != value) {
			pli1(v);
			value = v;
		}
		_delay_ms(10);
	}
	
	return 0;
}
