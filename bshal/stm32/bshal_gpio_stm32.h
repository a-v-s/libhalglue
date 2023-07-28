// bshal_gpio_stm32.h

#ifndef BSHAL_GPIO_STM32
#define BSHAL_GPIO_STM32

#include <stdlib.h>
#include <stdint.h>
#include "hal.h"
#include "bshal_gpio.h"

uint8_t bshal_gpio_encode_pin(GPIO_TypeDef * port, uint16_t pin);
void bshal_gpio_decode_pin(uint8_t bs_pin, GPIO_TypeDef ** port, uint16_t *pin);
void bshal_gpio_write_pin(uint8_t bs_pin, bool val);
void bshal_gpio_port_enable_clock(uint8_t bs_pin);


#endif //BSHAL_GPIO_STM32
