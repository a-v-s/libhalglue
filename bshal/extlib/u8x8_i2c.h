#include "system.h"
#include "u8g2.h"
#include "u8x8.h"
#include "bshal_i2cm.h"


uint8_t bshal_u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg,
		uint8_t arg_int, void *arg_ptr) ;
uint8_t bshal_u8x8_byte_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr);
