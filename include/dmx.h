#ifndef DMX_H
#define DMX_H

#include <stdint.h>

#define DMX_MAX_CHANNELS 3

void dmx_init();
void dmx_set_channel(uint8_t n, uint16_t channel);
uint8_t dmx_get_value(uint8_t n);

void dmx_set_received_callback(void (*cb)());

#endif
