#define F_CPU 16000000

#include <util/delay.h>

#include "i2cm.h"
#include "pca9685.h"

int main() {

	i2cm_init();

	pca9685_init();   
	
	for (;;) {
		pca9685_set(6, 255);
		pca9685_set(7, 0);
		_delay_ms(1000);
		
		pca9685_set(7, 255);
		pca9685_set(6, 0);
		_delay_ms(500);
		
	}
	
	return 0;
}

