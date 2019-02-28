#if defined(__AVR_ATmega328PB__)

#ifndef I2CM_H
#define I2CM_H

#include <stdint.h>

#ifndef I2CM_FREQ
#define I2CM_FREQ 100000UL
#endif

#ifndef I2CM_BUFFER_LENGTH
#define I2CM_BUFFER_LENGTH 8
#endif

void i2cm_init();
void i2cm_write(uint8_t address, uint8_t length, void (*callback)(uint8_t, uint8_t *));
void i2cm_read(uint8_t address, uint8_t length, void (*callback)(uint8_t, uint8_t *));
uint8_t *i2cm_wait();

#endif

#endif
