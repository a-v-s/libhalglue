

#ifndef BSHAL_U8X8_SPI
#define BSHAL_U8X8_SPI

#include "u8x8.h"

uint8_t bshal_u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg,
		uint8_t arg_int, void *arg_ptr) ;
uint8_t bshal_u8x8_byte_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr);
#endif // BSHAL_U8X8_SPI
