/*
 * ucglib_hal.h
 *
 *  Created on: 10 apr. 2020
 *      Author: andre
 */

#ifndef UCGLIB_HAL_H_
#define UCGLIB_HAL_H_

#include <stdint.h>

#include "bshal_u8x8.h"

typedef bshal_u8x8_spi_t bshal_ucg_spi_t;
typedef bshal_u8x8_t bshal_ucg_t;

#include "ucg.h"

int16_t ucg_com_bshal(ucg_t *ucg, int16_t msg, uint16_t arg, uint8_t *data);

#endif /* UCGLIB_HAL_H_ */
