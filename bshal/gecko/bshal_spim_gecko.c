/*
 * bshal_spim_gecko.c
 *
 *  Created on: 5 mei 2023
 *      Author: andre
 */

#include <bshal_spim.h>
#include <bshal_delay.h>
#include <bshal_gpio.h>

#include <em_device.h>
#include <em_usart.h>


int bshal_spim_transmit(bshal_spim_instance_t *spim_instance, const void *data,
		size_t amount, bool nostop) {

	bshal_gpio_write_pin(spim_instance->cs_pin, spim_instance->cs_pol);
//	bshal_delay_ms(1);

	uint8_t *data8 = data;

	for (int i = 0; i < amount; i++)
		USART_SpiTransfer(spim_instance->drv_specific, data8[i]);

	if (!nostop)
		bshal_gpio_write_pin(spim_instance->cs_pin, !spim_instance->cs_pol);
//	bshal_delay_ms(1);
	return 0;
}

int bshal_spim_receive(bshal_spim_instance_t *spim_instance, void *data,
		size_t amount, bool nostop) {

	bshal_gpio_write_pin(spim_instance->cs_pin, spim_instance->cs_pol);
//	bshal_delay_ms(1);

	uint8_t *data8 = data;

	for (int i = 0; i < amount; i++)
		data8[i] = USART_SpiTransfer(spim_instance->drv_specific, 0xFF);

	if (!nostop)
		bshal_gpio_write_pin(spim_instance->cs_pin, !spim_instance->cs_pol);
//	bshal_delay_ms(1);
	return 0;
}

int bshal_spim_transceive(bshal_spim_instance_t *spim_instance, void *data,
		size_t amount, bool nostop) {


	bshal_gpio_write_pin(spim_instance->cs_pin, spim_instance->cs_pol);
//	bshal_delay_ms(1);

	uint8_t *data8 = data;

	for (int i = 0; i < amount; i++)
		data8[i] = USART_SpiTransfer(spim_instance->drv_specific, data8[i]);

	if (!nostop)
		bshal_gpio_write_pin(spim_instance->cs_pin, !spim_instance->cs_pol);
//	bshal_delay_ms(1);
	return 0;
}
