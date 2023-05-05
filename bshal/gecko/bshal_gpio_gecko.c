/*
 * bshal_gpio_gecko.c
 *
 *  Created on: 5 mei 2023
 *      Author: andre
 */

#include "bshal_gpio_gecko.h"

uint8_t bshal_gpio_encode_pin(GPIO_Port_TypeDef port, uint8_t pin) {
	return (port << 4) | (pin & 0xF);
}

void bshal_gpio_decode_pin(uint8_t bs_pin, GPIO_Port_TypeDef *port,
		uint8_t *pin) {
	*port = bs_pin >> 4;
	*pin = bs_pin & 0xF;
}

int bshal_gpio_gecko_routeloc(GPIO_Port_TypeDef port, uint8_t pin) {
	switch (port) {
	case gpioPortA:
		if (pin > 5) return -1;
		return pin;
	case gpioPortB:
		if (pin < 11) return -1;
		if (pin > 15) return -1;
		return 6 + pin - 11;
	case gpioPortC:
		if (pin < 6 ) return -1;
		if (pin > 11) return -1;
		return 11 + pin - 6;
	case gpioPortD:
		if ( pin < 9) return -1;
		if (pin > 15) return -1;
		return 17 + pin -9;
	case gpioPortF:
		if (pin > 7) return -1;
		return 24 + pin;
	default:
		return -1;
	}
}

void bshal_gpio_write_pin(uint8_t bs_pin, bool val) {
	GPIO_Port_TypeDef port = -1;
	uint16_t pin = -1;
	bshal_gpio_decode_pin(bs_pin, &port, &pin);
	if (val)
		GPIO_PinOutSet(port, pin);
	else
		GPIO_PinOutClear(port, pin);
}

void bshal_gpio_port_enable_clock(uint8_t bs_pin) {
	(void) bs_pin;
	CMU_ClockEnable(cmuClock_GPIO, true);
}

int bshal_gpio_cfg_out(uint8_t bshal_pin) {
	if (bshal_pin == 0xFF)
		return -1;
	GPIO_Port_TypeDef efr32_port = -1;
	uint16_t efr32_pin = -1;
	bshal_gpio_decode_pin(bshal_pin, &efr32_port, &efr32_pin);
	GPIO_PinModeSet(efr32_port, efr32_pin, gpioModePushPull, 0);
	return 0;

}

