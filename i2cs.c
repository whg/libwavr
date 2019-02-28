#if defined(__AVR_ATtiny841__)

#include "i2cs.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

volatile uint8_t bit_counter;
volatile uint32_t in_buffer, out_buffer;
volatile uint8_t doing_master_read;
i2c_receive_func receive_func = NULL;

void i2cs_init(uint8_t address) {
	TWSA = address << 1;
	TWSCRA = (1<<TWEN) | (1<<TWSHE) | (1<<TWASIE) | (1<<TWSIE) | (1<<TWDIE);

	bit_counter = 0;
	in_buffer = out_buffer = 0;

	sei();
}

void i2cs_register_receive(i2c_receive_func func) {
	receive_func = func;
}

void i2cs_write(uint32_t v) {
	out_buffer = 0; // do we need to set all bits to 0?
	out_buffer = v;
}


ISR(TWI_SLAVE_vect) {
	uint8_t status = TWSSRA;
	uint8_t ack_and_wait = 0;
	
	if (status & (1<<TWDIF)) { // data
		doing_master_read = status & (1<<TWDIR);
		if (doing_master_read) { // master read
			TWSD = (out_buffer >> bit_counter) & 0xff;
		}
		else { // master write
			uint32_t b = TWSD;
			in_buffer |= (b << bit_counter);
		}
		bit_counter+= 8;
		ack_and_wait = 1;
	}
	else if (status & (1<<TWASIF)) { // address or stop
		if (TWSSRA & (1<<TWAS)) { // address
			ack_and_wait = 1;
		}
		else { // stop
			if (!doing_master_read && receive_func) {
				(*receive_func)(in_buffer);
			}
			in_buffer = 0;
			bit_counter = 0;
			TWSCRB = (1<<TWCMD1);
		}
	}

	if (ack_and_wait) {
		// TWSCRB bits need to be written at the same time
		TWSCRB = (1<<TWCMD0) | (1<<TWCMD1);
	}

}


#endif
