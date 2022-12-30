
#include <stdbool.h>
#include <stdint.h>

#include <driver/gpio.h>

void bshal_gpio_write_pin(uint8_t bs_pin, bool val) {
	gpio_set_level(bs_pin, val);
}
//
