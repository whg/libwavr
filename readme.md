# libwavr

**libwavr** is a thin abstraction layer for the AVR, with the goal to make developing on the AVR a bit nicer.

### Build

To build to the library for a given chip pass the `avr-gcc` chip name to the Makefile:

`$ make MCU_TARGET=atmega328pb`

This will create a directory for the chip and put `libwavr.a` in it.
