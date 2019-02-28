#define F_CPU 16000000
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "pin.h"
#include "serial.h"
#include "dmx.h"

#define enable_pin_interrupt()	 \
	PCMSK0 |= (1<<PCINT0);       \
	GIMSK |= (1<<PCIE0)

#define disable_pin_interrupt() GIMSK &= ~(1<<PCIE0)

#define setup_timer()					         \
	TCCR0A |= (1<<WGM01); /* CTC */				 \
	/* 16MHz / 250kHz = 64 steps */				 \
	TCCR0B |= (1<<CS01);  /* 8 prescale */	     \
	OCR0A = 8; /* sample once per period */	 \
	TIMSK0 |= (1<<OCIE1A) /* enable interrupt */

#define enable_timer() TCCR0B = (1<<CS01);
#define disable_timer() TCCR0B = 0
#define reset_timer() TCNT0 = 0;
#define set_timer_to_half_period() TCNT0 = 4

#define read_pin() (PINA & (1<<PA0))

#define WAITING_RESET 1
#define FOUND_RESET 2
#define READING_DATA 4

volatile int8_t bit_counter = 0;
volatile uint8_t packet_valid = 0;
volatile uint8_t packet_value = 0;
volatile uint16_t packet_counter = 0;
volatile uint8_t state = WAITING_RESET;
volatile uint16_t wanted_channel = 1;
volatile uint8_t wanted_value = 0;

void dmx_init() {

	DDRA &= ~(1<<PA0);
	
	enable_pin_interrupt();
	setup_timer();
	disable_timer();
}

ISR(PCINT0_vect) {
	uint8_t sreg = SREG;
	cli();
	if(!read_pin()) { // falling edge
		if (state == FOUND_RESET && bit_counter >= 0) {
			packet_counter = 0;
			/* if (bit_counter < 5) */
			/* 	PORTA |= (1<<PA7); */
			/* else */
			/* 	PORTA &= ~(1<<PA7); */
		}
		state = READING_DATA;
		disable_pin_interrupt();
		packet_value = 0;
		packet_valid = 1;
		set_timer_to_half_period();
		enable_timer();
	}
	else { // rising edge
		if (state == WAITING_RESET && bit_counter >= 21) {
			state = FOUND_RESET;
		}
	}
	bit_counter = -1;
	SREG = sreg;
}

ISR(TIMER0_COMPA_vect) {
	int8_t bc = bit_counter;

	if (state == READING_DATA) {
		uint8_t reading = read_pin();
		if (bc == -1 && reading) {
			packet_valid = 0;
		}
		else if (bc == 8 && !reading) {
			packet_valid = 0;
		}
		else if (bc == 9) {
			packet_valid &= reading;
			if (packet_valid) {
				if (packet_counter++ == wanted_channel) {
					wanted_value = packet_value;
					PORTA |= (1<<PA7);
					/* print_c('i'); */
				}
				disable_timer();
			}
			else if (!packet_value) {
				state = WAITING_RESET;
			}
			enable_pin_interrupt();
			return;
		}
		else if (reading) {
			packet_value &= 1 << bc;
		}
	}
	bit_counter = bc + 1;
}


int main() {

	pin_create(led_pin, A, 7, OUTPUT);
	pin_clear(led_pin);
	
	/* serial_init(); */
	dmx_init();

	sei();

	/* println_c('R'); */
	for (;;) {
		/* PORTA |= (1<<PA7);	 */
		_delay_ms(1000);
		/* PORTA &= ~(1<<PA7); */
		/* _delay_ms(1000); */
		/* println_i(wanted_value); */
	}
	
	return 0;
}
