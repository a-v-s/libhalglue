/*
 * bshal_i2cm_stm32.h
 *
 *  Created on: 11 feb. 2022
 *      Author: andre
 */

#ifndef LIBHALGLUE_BSHAL_STM32_BSHAL_I2CM_STM32_H_
#define LIBHALGLUE_BSHAL_STM32_BSHAL_I2CM_STM32_H_

#include <stdint.h>

#include "hal.h"
#include "bshal_i2cm.h"

int bshal_stm32_i2cm_send(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop);
int bshal_stm32_i2cm_recv(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop);
int bshal_stm32_i2cm_isok(void *drv_specific, uint8_t address) ;



#endif /* LIBHALGLUE_BSHAL_STM32_BSHAL_I2CM_STM32_H_ */
