/*
 * bshal_gpio_gecko.h
 *
 *  Created on: 5 mei 2023
 *      Author: andre
 */

#ifndef LIBHALGLUE_BSHAL_GECKO_BSHAL_GPIO_GECKO_H_
#define LIBHALGLUE_BSHAL_GECKO_BSHAL_GPIO_GECKO_H_

#include <stdlib.h>

#include "em_device.h"
#include "em_gpio.h"
#include "em_cmu.h"

uint8_t bshal_gpio_encode_pin(GPIO_Port_TypeDef port, uint8_t pin) ;
void bshal_gpio_decode_pin(uint8_t bs_pin, GPIO_Port_TypeDef *port, uint8_t *pin);
void bshal_gpio_write_pin(uint8_t bs_pin, bool val);
void bshal_gpio_port_enable_clock(uint8_t bs_pin);
int bshal_gpio_cfg_out(uint8_t bshal_pin);
int bshal_gpio_gecko_routeloc(GPIO_Port_TypeDef port, uint8_t pin);


#endif /* LIBHALGLUE_BSHAL_GECKO_BSHAL_GPIO_GECKO_H_ */
