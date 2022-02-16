/*
 * bshal_i2cm_stm32.c
 *
 *  Created on: 5 aug. 2021
 *      Author: andre
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "hal.h"
#include "bshal_i2cm.h"
#include "bshal_gpio_stm32.h"
#include "bshal_i2cm_stm32.h"

static I2C_HandleTypeDef m_i2c_handles[2];


int bshal_stm32_i2cm_init(bshal_i2cm_instance_t *i2c_instance) {
	i2c_instance->i2cm_isok = bshal_stm32_i2cm_isok;
	i2c_instance->i2cm_recv = bshal_stm32_i2cm_recv;
	i2c_instance->i2cm_send = bshal_stm32_i2cm_send;




	GPIO_InitTypeDef GPIO_InitStruct;
	// Common configuration for all channels
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	//GPIO_InitStruct.Alternate = 0x01; // Differs on L0


	GPIO_TypeDef *port = NULL;
	uint16_t pin = -1;

	bshal_gpio_port_enable_clock(i2c_instance->scl_pin);
	bshal_gpio_decode_pin(i2c_instance->scl_pin, &port, &pin);

	if (port == GPIOB && pin == GPIO_PIN_6 && i2c_instance->hw_nr==1) GPIO_InitStruct.Alternate = 0x01;
	else if (port == GPIOB && pin == GPIO_PIN_8 && i2c_instance->hw_nr==1) GPIO_InitStruct.Alternate = 0x04;
	else if (port == GPIOB && pin == GPIO_PIN_10 && i2c_instance->hw_nr==2) GPIO_InitStruct.Alternate = 0x06;
	else if (port == GPIOB && pin == GPIO_PIN_13 && i2c_instance->hw_nr==2) GPIO_InitStruct.Alternate = 0x05;
	else return -1;

	GPIO_InitStruct.Pin = pin;
	HAL_GPIO_Init(port, &GPIO_InitStruct);

	bshal_gpio_port_enable_clock(i2c_instance->sda_pin);
	bshal_gpio_decode_pin(i2c_instance->sda_pin, &port, &pin);

	if (port == GPIOB && pin == GPIO_PIN_7 && i2c_instance->hw_nr==1) GPIO_InitStruct.Alternate = 0x01;
		else if (port == GPIOB && pin == GPIO_PIN_9 && i2c_instance->hw_nr==1) GPIO_InitStruct.Alternate = 0x04;
		else if (port == GPIOB && pin == GPIO_PIN_11 && i2c_instance->hw_nr==2) GPIO_InitStruct.Alternate = 0x06;
		else if (port == GPIOB && pin == GPIO_PIN_14 && i2c_instance->hw_nr==2) GPIO_InitStruct.Alternate = 0x05;
		else return -1;

	GPIO_InitStruct.Pin = pin;
	HAL_GPIO_Init(port, &GPIO_InitStruct);




	I2C_HandleTypeDef *handle;
	switch (i2c_instance->hw_nr) {
#ifdef I2C1
	case 1:
		__HAL_RCC_I2C1_CLK_ENABLE();
		handle = m_i2c_handles+0;
		memset(handle, 0, sizeof(I2C_HandleTypeDef));
		handle->Instance = I2C1;
		break;
#endif
#ifdef I2C2
	case 2:
		__HAL_RCC_I2C2_CLK_ENABLE();
		handle = m_i2c_handles+1;
		memset(handle, 0, sizeof(I2C_HandleTypeDef));
		handle->Instance = I2C2;
		break;
#endif

	default:
		return -1;
	}


	// The L0 HAL does not appear to offer a clock speed/duty cycle setting
	// In stead it has a Timing setting, Copying the value from the example
	// TODO: How to calculate this value given a speed?
	// Do other Cubes which take the speed calculate it, can we copy the code?
	//handle->Init.ClockSpeed = 400000;
	//handle->Init.DutyCycle = I2C_DUTYCYCLE_16_9;

	/* I2C TIMING Register define when I2C clock source is SYSCLK */
	/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 32 MHz */
	//#define I2C_TIMING    0x10A13E56 /* 100 kHz with analog Filter ON, Rise Time 400ns, Fall Time 100ns */
    //#define I2C_TIMING    0x00B1112E /* 400 kHz with analog Filter ON, Rise Time 250ns, Fall Time 100ns */
	handle->Init.Timing = 0x00B1112E;

	handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;

	i2c_instance->drv_specific = handle;
	HAL_I2C_Init(handle);
    HAL_I2CEx_ConfigAnalogFilter(handle,I2C_ANALOGFILTER_ENABLE);

}
