# MCU_TARGET=attiny841
MCU_TARGET=atmega328pb
OPTIMIZE=-Os
CFLAGS=-g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) -std=gnu11 -I../include
LDFLAGS=-Wl,-Map,$(PRG).map -L../$(MCU_TARGET) -lwavr
# LDFLAGS=-Wl,-Map,$(PRG).map

$(PRG).hex: $(PRG).c
	avr-gcc $(CFLAGS) -o $(PRG).elf $^ $(LDFLAGS)
	avr-objcopy -O ihex $(PRG).elf $(PRG).hex

asm: $(PRG).c
	avr-gcc $(CFLAGS) $^ $(LDFLAGS) -S


clean:
	rm -rfv *.o *.hex *.map *.elf

install: $(PRG).hex
	avrdude -c usbtiny -p $(MCU_TARGET) -U flash:w:$(PRG).hex

flash:
	avrdude -C/etc/avrdude.conf -p $(MCU_TARGET) -carduino -P/dev/ttyUSB0 -b115200 -D -Uflash:w:$(PRG).hex

flash2:
	avrdude -C/etc/avrdude.conf -p atmega328p -carduino -P/dev/ttyUSB1 -b57600 -D -Uflash:w:display.hex

ard:
	avrdude -C/etc/avrdude.conf -p $(MCU_TARGET) -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:$(PRG).hex


