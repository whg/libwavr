#if defined(__AVR_ATtiny841__)

#ifndef I2CS_H
#define I2CS_H

#include <stdint.h>

typedef void (*i2c_receive_func)(uint32_t);

void i2cs_init(uint8_t address);
void i2cs_register_receive(i2c_receive_func func);
void i2cs_write(uint32_t v);

#endif

#endif
