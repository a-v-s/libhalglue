/*
 * delay.h
 *
 *  Created on: 5 aug. 2021
 *      Author: andre
 */

#ifndef LIBHALGLUE_COMMON_BSHAL_DELAY_H_
#define LIBHALGLUE_COMMON_BSHAL_DELAY_H_

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

typedef void (*delay_fn)(uint32_t);

//extern delay_fn bshal_delay_ms;
//extern delay_fn bshal_delay_us;
//extern delay_fn bshal_delay_cycles;

void bshal_delay_ms(uint32_t);
void bshal_delay_us(uint32_t);
int bshal_delay_init(void);

#endif /* LIBHALGLUE_COMMON_BSHAL_DELAY_H_ */
