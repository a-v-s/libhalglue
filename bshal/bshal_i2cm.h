/*
 * bshal_i2cm.h
 *
 *  Created on: 15 jan. 2018
 *      Author: André van Schoubroeck
 *
 *      I²C abstraction layer
 */

#ifndef _BSHAL_I2CM__
#define _BSHAL_I2CM__

#include <stdint.h>
#include <stdbool.h>


#define I2C_OK		0
#define I2C_ERROR	-1		// Unspecified Error
#define I2C_NO_INIT	-2		// Not initialised
#define I2C_ANACK 	-10		// Address Nack
#define I2C_DNACK 	-11  	// Data Nack

typedef int (*i2cm_send_f)(void* drv_specific, uint8_t address, void * p_data, uint8_t length, bool nostop);
typedef int (*i2cm_recv_f)(void* drv_specific, uint8_t address, void * p_data, uint8_t length, bool nostop);
typedef int (*i2cm_isok_f)(void* drv_specific, uint8_t address);

typedef struct i2c_instance_t{
	void* drv_specific;
	i2cm_send_f i2cm_send;
	i2cm_recv_f i2cm_recv;
	i2cm_isok_f i2cm_isok;
	uint32_t speed_hz;
	uint8_t sda_pin;
	uint8_t scl_pin;
	uint8_t hw_nr; 	       ///< Hardware number
} bshal_i2cm_instance_t;

int bshal_i2cm_init(bshal_i2cm_instance_t *i2c_instance);
int bshal_i2cm_send(bshal_i2cm_instance_t* i2c_instance, uint8_t address, void * p_data, uint8_t length, bool nostop);
int bshal_i2cm_recv(bshal_i2cm_instance_t* i2c_instance, uint8_t address, void * p_data, uint8_t length, bool nostop);
int bshal_i2cm_isok(bshal_i2cm_instance_t* i2c_instance, uint8_t address);
int bshal_i2cm_send_reg(bshal_i2cm_instance_t* i2c_instance, uint8_t address, uint8_t reg, uint8_t * p_data, uint8_t length);
int bshal_i2cm_recv_reg(bshal_i2cm_instance_t* i2c_instance, uint8_t address, uint8_t reg, uint8_t * p_data, uint8_t length);

#endif
