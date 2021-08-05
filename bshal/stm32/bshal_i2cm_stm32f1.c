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

static I2C_HandleTypeDef m_i2c_handles[2];

static bool m_state = false; // Well... this has to be different

int bshal_stm32_i2cm_send(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop) {

// Intended to write an implementation supporting a nostop flag
// To disable renerating a stop condition (intended for repeated start)
// However this ain't working yet
//
//	int flags = 0;
//	if (m_state) {
//		if (nostop)
//			flags = I2C_OTHER_FRAME;
//		else
//			flags = I2C_OTHER_AND_LAST_FRAME;
//	} else {
//		if (nostop)
//			flags = I2C_FIRST_AND_NEXT_FRAME;
//		else
//			flags = I2C_FIRST_AND_LAST_FRAME;
//	}
//
//	m_state = nostop;
//
//	return HAL_I2C_Master_Seq_Transmit_IT(drv_specific, address << 1, p_data,
//			length, flags);
//

	return HAL_I2C_Master_Transmit(drv_specific, address << 1, p_data, length,
			1000);
}
int bshal_stm32_i2cm_recv(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop) {

// Intended to write an implementation supporting a nostop flag
// To disable renerating a stop condition (intended for repeated start)
// However this ain't working yet
//
//	int flags = 0;
//	if (m_state) {
//		if (nostop)
//			flags = I2C_NEXT_FRAME;
//		else
//			flags = I2C_LAST_FRAME;
//	} else {
//		if (nostop)
//			flags = I2C_FIRST_AND_NEXT_FRAME;
//		else
//			flags = I2C_FIRST_AND_LAST_FRAME;
//	}
//
//	m_state = nostop;
//
//	return HAL_I2C_Master_Seq_Receive_IT(drv_specific, address << 1, p_data, length,
//			flags);

	return HAL_I2C_Master_Receive(drv_specific, address << 1, p_data, length,
			1000);

}
int bshal_stm32_i2cm_isok(void *drv_specific, uint8_t address) {
	return HAL_I2C_IsDeviceReady(drv_specific, address << 1, 1, 1000);
}

int bshal_stm32_i2cm_init(bshal_i2cm_instance_t *i2c_instance) {
	i2c_instance->i2cm_isok = bshal_stm32_i2cm_isok;
	i2c_instance->i2cm_recv = bshal_stm32_i2cm_recv;
	i2c_instance->i2cm_send = bshal_stm32_i2cm_send;

	GPIO_InitTypeDef GPIO_InitStruct;
	// Common configuration for all channels
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_TypeDef *port = NULL;
	uint16_t pin = -1;

	bshal_gpio_port_enable_clock(i2c_instance->scl_pin);
	bshal_gpio_decode_pin(i2c_instance->scl_pin, &port, &pin);
	GPIO_InitStruct.Pin = pin;

	HAL_GPIO_Init(port, &GPIO_InitStruct);

	bshal_gpio_port_enable_clock(i2c_instance->sda_pin);
	bshal_gpio_decode_pin(i2c_instance->sda_pin, &port, &pin);
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

	handle->Init.ClockSpeed = 400000;
	handle->Init.DutyCycle = I2C_DUTYCYCLE_2;
	handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;

	i2c_instance->drv_specific = handle;
	HAL_I2C_Init(handle);

}
