#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "pin.h"
#include "i2cs.h"

pin_t led_pin;
uint8_t command;

void i2cs_receive(uint32_t v) {
	command = v & 0xff;
	if (command) {
		pin_set(led_pin);
	} else {
		pin_clear(led_pin);
	}
}

int main() {

	i2cs_init(8);
	i2cs_register_receive(&i2cs_receive);
	
	sei();

	pin_init(led_pin, A, 7, OUTPUT);
	command = 0;
	
	for (;;) {
		// only will be sent when requested for...
		i2cs_write(command);
		_delay_ms(200);
		pin_clear(led_pin);
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
