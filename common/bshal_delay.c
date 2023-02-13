/*
 * Copyright (c) 2021
 * 				André van Schoubroeck <andre@blaatschaap.be>
 *
 * Based upon nrfx_coredep.h from nrfx by Nordic:
 *
 * Copyright (c) 2018 - 2021, Nordic Semiconductor ASA
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "bshal_delay.h"
#include "system.h"

#ifdef __arm__
#include "arm_cpuid.h"
#endif


#if (defined __csky__)
#include <stdint.h>
#include "wm_cpu.h"
uint32_t SystemCoreClock = 80000000;

void SystemCoreClockUpdate(void) {
	wm_sys_clk sys_clk = {0};
	SystemClock_Get(&sys_clk);
	SystemCoreClock = sys_clk.cpuclk * 1000000ul;
}
#endif

// Should probably a ramfuncion
// __attribute__( ( section(".data") ) )
 __attribute__( ( section(".ramfunc") ) )
void delay_cycles_asm(uint32_t time_cycles) {
#if (defined __thumb2__)
/*
//	static uint32_t cycles_per_loop = 10;	//CKS32F103 RAM	// M3
	static uint32_t cycles_per_loop = 8;	//STM32F103	RAM // M3
	asm("loop:" );
	asm("subs  r0, %0" :: "r" (cycles_per_loop) ); 	 
	asm("bhi loop"); 								
*/
	asm("loop:" );
	asm("subs  r0, 6"  ); 	  // cks
//	asm("subs  r0, 4"  ); 	  // stm
	asm("bhi loop"); 								
#elif (defined __thumb__)

/*
//	static uint32_t cycles_per_loop = 12; // F072 HSE 48 FLASH
	static uint32_t cycles_per_loop = 8; // F072 HSE 48 RAM		// M0
//	static uint32_t cycles_per_loop = 7; // L051 HSE 32 RAM 	// M0+
	asm("loop:" );
	asm("sub  r0, %0" :: "l" (cycles_per_loop) );
	asm("bhi loop");
*/

	asm("loop:" );
//	asm("sub  r0, r0, #3"); // L051
	asm("sub  r0, r0, #4"); // F072
	asm("bhi loop");



#elif (defined __riscv)
	uint32_t cycles_per_loop = 3; // TODO
	asm("loop:");
	asm("sub  a0, a0, %0" :: "r" (cycles_per_loop) );
	asm("bgtz a0, loop"); /// might need to be replaced
#elif (defined __csky__)
	uint32_t cycles_per_loop = 6;
	asm(".loop:");
	asm("subu	a0, a0, %0" :: "r" (cycles_per_loop) );
	asm("jbhsz	a0, .loop");
#else 				
#error "CPU Architecture Not Supported"
#endif
}

delay_fn bshal_delay_cycles = delay_cycles_asm;


#if ( __ARM_ARCH >= 7 ) && ( __ARM_ARCH_PROFILE == 'M' )
static void delay_cycles_dwt(uint32_t time_cycles) {
	// Based upon nrfx_coredep.h from nrfx by Nordic:

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
	while ((DWT->CYCCNT - cyccnt_initial) < time_cycles)
		;

	// Restore preserved registers.
	DWT->CTRL = dwt_ctrl;
	CoreDebug->DEMCR = core_debug;
}
#endif

#ifndef __riscv
void bshal_delay_us(uint32_t delay_us) {
	bshal_delay_cycles(delay_us * (SystemCoreClock / 1000000));
}

void bshal_delay_ms(uint32_t delay_ms) {
	bshal_delay_us(delay_ms * 1000);
}
#endif


int bshal_delay_init(void) {
	// Check whether our current core supports DWT
#if ( __ARM_ARCH >= 7 ) && ( __ARM_ARCH_PROFILE == 'M' )

	// ATMv7-M Architecture Reference Manual
	// page 742 Table C1-2
	// ROM Table Entry ROMDWT[0]

	// Page 744 - Table C1-3
	// Points to the Data Watchpoint and Trace unit at 0xE0001000.
	// Bit[0] is set to 1 if a DWT is fitted.
	// Page 798 -
	// NOCYCCNT, bit[25] 0 - Cycle counter supported.

	// TODO: Is there a mask defined for this?
	// Also... can we make use a struct with bitfields instead
	bool use_dwt = (CORTEX_M_ROMTABLE->dwt & 1) && (!(DWT->CTRL & (1 << 25)));

	if (use_dwt)
		bshal_delay_cycles = delay_cycles_dwt;
	else
#endif
		bshal_delay_cycles = delay_cycles_asm;
	return 0;
}

