/*
 * bshal_i2cm_gecko.c
 *
 *  Created on: 5 mei 2023
 *      Author: andre
 */

#include "bshal_i2cm_gecko.h"
#include "bshal_gpio_gecko.h"

#include <em_i2c.h>
#include <em_cmu.h>

int bshal_gecko_i2cm_init(bshal_i2cm_instance_t *i2c_instance) {

	switch (i2c_instance->hw_nr) {
	case 0:
		CMU_ClockEnable(cmuClock_I2C0, true);
		i2c_instance->drv_specific = I2C0;
		break;
//		 case 1:
//			 CMU_ClockEnable(cmuClock_I2C1, true);
//			 i2c_instance->drv_specific = I2C1;
//			 break;
	default:
		return -1;
	}

	// Using default settings
	I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

	i2cInit.freq = i2c_instance->speed_hz;
	if (i2c_instance->speed_hz > 100000)
		i2cInit.clhr = i2cClockHLRAsymetric;
	else
		i2cInit.clhr = i2cClockHLRStandard;

	GPIO_Port_TypeDef port;
	uint8_t pin;

	CMU_ClockEnable(cmuClock_GPIO, true);
	bshal_gpio_decode_pin(i2c_instance->scl_pin, &port, &pin);
	GPIO_PinModeSet(port, pin, gpioModeWiredAndPullUpFilter, 1);
#if GECKO == 1
	int sda_route_loc, scl_route_loc;
	// Calculated to allow for dynamic allocation
	scl_route_loc = bshal_gpio_gecko_routeloc(port, pin);
	if (-1 == scl_route_loc)
		return -1;
	scl_route_loc = ((scl_route_loc - 1) % 32) << 8;
	I2C0->ROUTELOC0 = (I2C0->ROUTELOC0 & (~_I2C_ROUTELOC0_SCLLOC_MASK))
			| scl_route_loc;
#elif GECKO == 2
	  GPIO->I2CROUTE[0].SCLROUTE = (GPIO->I2CROUTE[0].SCLROUTE & ~_GPIO_I2C_SCLROUTE_MASK)
	                        | (port << _GPIO_I2C_SCLROUTE_PORT_SHIFT
	                        | (pin << _GPIO_I2C_SCLROUTE_PIN_SHIFT));
#endif

	bshal_gpio_decode_pin(i2c_instance->sda_pin, &port, &pin);
	GPIO_PinModeSet(port, pin, gpioModeWiredAndPullUpFilter, 1);
#if GECKO == 1
	// Calculated to allow for dynamic allocation
	sda_route_loc = bshal_gpio_gecko_routeloc(port, pin);
	if (-1 == sda_route_loc)
		return -1;
	I2C0->ROUTELOC0 = (I2C0->ROUTELOC0 & (~_I2C_ROUTELOC0_SDALOC_MASK))
			| sda_route_loc;
#elif GECKO == 2
	  GPIO->I2CROUTE[0].SDAROUTE = (GPIO->I2CROUTE[0].SDAROUTE & ~_GPIO_I2C_SDAROUTE_MASK)
	                        | (port << _GPIO_I2C_SDAROUTE_PORT_SHIFT
	                        | (pin << _GPIO_I2C_SDAROUTE_PIN_SHIFT));
#endif


#if GECKO == 1
	// Calculated to allow for dynamic allocation
	I2C0->ROUTEPEN = I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN;
#elif GECKO == 2
	GPIO->I2CROUTE[0].ROUTEEN = GPIO_I2C_ROUTEEN_SDAPEN | GPIO_I2C_ROUTEEN_SCLPEN;
#endif
	// Initializing the I2C
	I2C_Init(i2c_instance->drv_specific, &i2cInit);
	i2c_instance->i2cm_isok = bshal_gecko_i2cm_isok;
	i2c_instance->i2cm_send = bshal_gecko_i2cm_send;
	i2c_instance->i2cm_recv = bshal_gecko_i2cm_recv;
	return 0;

}

int bshal_gecko_i2cm_send(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop) {
	(void)nostop;

	I2C_TransferSeq_TypeDef i2cTransfer;
	I2C_TransferReturn_TypeDef result;

	// Initializing I2C transfer
	i2cTransfer.addr = address << 1; // 8 nit notation??
	i2cTransfer.flags = I2C_FLAG_WRITE;
	i2cTransfer.buf[0].data = p_data;
	i2cTransfer.buf[0].len = length;
	i2cTransfer.buf[1].data = NULL;
	i2cTransfer.buf[1].len = 0;
	result = I2C_TransferInit(drv_specific, &i2cTransfer);

	// Sending data
	while (result == i2cTransferInProgress) {
		result = I2C_Transfer(drv_specific);
	}

	return result;
}
int bshal_gecko_i2cm_recv(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop) {
	(void)nostop;

	I2C_TransferSeq_TypeDef i2cTransfer;
	I2C_TransferReturn_TypeDef result;

	// Initializing I2C transfer
	i2cTransfer.addr = address << 1; // 8 nit notation??
	i2cTransfer.flags = I2C_FLAG_READ;
	i2cTransfer.buf[0].data = p_data;
	i2cTransfer.buf[0].len = length;
	i2cTransfer.buf[1].data = NULL;
	i2cTransfer.buf[1].len = 0;
	result = I2C_TransferInit(drv_specific, &i2cTransfer);

	// Sending data
	while (result == i2cTransferInProgress) {
		result = I2C_Transfer(drv_specific);
	}

	return result;

}
int bshal_gecko_i2cm_isok(void *drv_specific, uint8_t address) {
	uint8_t c;
	// Not yet sure yet if we can address without a transfer,
	// for now, let's raed a byte instead
	return bshal_gecko_i2cm_recv(drv_specific, address, &c, 1, false);
}

