#ifndef __CORE_CM3_COMPAR_H_
#define __CORE_CM3_COMPAR_H_

#include <stdint.h>

//extern uint32_t SystemCoreClock;
//extern const uint8_t AHBPrescTable[16U];
//extern const uint8_t APBPrescTable[8U];


#define IRQn_Type           int
#define __ECLIC_PRESENT 	1
#define __ECLIC_BASEADDR 	0xd2000000
#include "core_ch32.h"
#include "core_feature_eclic.h"
#include "core_compatiable.h"


#ifndef read_csr
#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })
#endif

#define GD32_MARCHID 0x80000022
#define GD32_IRQ_OFFSET	19
#define CH32_IRQ_OFFSET 16


#define NVIC_EnableIRQ              __Compat_EnableIRQ
#define NVIC_DisableIRQ             __Compat_DisableIRQ
#define NVIC_GetPendingIRQ          __Compat_GetPendingIRQ
#define NVIC_SetPendingIRQ          __Compat_SetPendingIRQ
#define NVIC_ClearPendingIRQ        __Compat_ClearPendingIRQ
#define NVIC_SetPriority            __Compat_SetPriority

// Functions that have no obvious equivalent for any or either PFIC or ECLIC
// This will require some manual work to enable them
//#define NVIC_SetPriorityGrouping    __Compat_SetPriorityGrouping 	// -PFIC -ECLIC
//#define NVIC_GetPriorityGrouping    __Compat_GetPriorityGrouping	// -PFIC -ECLIC
//#define NVIC_GetEnableIRQ           __Compat_GetEnableIRQ 		// -PFIC +ECLIC
//#define NVIC_GetActive              __Compat_GetActive			// +PFIC -ECLIC
//#define NVIC_GetPriority            __Compat_GetPriority			// -PFIC +ECLIC
//#define NVIC_SystemReset            __Compat_SystemReset			// +PFIC -ECLIC

static inline void __Compat_EnableIRQ(int IRQn){
	if (read_csr(marchid)==GD32_MARCHID) {
		IRQn += GD32_IRQ_OFFSET;
		ECLIC_EnableIRQ(IRQn);
	} else {
		IRQn += CH32_IRQ_OFFSET;
		PFIC_EnableIRQ(IRQn);
	}
}

static inline void __Compat_DisableIRQ(int IRQn){
	if (read_csr(marchid)==GD32_MARCHID) {
		IRQn += GD32_IRQ_OFFSET;
		ECLIC_DisableIRQ(IRQn);
	} else {
		IRQn += CH32_IRQ_OFFSET;
		PFIC_DisableIRQ(IRQn);
	}
}

static inline uint32_t __Compat_GetPendingIRQ(int IRQn){
	if (read_csr(marchid)==GD32_MARCHID) {
		IRQn += GD32_IRQ_OFFSET;
		return ECLIC_GetPendingIRQ(IRQn);
	} else {
		IRQn += CH32_IRQ_OFFSET;
		return PFIC_GetPendingIRQ(IRQn);
	}
}
static inline void __Compat_SetPendingIRQ(int IRQn){
	if (read_csr(marchid)==GD32_MARCHID) {
		IRQn += GD32_IRQ_OFFSET;
		ECLIC_SetPendingIRQ(IRQn);
	} else {
		IRQn += CH32_IRQ_OFFSET;
		PFIC_SetPendingIRQ(IRQn);
	}
}

static inline void  __Compat_ClearPendingIRQ(int IRQn){
	if (read_csr(marchid)==GD32_MARCHID) {
		IRQn += GD32_IRQ_OFFSET;
		ECLIC_ClearPendingIRQ(IRQn);
	} else {
		IRQn += CH32_IRQ_OFFSET;
		PFIC_ClearPendingIRQ(IRQn);
	}
}

static inline void __Compat_SetPriority(int IRQn, uint8_t priority){
	// NB. TODO: Verify the priority parameter is compatible
	if (read_csr(marchid)==GD32_MARCHID) {
		IRQn += GD32_IRQ_OFFSET;
		ECLIC_SetPriorityIRQ(IRQn, priority);
	} else {
		IRQn += CH32_IRQ_OFFSET;
		PFIC_SetPriority(IRQn, priority);
	}
}


static inline void NVIC_SetFastIRQ(uint32_t addr, int IRQn, uint8_t num){}
static inline void NVIC_SystemReset(void){}

//static inline uint32_t NVIC_GetActive(int IRQn){}
//static inline uint32_t NVIC_GetStatusIRQ(int IRQn){}

/*
static inline void __enable_irq() {
	asm("csrrs zero, mstatus, 0x08");
}

static inline void __disable_irq() {
	asm("csrrc zero, mstatus, 0x08");
}
*/

#endif // __INTERRUPTS_H_


