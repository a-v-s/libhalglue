// bshal_gpio_stm32.h

#ifndef BSHAL_GPIO_STM32
#define BSHAL_GPIO_STM32

#include <stdlib.h>
#include <stdint.h>
#include "hal.h"

uint8_t bshal_gpio_encode_pin(GPIO_TypeDef * port, uint16_t pin);
void bshal_gpio_decode_pin(uint8_t bs_pin, GPIO_TypeDef ** port, uint16_t *pin);


#endif //BSHAL_GPIO_STM32
