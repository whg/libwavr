#if defined(__AVR_ATmega328PB__)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <string.h>

#include "i2cm.h"

#if !defined(F_CPU)
#define F_CPU 16000000
#endif

static volatile uint8_t busy;
static struct {
	uint8_t buffer[I2CM_BUFFER_LENGTH];
	uint8_t length;
	uint8_t index;
	void (*callback)(uint8_t, uint8_t *);
} transmission;

void i2cm_init() {
	TWBR0 = ((F_CPU / I2CM_FREQ) - 16) / 2;
	TWSR0 = 0; // prescaler = 1

	busy = 0;

	sei();

	TWCR0 = _BV(TWEN);
}

uint8_t *i2cm_wait() {
	while (busy);
	return &transmission.buffer[1];
}

inline void i2cm_start(void) {
	TWCR0 = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) | _BV(TWSTA);
}

inline void i2cm_stop(void) {
	TWCR0 = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
}

inline void i2cm_ack() {
	TWCR0 = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}

inline void i2cm_nack() {
	TWCR0 = _BV(TWINT) | _BV(TWEN) | _BV(TWIE);
}

inline void i2cm_send(uint8_t data) {
	TWDR0 = data;
}

void i2cm_recv() {
	transmission.buffer[transmission.index++] = TWDR0;
}

void i2cm_reply() {
	if (transmission.index < (transmission.length - 1)) {
		i2cm_ack();
	} else {
		i2cm_nack();
	}
}

void i2cm_done() {
	uint8_t address = transmission.buffer[0] >> 1;
	uint8_t *data = &transmission.buffer[1];

	busy = 0;

	if (transmission.callback != NULL) {
		transmission.callback(address, data);
	}
}

void i2cm_write(uint8_t address, uint8_t length, void (*callback)(uint8_t, uint8_t *)) {
	i2cm_wait();

	busy = 1;

	transmission.buffer[0] = (address << 1) | TW_WRITE;
	transmission.length = length + 1;
	transmission.index = 0;
	transmission.callback = callback;
	//	memcpy(&transmission.buffer[1], data, length);

	i2cm_start();
}

void i2cm_read(uint8_t address, uint8_t length, void (*callback)(uint8_t, uint8_t *)) {
	i2cm_wait();

	busy = 1;

	transmission.buffer[0] = (address << 1) | TW_READ;
	transmission.length = length + 1;
	transmission.index = 0;
	transmission.callback = callback;

	i2cm_start();
}

ISR(TWI0_vect) {
	switch (TWSR0 & (_BV(TWS7)|_BV(TWS6)|_BV(TWS5)|_BV(TWS4)|_BV(TWS3))) {
	case TW_START:
	case TW_REP_START:
	case TW_MT_SLA_ACK:
	case TW_MT_DATA_ACK:
		if (transmission.index < transmission.length) {
			i2cm_send(transmission.buffer[transmission.index++]);
			i2cm_nack();
		} else {
			i2cm_stop();
			i2cm_done();
		}
		break;

	case TW_MR_DATA_ACK:
		i2cm_recv();
		i2cm_reply();
		break;

	case TW_MR_SLA_ACK:
		i2cm_reply();
		break;

	case TW_MR_DATA_NACK:
		i2cm_recv();
		i2cm_stop();
		i2cm_done();
		break;

	case TW_MT_SLA_NACK:
	case TW_MR_SLA_NACK:
	case TW_MT_DATA_NACK:
	default:
		i2cm_stop();
		i2cm_done();
		break;
	}
}

#endif
