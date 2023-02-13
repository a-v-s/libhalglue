#ifndef __BSHAL__GPIO__H
#define __BSHAL__GPIO__H


#include <stdint.h>
#include <stdbool.h>

void bshal_gpio_write_pin(uint8_t bs_pin, bool val);
int bshal_gpio_cfg_out(uint8_t bshal_pin);

#endif
