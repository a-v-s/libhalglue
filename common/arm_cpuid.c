/*
 * arm_cpuid.c
 *
 *  Created on: 6 feb. 2020
 *      Author: andre
 */

#include <stdint.h>


#include "stm32f103xb.h"

#include "arm_cpuid.h"

#include "core_cm3.h"

#include <inttypes.h>

char* cpuid(){
	static char buff[64] = "Unknown";
	// ARM v7m manual,page 702, section B4.1.2.
  // 0xE000ED00 // CPUID
	SCB->CPUID;
	cpuid_t *cpuid =(cpuid_t *)(&SCB->CPUID);
	//
	if ((cpuid->PartNo & (0b11<<10)) == (0b11<<10)) {
		// Cortex family
		if ((cpuid->PartNo & (0b10 << 4)) == (0b10 << 4)) {
			// Cortex M
			sprintf(buff,"Cortex-M%d r%dp%d"  , cpuid->PartNo & 0xF ,cpuid->Variant, cpuid->Revision);
		}
	}
	return buff;
	// But the interesting part comes at 0xE000ED40 // ID_PFR0 (Processor Feature Register) and following
	// We should be able to access it through
	SCB->PFR[0];
	SCB->PFR[1];
	/*
  __IM  uint32_t PFR[2U];         		 //< Offset: 0x040 (R/ )  Processor Feature Register
  __IM  uint32_t DFR;                    //< Offset: 0x048 (R/ )  Debug Feature Register
  __IM  uint32_t ADR;                    //< Offset: 0x04C (R/ )  Auxiliary Feature Register
  __IM  uint32_t MMFR[4U];               //< Offset: 0x050 (R/ )  Memory Model Feature Register
  __IM  uint32_t ISAR[5U];               //< Offset: 0x060 (R/ )  Instruction Set Attributes Register
	 */


}
