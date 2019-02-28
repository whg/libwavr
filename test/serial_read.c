#define F_CPU 16000000ul

#include <avr/io.h>
#include <util/delay.h>

#include "pin.h"
#include "serial.h"

int main() {


	uart0.init(9600);

	println_str(&uart0, "Hello world!");
	
	for (;;) {
		char c = read_c(&uart0);
		println_c(&uart0, c);
	}
	
	return 0;
}
