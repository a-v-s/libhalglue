/*
 File:		arm_cpuid.c
 License: 	MIT

 Copyright (c) 2020 André van Schoubroeck

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifdef __ARM_EABI__
#include <stdint.h>
#include <stdio.h>

#include "system.h"
#include "arm_cpuid.h"
#include <inttypes.h>

char* cpuid() {
	static char buff[64] = "Unknown";
	// ARM v7m manual,page 702, section B4.1.2.
	// 0xE000ED00 // CPUID
	SCB->CPUID;


	cpuid_t *cpuid = (cpuid_t*) (&SCB->CPUID);

	// Cortex M3 r1p1 TRM page 162
	// To find the document on arm.com, sarch "ARM DDI 0337E"
	// However, with newer cores like m23, m33, m55, m85
	// This no longer holds... I guess we'll need a lookup table
	// with these new namings
	//
	// 0xC20	m0
	// 0xC60	m0+
	// 0xC21	m1
	// 0xc23	m3
	// 0xC24	m4
	// 0xC27   	m7
	// 0xD20   	m23
	// 0xD21	m33
	// 0xD22	m55
	// 0xD23	m85


	if ((cpuid->PartNo & (0b11 << 10)) == (0b11 << 10)) {
		// Cortex family
		if ((cpuid->PartNo & (0b10 << 4)) == (0b10 << 4)) {
			// Cortex M

			// This works for Cortex M3 and M4
			// But has to be updated for M33 (which reads as M1) 
			sprintf(buff, "Cortex-M%d r%dp%d", cpuid->PartNo & 0xF,
					cpuid->Variant, cpuid->Revision);
		}
	}
	return buff;
	// But the interesting part comes at 0xE000ED40 // ID_PFR0 (Processor Feature Register) and following
	// We should be able to access it through
	//SCB->PFR[0];
	//SCB->PFR[1];
	/*
	 __IM  uint32_t PFR[2U];         		 //< Offset: 0x040 (R/ )  Processor Feature Register
	 __IM  uint32_t DFR;                    //< Offset: 0x048 (R/ )  Debug Feature Register
	 __IM  uint32_t ADR;                    //< Offset: 0x04C (R/ )  Auxiliary Feature Register
	 __IM  uint32_t MMFR[4U];               //< Offset: 0x050 (R/ )  Memory Model Feature Register
	 __IM  uint32_t ISAR[5U];               //< Offset: 0x060 (R/ )  Instruction Set Attributes Register
	 */

}

romtable_pid_t extract_romtable_pid(romtable_id_t *rid) {
	return (romtable_pid_t ) { .pid = rid->pid0 << 0 | rid->pid1 << 8
					| rid->pid2 << 16 | rid->pid3 << 24
					| (uint64_t) rid->pid4 << 32 | (uint64_t) rid->pid5 << 40
					| (uint64_t) rid->pid6 << 48 | (uint64_t) rid->pid7 << 56 } ;
		}

#endif
