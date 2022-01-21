/*
 * bshal.h
 *
 *  Created on: 21 jan. 2022
 *      Author: andre
 */

#ifndef LIBHALGLUE_BSHAL_BSHAL_H_
#define LIBHALGLUE_BSHAL_BSHAL_H_

#include "bshal_i2cm.h"
#include "bshal_spim.h"

// We have all transport functions as weak empty functions, such that we can ommit a unused transport implementation
// While still using the transport abstraction
// Note this should probably be moved to a .c file instead as this would generate multiple weak implementations
// Even through they get thrown away at link time, so it wouldn't pose a problem, but it's less efficient,
// and ugly

int __attribute__((weak)) bshal_spim_transmit(bshal_spim_instance_t * bshal_spim, void *data, size_t amount, bool nostop)
 {return -1;}
int __attribute__((weak))  bshal_spim_recveive(bshal_spim_instance_t * bshal_spim, void *data, size_t amount, bool nostop)
{return -1;}
int __attribute__((weak)) bshal_spim_transceive(bshal_spim_instance_t * bshal_spim, void *data, size_t amount, bool nostop)
 {return -1;}

int __attribute__((weak))  bshal_i2cm_send(bshal_i2cm_instance_t* i2c_instance, uint8_t address, void * p_data, uint8_t length, bool nostop)
{return -1;}
int __attribute__((weak))  bshal_i2cm_recv(bshal_i2cm_instance_t* i2c_instance, uint8_t address, void * p_data, uint8_t length, bool nostop)
{return -1;}
int __attribute__((weak))  bshal_i2cm_isok(bshal_i2cm_instance_t* i2c_instance, uint8_t address)
{return -1;}
int __attribute__((weak)) bshal_i2cm_send_reg(bshal_i2cm_instance_t* i2c_instance, uint8_t address, uint8_t reg, uint8_t * p_data, uint8_t length)
 {return -1;}
int __attribute__((weak))  bshal_i2cm_recv_reg(bshal_i2cm_instance_t* i2c_instance, uint8_t address, uint8_t reg, uint8_t * p_data, uint8_t length)
{return -1;}



typedef enum {
    bshal_transport_undefined,
	bshal_transport_spi,
	bshal_transport_i2c,
	bshal_transport_uart,
	bshal_transport_8080,
	bshal_transport_6800,
} bshal_transport_type_t;

typedef union {
	void * raw;
	bshal_i2cm_instance_t * i2cm;
	bshal_spim_instance_t * spim;
} bshal_transport_instance_t;



#endif /* LIBHALGLUE_BSHAL_BSHAL_H_ */
