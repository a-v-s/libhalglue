/*
 * bshal_u8x8.h
 *
 *  Created on: 4 sep. 2021
 *      Author: andre
 */

#ifndef LIBHALGLUE_BSHAL_EXTLIB_BSHAL_U8X8_H_
#define LIBHALGLUE_BSHAL_EXTLIB_BSHAL_U8X8_H_

#include "system.h"
//#include "u8g2.h"
//#include "u8x8.h"
//#include "u8x8_spi.h"
#include "bshal_spim.h"


typedef struct {
	bshal_spim_instance_t	instance;		///< SPI MASTER INSTANCE
	uint8_t			ncd_pin;		///< D/CX pin number (optional).

	// Should we have reset and chip select here as well
	// Or keep those at SPI level?
} bshal_u8x8_spi_t;


typedef struct {
	uint8_t transport; // TODO: We have transports defined before, find and unift
	union {
		bshal_u8x8_spi_t spim;
	};
} bshal_u8x8_t;


#endif /* LIBHALGLUE_BSHAL_EXTLIB_BSHAL_U8X8_H_ */
