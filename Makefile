# MCU_TARGET=attiny841 # set this on the command line
OPTIMIZE=-Os
INCD=include
CFLAGS=-g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) -std=gnu11 -I$(INCD)
LIBNAME=wavr
OUTD="$(MCU_TARGET)"
OUTPUT="$(OUTD)/lib$(LIBNAME).a"

$(OUTPUT): adc.o serial.o dmx.o i2cs.o i2cm.o
	[ -d $(OUTD) ] || mkdir -p $(OUTD)
	avr-ar rcs $(OUTPUT) $^

serial.o: serial.c $(INCD)/serial.h
	avr-gcc $(CFLAGS) -c -o $@ $<

adc.o: adc.c $(INCD)/adc.h
	avr-gcc $(CFLAGS) -c -o $@ $<

dmx.o: dmx.c $(INCD)/dmx.h
	avr-gcc $(CFLAGS) -c -o $@ $<

i2cs.o: i2cs.c $(INCD)/i2cs.h
	avr-gcc $(CFLAGS) -c -o $@ $<

i2cm.o: i2cm.c $(INCD)/i2cm.h
	avr-gcc $(CFLAGS) -c -o $@ $<


clean:
	rm -rfv *.o *.a
