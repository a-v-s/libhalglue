#ifndef __BSHAL__GPIO__H
#define __BSHAL__GPIO__H


#include <stdint.h>
#include <stdbool.h>

typedef enum {
	pushpull, opendrain
} gpio_drive_type_t;

bool bshal_gpio_read_pin(uint8_t bs_pin);
void bshal_gpio_write_pin(uint8_t bs_pin, bool val);
int bshal_gpio_cfg_out(uint8_t bshal_pin, gpio_drive_type_t drive_type, bool init_val);

#endif
