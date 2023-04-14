/*
 * bshal_i2cm_stm32.c
 *
 *  Created on: 11 feb. 2022
 *      Author: andre
 */

#include "bshal_i2cm_stm32.h"

int bshal_stm32_i2cm_send(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop) {
	int result = HAL_I2C_Master_Transmit(drv_specific, address << 1, p_data,
			length, 100);
	if (result) {
		I2C_HandleTypeDef *hi2c = drv_specific;
		if (hi2c->Instance->SR2 & I2C_SR2_BUSY) {
			// On the MH32 the BUSY bit is stuck when a NACK is received
			// We need to Software Reset the I²C peripheral
			hi2c->Instance->CR1 |= I2C_CR1_SWRST;
			hi2c->Instance->CR1 &= ~I2C_CR1_SWRST;
			HAL_I2C_Init(drv_specific);
		}
	}
	return result;
}

int bshal_stm32_i2cm_recv(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop) {
	int result = HAL_I2C_Master_Receive(drv_specific, address << 1, p_data,
			length, 100);
	if (result) {
		I2C_HandleTypeDef *hi2c = drv_specific;
		if (hi2c->Instance->SR2 & I2C_SR2_BUSY) {
			// On the MH32 the BUSY bit is stuck when a NACK is received
			// We need to Software Reset the I²C peripheral
			hi2c->Instance->CR1 |= I2C_CR1_SWRST;
			hi2c->Instance->CR1 &= ~I2C_CR1_SWRST;
			HAL_I2C_Init(drv_specific);
		}
	}

	return result;
}

int bshal_stm32_i2cm_isok(void *drv_specific, uint8_t address) {
	int result =  HAL_I2C_IsDeviceReady(drv_specific, address << 1, 1, 1000);
	if (result) {
		I2C_HandleTypeDef *hi2c = drv_specific;
		if (hi2c->Instance->SR2 & I2C_SR2_BUSY) {
			// On the MH32 the BUSY bit is stuck when a NACK is received
			// We need to Software Reset the I²C peripheral
			hi2c->Instance->CR1 |= I2C_CR1_SWRST;
			hi2c->Instance->CR1 &= ~I2C_CR1_SWRST;
			HAL_I2C_Init(drv_specific);
		}
	}

	return result;
}
