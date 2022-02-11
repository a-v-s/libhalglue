/*
 * bshal_i2cm_stm32.c
 *
 *  Created on: 11 feb. 2022
 *      Author: andre
 */


#include "bshal_i2cm_stm32.h"

int bshal_stm32_i2cm_send(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop) {
	return HAL_I2C_Master_Transmit(drv_specific, address << 1, p_data, length,
			1000);
}

int bshal_stm32_i2cm_recv(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop) {
	return HAL_I2C_Master_Receive(drv_specific, address << 1, p_data, length,
			1000);
}

int bshal_stm32_i2cm_isok(void *drv_specific, uint8_t address) {
	return HAL_I2C_IsDeviceReady(drv_specific, address << 1, 1, 1000);
}
