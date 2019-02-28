#include "dmx.h"

#include <stdlib.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t channel_counter = 0;
uint16_t wanted_channels[DMX_MAX_CHANNELS];
volatile uint8_t wanted_values[DMX_MAX_CHANNELS];
volatile bool valid_packet;

static void (*received_callback)() = NULL;

void dmx_init() {
	UBRR0L = 3;
	UCSR0B = (1<<RXEN0) | (1<<RXCIE0);
	UCSR0C = (1<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00);

	sei();

	for (uint8_t i = 0; i < DMX_MAX_CHANNELS; i++) {
		wanted_channels[i] = i;
	}
}

void dmx_set_channel(uint8_t n, uint16_t channel) {
	if (n < DMX_MAX_CHANNELS)
		wanted_channels[n] = channel;
}

uint8_t dmx_get_value(uint8_t n) {
	return wanted_values[n];
}

void dmx_set_received_callback(void (*cb)()) {
	received_callback = cb;
}

#if defined(__AVR_ATmega328P__)
ISR(USART_RX_vect) {
#else
ISR(USART0_RX_vect) {
#endif		
	uint8_t status = UCSR0A;
	uint8_t d = UDR0;

	if (status & (1<<FE0)) {
		channel_counter = 0;
		valid_packet = false;
	}
	else {
		if (channel_counter == 0) {
			valid_packet = d == 0;
			if (valid_packet) {
				if (received_callback) {
					received_callback();
				}
			}
		}
		else if (valid_packet) {
			for (uint8_t i = 0; i < DMX_MAX_CHANNELS; i++) {
				if (wanted_channels[i] == channel_counter) {
					wanted_values[i] = d;
				}
			}
		}
		channel_counter++;
	}
}
