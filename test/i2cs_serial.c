#include "i2cs.h"

#include <avr/interrupt.h>
#include "serial.h"

void test_func(uint32_t v) {
	uint16_t b = v;
	println_i(b);
}

int main(void)
{
	DDRA |= (1<<PA7);

	serial_init(57600);
	i2cs_init(8);
	i2cs_register_receive(&test_func);
  
	sei();
	while(1) {

	}
}

// on the Arduino:
// 
// #include <Wire.h>
// 
// void setup() {
//   Wire.begin();
// }
// 
// void loop() {
//   Wire.beginTransmission(8);
//   uint16_t v = 9876;
//   Wire.write(v & 0xff);
//   Wire.write((v>>8) & 0xff);
//   Wire.endTransmission();
// 
//   delay(1000);
// }
