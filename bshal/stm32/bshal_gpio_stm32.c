

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "bshal_gpio_stm32.h"

uint8_t bshal_gpio_encode_pin(GPIO_TypeDef *port, uint16_t pin) {
	uint8_t result = 0;
	for (int i = 0; i < 16; i++) {
		if ((1 << i) & pin) {
			if (result)
				return -1;
			result = i;
		}
	}
#ifdef GPIOA
	if (port == GPIOA)
	return result | ( 0 << 4);
#endif	
#ifdef GPIOB
	if (port == GPIOB)
	return result | ( 1 << 4);
#endif	
#ifdef GPIOC
	if (port == GPIOC)
	return result | ( 2 << 4);
#endif	
#ifdef GPIOD
	if (port == GPIOD)
	return result | ( 3 << 4);
#endif	
#ifdef GPIOE
	if (port == GPIOE)
	return result | ( 4 << 4);
#endif	
#ifdef GPIOF
	if (port == GPIOF)
	return result | ( 5 << 4);
#endif	
#ifdef GPIOG
	if (port == GPIOG)
	return result | ( 6 << 4);
#endif	
#ifdef GPIOH
	if (port == GPIOH)
	return result | ( 7 << 4);
#endif	
#ifdef GPIOI
	if (port == GPIOI)
	return result | ( 8 << 4);
#endif	
#ifdef GPIOJ
	if (port == GPIOJ)
	return result | ( 9 << 4);
#endif	
#ifdef GPIOK
	if (port == GPIOK)
	return result | ( 10 << 4);
#endif	
#ifdef GPIOL
	if (port == GPIOL)
	return result | ( 11 << 4);
#endif	
#ifdef GPIOM
	if (port == GPIOM)
	return result | ( 12 << 4);
#endif	
#ifdef GPION
	if (port == GPION)
	return result | ( 13 << 4);
#endif	
#ifdef GPIOO
	if (port == GPIOO)
	return result | ( 14 << 4);
#endif
}

void bshal_gpio_decode_pin(uint8_t bs_pin, GPIO_TypeDef **port, uint16_t *pin) {
	*pin = 1 << (bs_pin & 0xF);
	*port = NULL;
#ifdef GPIOA
	if ( (bs_pin & 0xF0)  == ( 0 << 4) )
		*port = GPIOA;
#endif	
#ifdef GPIOB
	if ( (bs_pin & 0xF0)  == ( 1 << 4) )
		*port = GPIOB;
#endif	
#ifdef GPIOC
	if ( (bs_pin & 0xF0)  == ( 2 << 4) )
		*port = GPIOC;
#endif	
#ifdef GPIOD
	if ( (bs_pin & 0xF0)  == ( 3 << 4) )
		*port = GPIOD;
#endif	
#ifdef GPIOE
	if ( (bs_pin & 0xF0)  == ( 4 << 4) )
		*port = GPIOE;
#endif	
#ifdef GPIOF
	if ( (bs_pin & 0xF0)  == ( 5 << 4) )
		*port = GPIOF;
#endif	
#ifdef GPIOG
	if ( (bs_pin & 0xF0)  == ( 6 << 4) )
		*port = GPIOG;
#endif	
#ifdef GPIOH
	if ( (bs_pin & 0xF0)  == ( 7 << 4) )
		*port = GPIOH;
#endif	
#ifdef GPIOI
	if ( (bs_pin & 0xF0)  == ( 8 << 4) )
		*port = GPIOI;
#endif	
#ifdef GPIOJ
	if ( (bs_pin & 0xF0)  == ( 9 << 4) )
		*port = GPIOJ;
#endif	
#ifdef GPIOK
	if ( (bs_pin & 0xF0)  == ( 10 << 4) )
		*port = GPIOK;
#endif	
#ifdef GPIOL
	if ( (bs_pin & 0xF0)  == ( 11 << 4) )
		*port = GPIOL;
#endif	
#ifdef GPIOM
	if ( (bs_pin & 0xF0)  == ( 12 << 4) )
		*port = GPIOM;
#endif	
#ifdef GPION
	if ( (bs_pin & 0xF0)  == ( 13 << 4) )
		*port = GPION;
#endif	
#ifdef GPIOO
	if ( (bs_pin & 0xF0)  == ( 14 << 4) )
		*port = GPIOO;
#endif	
}

void bshal_gpio_write_pin(uint8_t bs_pin, bool val) {
	GPIO_TypeDef *port = NULL;
	uint16_t pin = -1;
	bshal_gpio_decode_pin(bs_pin,&port,&pin);
	if (port)
		HAL_GPIO_WritePin(port, pin, val);
}
