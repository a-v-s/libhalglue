/*
 * bshal_i2cm_gecko.h
 *
 *  Created on: 5 mei 2023
 *      Author: andre
 */

#ifndef LIBHALGLUE_BSHAL_GECKO_BSHAL_I2CM_GECKO_H_
#define LIBHALGLUE_BSHAL_GECKO_BSHAL_I2CM_GECKO_H_

#include "hal.h"
#include "bshal_i2cm.h"

int bshal_gecko_i2cm_init(bshal_i2cm_instance_t *i2c_instance);
int bshal_gecko_i2cm_send(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop);
int bshal_gecko_i2cm_recv(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop);
int bshal_gecko_i2cm_isok(void *drv_specific, uint8_t address) ;



#endif /* LIBHALGLUE_BSHAL_GECKO_BSHAL_I2CM_GECKO_H_ */
