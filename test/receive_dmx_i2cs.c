#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "i2cs.h"
#include "dmx.h"

void i2cs_receive(uint32_t v) {
	uint8_t command = v & 0xff;
	if (command == 1) {
		// set dmx address
		uint16_t addr = v >> 8;
		dmx_set_channel(0, addr);
	}
}

int main() {

	i2cs_init(8);
	i2cs_register_receive(&i2cs_receive);
	
	dmx_init();
	
	sei();
	
	uint8_t value = 0;
	for (;;) {
		uint8_t v = dmx_get_value(0);
		if (v != value) {
			i2cs_write(v);
			value = v;
		}
		_delay_ms(10);
	}
	
	return 0;
}

// // on the other end:
//
// #include <Wire.h>
// 
// uint16_t channel = 1;
// 
// void setup() {
//   Wire.begin();
//   Serial.begin(9600);
// }
// 
// void loop() {
//   Wire.beginTransmission(8);
//   Wire.write(1);
//   Wire.write((channel + 1) & 0xff);
//   Wire.write(((channel + 1) >> 8) & 0xff);
//   Serial.print("set channel to ");
//   Serial.println(channel + 1);
//   channel = (channel + 1) % 6;
// 
//   Wire.endTransmission();
// 
//   delay(100);
// 
//   for (int i = 0; i < 10; i++) {
//     Wire.requestFrom(8, 1);
//     while (Wire.available()) {
//       Serial.println(int(Wire.read()));
//     }
//     delay(400);
//   }
// }
