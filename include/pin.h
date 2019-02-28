#ifndef PIN_H
#define PIN_H

#include <stdint.h>
#include "adc.h"

typedef struct {
	volatile uint8_t *port;
	volatile uint8_t *in_port;
	uint8_t bit;
} pin_t;

#define OUTPUT       0b001
#define INPUT        0b010
#define INPUT_PULLUP 0b110

#define LOW 0
#define HIGH 1

#define _pin_init(_port, _bit, mode) \
	if(mode & OUTPUT) DDR ## _port |= (1 << _bit); \
	else if(mode & INPUT) DDR ## _port &= ~(1 << _bit);	\
	if (mode == INPUT_PULLUP) PORT ## _port |= (1 << _bit)

#define pin_init(pin, _port, _bit, mode) \
	pin.port = &(PORT ## _port); \
	pin.in_port = &(PIN ## _port); \
	pin.bit = _bit; \
	_pin_init(_port, _bit, mode)

#define _pin_create(pin, _port, _bit, mode) \
	_pin_init(_port, _bit, mode)

#define pin_create(pin, _port, _bit, mode) \
	pin_t pin;							   \
	pin_init(pin, _port, _bit, mode)
	
#define pin_set(pin) *pin.port |= (1<<pin.bit)
#define pin_clear(pin) *pin.port &= ~(1<<pin.bit)

#define pin_read(pin) ((*pin.in_port >> pin.bit) & 1)

#define pin_adc_read(pin) adc_read8(pin.bit)

#endif
