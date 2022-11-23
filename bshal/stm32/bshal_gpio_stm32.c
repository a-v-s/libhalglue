

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
	return -1;
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

void bshal_gpio_port_enable_clock(uint8_t bs_pin) {

	GPIO_TypeDef *port;
	uint16_t pin;
	bshal_gpio_decode_pin(bs_pin, &port, &pin);
	switch ( (int)(port)) {
#ifdef GPIOA
	case (int)GPIOA:
			__HAL_RCC_GPIOA_CLK_ENABLE();
			break;
#endif
#ifdef GPIOB
	case (int)GPIOB:
			__HAL_RCC_GPIOB_CLK_ENABLE();
			break;
#endif
#ifdef GPIOC
	case (int)GPIOC:
			__HAL_RCC_GPIOC_CLK_ENABLE();
			break;
#endif
#ifdef GPIOD
	case (int)GPIOD:
			__HAL_RCC_GPIOD_CLK_ENABLE();
			break;
#endif
#ifdef GPIOE
	case (int)GPIOE:
			__HAL_RCC_GPIOE_CLK_ENABLE();
			break;
#endif
#ifdef GPIOF
	case (int)GPIOF:
			__HAL_RCC_GPIOF_CLK_ENABLE();
			break;
#endif
#ifdef GPIOG
	case (int)GPIOG:
			__HAL_RCC_GPIOG_CLK_ENABLE();
			break;
#endif
#ifdef GPIOH
	case (int)GPIOH:
			__HAL_RCC_GPIOH_CLK_ENABLE();
			break;
#endif
#ifdef GPIOI
	case (int)GPIOI:
			__HAL_RCC_GPIOI_CLK_ENABLE();
			break;
#endif
#ifdef GPIOJ
	case (int)GPIOJ:
			__HAL_RCC_GPIOJ_CLK_ENABLE();
			break;
#endif
#ifdef GPIOK
	case (int)GPIOK:
			__HAL_RCC_GPIOK_CLK_ENABLE();
			break;
#endif
#ifdef GPIOL
	case (int)GPIOL:
			__HAL_RCC_GPIOL_CLK_ENABLE();
			break;
#endif
#ifdef GPIOM
	case (int)GPIOM:
			__HAL_RCC_GPIOM_CLK_ENABLE();
			break;
#endif
#ifdef GPION
	case (int)GPION:
			__HAL_RCC_GPION_CLK_ENABLE();
			break;
#endif
#ifdef GPIOO
	case (int)GPIOO:
			__HAL_RCC_GPIOO_CLK_ENABLE();
			break;

#endif
	}
}

int bshal_gpio_cfg_out(uint8_t bshal_pin){
	if (bshal_pin == 0xFF) return -1;


	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_TypeDef *stm32_port = NULL;
	uint16_t stm32_pin = -1;
	bshal_gpio_decode_pin(bshal_pin, &stm32_port, &stm32_pin);
	bshal_gpio_port_enable_clock(bshal_pin);
	if (stm32_pin) {
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pin = stm32_pin;
		HAL_GPIO_Init(stm32_port, &GPIO_InitStruct);
		return 0;
	}
	return -1;
}

int bshal_gpio_cfg_int(uint8_t pin){}
