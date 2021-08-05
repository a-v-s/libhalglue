/*
 * delay.c
 *
 *  Created on: 5 aug. 2021
 *      Author: andre
 */

#include "bshal_delay.h"

#include "system.h"

typedef void (*delay_fn)(uint32_t);

void delay_cycles_asm(uint32_t time_cycles) {
   static uint32_t cycles_per_loop = 3;
   asm("loop:" );
   asm("subs  r0, %0 " :: "r" (cycles_per_loop) );   // 1 cycle
   asm("bhi loop");								 // 2 cycles
}

//delay_fn bshal_delay_ms = none;
//delay_fn bshal_delay_us = none;
delay_fn bshal_delay_cycles = delay_cycles_asm;


static void delay_cycles_dwt(uint32_t time_cycles) {
    // Save the current state of the DEMCR register to be able to restore it before exiting
    // this function. Enable the trace and debug blocks (including DWT).
    uint32_t core_debug = CoreDebug->DEMCR;
    CoreDebug->DEMCR = core_debug | CoreDebug_DEMCR_TRCENA_Msk;

    // Save the current state of the CTRL register in the DWT block. Make sure
    // that the cycle counter is enabled.
    uint32_t dwt_ctrl = DWT->CTRL;
    DWT->CTRL = dwt_ctrl | DWT_CTRL_CYCCNTENA_Msk;

    // Store start value of the cycle counter.
    uint32_t cyccnt_initial = DWT->CYCCNT;

    // Delay required time.
    while ((DWT->CYCCNT - cyccnt_initial) < time_cycles);

    // Restore preserved registers.
    DWT->CTRL = dwt_ctrl;
    CoreDebug->DEMCR = core_debug;
}





void bshal_delay_us(uint32_t delay_us) {
	bshal_delay_cycles(delay_us *  (SystemCoreClock / 1000000) );
}



void bshal_delay_ms(uint32_t delay_ms) {
	bshal_delay_us(delay_ms * 1000);
}


int bshal_delay_init(void) {
	// Check whether our current core supports DWT
	bool use_dwt = false;

	if (use_dwt)
		bshal_delay_cycles = delay_cycles_dwt;
	else
		bshal_delay_cycles = delay_cycles_asm;
}


