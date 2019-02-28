#ifndef SERIAL_H
#define SERIAL_H

#ifndef F_CPU
#define F_CPU 16000000ul
#endif

#include <avr/io.h>
#include <stdint.h>
#include <string.h>

typedef void (*uart_init_func)(uint16_t);
typedef char (*uart_read_func)();
typedef void (*uart_write_func)(char);

typedef struct {
	uart_init_func init;
	uart_read_func read;
	uart_write_func write;
} uart_t;

void print_c(uart_t *uart, char c);
void print_str(uart_t *uart, const char *str);
void print_i(uart_t *uart, uint32_t n);
void print_f(uart_t *uart, float f);

void print_endl(uart_t *uart);

void println_c(uart_t *uart, char c);
void println_str(uart_t *uart, const char *str);
void println_i(uart_t *uart, uint32_t n);
void println_f(uart_t *uart, float f);

char read_c(uart_t *uart);

extern uart_t uart0;

/* #if defined(__AVR_ATmega328pb__) || defined(__AVR_ATtiny841__) */
extern uart_t uart1;
/* #endif */

#define pli0(v) println_i(&uart0, (v))
#define plf0(v) println_f(&uart0, (v))
#define plc0(v) println_c(&uart0, (v))
#define plstr0(v) println_str(&uart0, (v))

#define pli1(v) println_i(&uart1, (v))
#define plf1(v) println_f(&uart1, (v))
#define plc1(v) println_c(&uart1, (v))
#define plstr1(v) println_str(&uart1, (v))



#endif
