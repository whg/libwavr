#include "serial.h"

#include <math.h>

void uart0_init(uint16_t baud) {
    UBRR0L = F_CPU / (16ul * baud) - 1;
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); // 8 bit
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
}

char uart0_read_c() {
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

void uart0_print_c(char c) {
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}

#if defined(__AVR_ATmega328PB__) || defined(__AVR_ATtiny841__)
void uart1_init(uint16_t baud) {
    UBRR1L = F_CPU / (16ul * baud) - 1;
    UCSR1C = (1<<UCSZ11) | (1<<UCSZ10); // 8 bit
    UCSR1B = (1<<RXEN1) | (1<<TXEN1);
}

char uart1_read_c() {
    loop_until_bit_is_set(UCSR1A, RXC1);
    return UDR1;
}

void uart1_print_c(char c) {
	loop_until_bit_is_set(UCSR1A, UDRE1);
	UDR1 = c;
}
#endif

uart_t uart0 = { &uart0_init, &uart0_read_c, &uart0_print_c };

#if defined(__AVR_ATmega328PB__) || defined(__AVR_ATtiny841__)
uart_t uart1 = { &uart1_init, &uart1_read_c, &uart1_print_c };
#endif

void print_c(uart_t *uart, char c) {
	(*uart->write)(c);
}

void print_buf(uart_t *uart, const char *buffer, size_t n) {
	while (n--) {
		print_c(uart, *buffer++);
	}
}

void print_str(uart_t *uart, const char *str) {
	print_buf(uart, str, strlen(str));
}
		
void print_i(uart_t *uart, uint32_t n) {
	char buf[11]; // 2**32 == 10 digits
	char *str = &buf[10];
	*str = '\0';
	do {
		*--str = (n % 10) + '0';
		n /= 10;
	} while(n);

	print_str(uart, str);
}

void print_f(uart_t *uart, float f) {
	if (isnan(f)) {
		print_str(uart, "nan");
	} else if (isinf(f)) {
		print_str(uart, "inf");
	} else {
		if (f < 0) {
			print_c(uart, '-');
			f = -f;
		}
		float rounding = 0.5f;
		uint8_t digits = 3;
		for (uint8_t i = 0; i < digits; i++) {
			rounding /= 10.f;
		}
		f += rounding;

		uint32_t integer = (uint32_t)(f);
		print_i(uart, integer);
		
		float remainder = f - (float)(integer);
		print_c(uart, '.');
		while (digits-- > 0) {
			remainder *= 10;
			uint32_t digit = (uint32_t)(remainder);
			print_i(uart, digit);
			remainder -= digit;
		}
	}
}

void print_endl(uart_t *uart) {
	print_c(uart, '\r');
	print_c(uart, '\n');
}

void println_c(uart_t *uart, char c) {
	print_c(uart, c);
	print_endl(uart);
}

void println_str(uart_t *uart, const char *str) {
	print_str(uart, str);
	print_endl(uart);
}

void println_f(uart_t *uart, float f) {
	print_f(uart, f);
	print_endl(uart);
}

void println_i(uart_t *uart, uint32_t i) {
	print_i(uart, i);
	print_endl(uart);
}

char read_c(uart_t *uart) {
	return uart->read();
}
