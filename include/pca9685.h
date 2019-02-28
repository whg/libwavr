#ifndef PCA9685_H
#define PCA9685_H

#include <avr/io.h>
#include <stddef.h>

#include "i2cm.h"

#define PCA9685_MODE1 0x00
#define PCA9685_ADDR 0x40
#define PCA9685_LED0_ADDR 0x06

#define PCA9685_MAX 128 //255

void pca9685_init()
{	
	uint8_t *buffer = i2cm_wait();
	buffer[0] = PCA9685_MODE1;

	buffer[1] = 0;
	i2cm_write(PCA9685_ADDR, 2, NULL);

	
	buffer[1] = 1 | (1<<5);
	i2cm_write(PCA9685_ADDR, 2, NULL);

	// output enable
	DDRE |= (1<<2);
    PORTE &= ~(1<<2);
}

void pca9685_set(uint8_t pin, uint8_t val)
{
	uint16_t a = 0, b = 0;

	if (val == 255) {
		a = 4096;
	} else if (val == 0) {
		b = 4096;
	} else {
		uint16_t v = val;
		v *= val;
		v >>= 4;
		b = v;
	}

	uint8_t *buffer = i2cm_wait();
	buffer[0] = PCA9685_LED0_ADDR + (pin * 4);
	buffer[1] = a & 0xff;
	buffer[2] = a >> 8;
	buffer[3] = b & 0xff;
	buffer[4] = b >> 8;

	i2cm_write(PCA9685_ADDR, 5, NULL);
	i2cm_wait();
}

#endif
