/*
 * bshal_spim_gecko.c
 *
 *  Created on: 5 mei 2023
 *      Author: andre
 */

#include <bshal_spim.h>

#include <em_device.h>
#include <em_gpio.h>
#include <em_cmu.h>
#include <em_usart.h>

int bshal_spim_config(bshal_spim_instance_t *spim_instance) {

	switch (spim_instance->hw_nr) {
	case 0:
		CMU_ClockEnable(cmuClock_USART0, true);
		spim_instance->drv_specific = USART0;
		break;
	case 1:
		CMU_ClockEnable(cmuClock_USART1, true);
		spim_instance->drv_specific = USART1;
		break;
	default:
		return -1;
	}

	USART_InitSync_TypeDef config = USART_INITSYNC_DEFAULT;
	config.master = true;            // master mode
	config.baudrate = spim_instance->frequency;         // CLK freq is 1 MHz
	config.autoCsEnable = true;   // CS pin controlled by hardware, not firmware
	config.clockMode = usartClockMode0; // clock idle low, sample on rising/first edge
	config.msbf = true;            // send MSB first
	config.enable = usartDisable; // Make sure to keep USART disabled until it's all set up
	USART_InitSync(spim_instance->drv_specific, &config);

	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_Port_TypeDef port;
	uint8_t pin;
	bshal_gpio_decode_pin(spim_instance->sck_pin, &port, &pin);
	GPIO_PinModeSet(port, pin, gpioModePushPull, !spim_instance->cs_pol);

#if GECKO == 1
	  uint32_t route = 0;
#endif

	bshal_gpio_decode_pin(spim_instance->miso_pin, &port, &pin);
	GPIO_PinModeSet(port, pin, gpioModeInput, 0);
#if GECKO == 1
	{
	// Calculated to allow for dynamic allocation
	int routeloc =   bshal_gpio_gecko_routeloc(port, pin);
	if (-1 == routeloc)
		return -1;
	routeloc -= 1;
	routeloc %= 32;
	route |= routeloc << _USART_ROUTELOC0_RXLOC_SHIFT;
	}
#elif GECKO == 2
	  GPIO->USARTROUTE[spim_instance->hw_nr].RXROUTE = (port << _GPIO_USART_RXROUTE_PORT_SHIFT)
	      | (pin << _GPIO_USART_RXROUTE_PIN_SHIFT);
#endif

	bshal_gpio_decode_pin(spim_instance->mosi_pin, &port, &pin);
	GPIO_PinModeSet(port, pin, gpioModePushPull, 0);
#if GECKO == 1
	{
	// Calculated to allow for dynamic allocation
	int routeloc  = bshal_gpio_gecko_routeloc(port, pin);
	if (-1 == routeloc)
		return -1;
	routeloc -= 0;
	routeloc %= 32;
	route |= routeloc << _USART_ROUTELOC0_TXLOC_SHIFT;
	}
#elif GECKO == 2
	  GPIO->USARTROUTE[spim_instance->hw_nr].RXROUTE = (port << _GPIO_USART_TXROUTE_PORT_SHIFT)
	      | (pin << _GPIO_USART_TXROUTE_PIN_SHIFT);
#endif

	bshal_gpio_decode_pin(spim_instance->sck_pin, &port, &pin);
	GPIO_PinModeSet(port, pin, gpioModePushPull, 0);
#if GECKO == 1
	{
	// Calculated to allow for dynamic allocation
	int routeloc =  bshal_gpio_gecko_routeloc(port, pin);
	if (-1 == routeloc)
		return -1;
	routeloc -= 2;
	routeloc %= 32;
	route |= routeloc << _USART_ROUTELOC0_CLKLOC_SHIFT;
	}
#elif GECKO == 2
	  GPIO->USARTROUTE[spim_instance->hw_nr].CLKROUTE = (port << _GPIO_USART_CLKROUTE_PORT_SHIFT)
	      | (pin << _GPIO_USART_CLKROUTE_PIN_SHIFT);
#endif

#if GECKO == 1
	  {
	  // Set USART pin locations
	  USART_TypeDef* u =(USART_TypeDef*)(spim_instance->drv_specific);
	  u->ROUTELOC0 = route;

	  // Enable USART pins
	u->ROUTEPEN = USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN;
	  }
#elif GECKO == 2
	  GPIO->USARTROUTE[spim_instance->hw_nr].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN |    // MISO
	                                GPIO_USART_ROUTEEN_TXPEN |    // MOSI
	                                GPIO_USART_ROUTEEN_CLKPEN;
#endif

	// Enable USART1
	USART_Enable(spim_instance->drv_specific, usartEnable);

}


int bshal_spim_transmit(bshal_spim_instance_t *spim_instance, void *data,
		size_t amount, bool nostop) {
	GPIO_Port_TypeDef port;
	uint8_t pin;

	bshal_gpio_write_pin(spim_instance->cs_pin, spim_instance->cs_pol);
	uint8_t *data8 = data;

	for (int i = 0; i < amount; i++)
		USART_SpiTransfer(spim_instance->drv_specific, data8[i]);

	if (!nostop)
		bshal_gpio_write_pin(spim_instance->cs_pin, !spim_instance->cs_pol);

	return 0;
}

int bshal_spim_receive(bshal_spim_instance_t *spim_instance, void *data,
		size_t amount, bool nostop) {
	GPIO_Port_TypeDef port;
	uint8_t pin;

	bshal_gpio_write_pin(spim_instance->cs_pin, spim_instance->cs_pol);
	uint8_t *data8 = data;

	for (int i = 0; i < amount; i++)
		data8[i] = USART_SpiTransfer(spim_instance->drv_specific, 0xFF);

	if (!nostop)
		bshal_gpio_write_pin(spim_instance->cs_pin, !spim_instance->cs_pol);

	return 0;
}

int bshal_spim_transceive(bshal_spim_instance_t *spim_instance, void *data,
		size_t amount, bool nostop) {
	GPIO_Port_TypeDef port;
	uint8_t pin;

	bshal_gpio_write_pin(spim_instance->cs_pin, spim_instance->cs_pol);
	uint8_t *data8 = data;

	for (int i = 0; i < amount; i++)
		data8[i] = USART_SpiTransfer(spim_instance->drv_specific, data8[i]);

	if (!nostop)
		bshal_gpio_write_pin(spim_instance->cs_pin, !spim_instance->cs_pol);

	return 0;
}
