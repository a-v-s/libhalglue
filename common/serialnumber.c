/*
	File:		serialnumber.c
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

// I'm spending more time trying to find the old code then it would take to re-implement it
#include <string.h>
#include "serialnumber.h"
#include "hal.h"

void FormatSerialHexASCII(uint8_t *in_data, size_t in_size, uint8_t *out_str,
		size_t out_size) {
	uint8_t tmp_out[out_size];
	memset(tmp_out, 0, sizeof(tmp_out));
	for (size_t i = 0; i < in_size; i++) {
		tmp_out[i % out_size] += in_data[i % in_size];
	}
	for (size_t i = 0; i < out_size; i++) {
		uint8_t tmp = tmp_out[i] % 16;
		if (tmp < 10) {
			out_str[i] = '0' + i;
		} else {
			out_str[i] = 'A' + i - 10;
		}
	}
}

void FormatSerialHexUTF16(uint8_t *in_data, size_t in_size, uint16_t *out_str,
		size_t out_size) {
	uint8_t tmp_out[out_size];
	memset(tmp_out, 0, sizeof(tmp_out));
	for (size_t i = 0; i < in_size; i++) {
		// Add or Xor them, what looks better?
		//tmp_out[i%out_size] += in_data[i%in_size];
		tmp_out[i % out_size] ^= in_data[i % in_size];
	}
	for (size_t i = 0; i < out_size; i++) {
		uint8_t tmp = tmp_out[i] % 16;
		if (tmp < 10) {
			out_str[i] = '0' + tmp;
		} else {
			out_str[i] = 'A' + tmp - 10;
		}
	}
}

void FormatSerialStringASCII(uint8_t *in_data, size_t in_size, uint8_t *out_str,
		size_t out_size) {
	uint8_t tmp_out[out_size];
	memset(tmp_out, 0, sizeof(tmp_out));
	for (size_t i  = 0; i < in_size; i++) {
		tmp_out[i % out_size] += in_data[i % in_size];
	}
	for (size_t i  = 0; i < out_size; i++) {
		uint8_t tmp = tmp_out[i] % 36;
		if (tmp < 10) {
			out_str[i] = '0' + i;
		} else {
			out_str[i] = 'A' + i - 10;
		}
	}
}

void FormatSerialStringUTF16(uint8_t *in_data, size_t in_size,
		uint16_t *out_str, size_t out_size) {
	uint8_t tmp_out[out_size];
	memset(tmp_out, 0, sizeof(tmp_out));
	for (size_t i  = 0; i < in_size; i++) {
		// Add or Xor them, what looks better?
		//tmp_out[i%out_size] += in_data[i%in_size];
		tmp_out[i % out_size] ^= in_data[i % in_size];
	}
	for (size_t i  = 0; i < out_size; i++) {
		uint8_t tmp = tmp_out[i] % 36;
		if (tmp < 10) {
			out_str[i] = '0' + tmp;
		} else {
			out_str[i] = 'A' + tmp - 10;
		}
	}
}

void GetHardwareSerial(uint8_t **ptr, size_t *size) {
	// We need a hardware family header thing
	// FOr now, we have
	// http://blog.gorski.pm/stm32-unique-id
	// Please note the STM32 HAL defines this addess UID_BASE
	// So we can use the HAL instead. Generate a geneic STM32 case
	
	// Also our HAL-Abstraction Layer can provide the same for NRF5x and
	// If it also defined UID_LEN we can make it work for either

	// Note there are MCUs that don't expose their serial number
	// as a pointer in memory space.
	// If we consider non-Cortex-M targets, Atmel's AVR does not

/*
#if defined STM32F072xB
	*ptr= (uint8_t*)0x1FFFF7AC;
	*size = 12;
#elif defined STM32F1
	*ptr= (uint8_t*)0x1FFFF7E8;
	*size = 12;
#elif defined STM32L151xC
	*ptr= (uint8_t*) 0x1FF800D0;
	*size = 12;
#elif defined STM32F3
	*ptr= (uint8_t*) 0x1FFFF7AC;
	*size = 12;
#elif defined NRF52_SERIES
	*ptr= (uint8_t*) 0x10000060;
	*size = 8;
*/
	// On some targets, eg STM32L0, the serial number appears to be 
	// non-contiguous. So we'll have to look at some other implementation.
	// using LL_GetUID_Word{0,1,2}. If so, we'll need to allocate some RAM here.
	// Damnit... I wanted to avoid that. 
	
#if defined(UID_BASE) && defined(UID_LEN)
	*ptr= (uint8_t*) UID_BASE;
	*size = UID_LEN;
#else
	// Not supported
	*ptr = NULL;
	*size = 0;
#endif

}

// Convenience functions
void GetSerialStringASCII(uint8_t *out_str, size_t out_size) {
	uint8_t *hw_serial_prt;
	size_t hw_serial_size;
	GetHardwareSerial(&hw_serial_prt, &hw_serial_size);
	FormatSerialStringASCII(hw_serial_prt, hw_serial_size, out_str, out_size);
}
void GetSerialStringUTF16(uint16_t *out_str, size_t out_size) {
	uint8_t *hw_serial_prt;
	size_t hw_serial_size;
	GetHardwareSerial(&hw_serial_prt, &hw_serial_size);
	FormatSerialStringUTF16(hw_serial_prt, hw_serial_size, out_str, out_size);
}

void GetSerialHexASCII(uint8_t *out_str, size_t out_size) {
	uint8_t *hw_serial_prt;
	size_t hw_serial_size;
	GetHardwareSerial(&hw_serial_prt, &hw_serial_size);
	FormatSerialHexASCII(hw_serial_prt, hw_serial_size, out_str, out_size);
}
void GetSerialHexUTF16(uint16_t *out_str, size_t out_size) {
	uint8_t *hw_serial_prt;
	size_t hw_serial_size;
	GetHardwareSerial(&hw_serial_prt, &hw_serial_size);
	FormatSerialHexUTF16(hw_serial_prt, hw_serial_size, out_str, out_size);
}
