/*
 * serialnumber.c
 *
 *  Created on: 1 feb. 2020
 *      Author: andre
 */


// I'm spending more time trying to find the old code then it would take to re-implement it


#include <string.h>
#include "serialnumber.h"


void FormatSerialHexASCII(uint8_t* in_data, size_t in_size, uint8_t* out_str, size_t out_size) {
	uint8_t tmp_out[out_size];
	memset(tmp_out,0,sizeof(tmp_out));
	for (int i = 0 ; i < in_size; i++) {
		tmp_out[i%out_size] += in_data[i%in_size];
	}
	for (int i = 0 ; i < out_size; i++) {
		uint8_t  tmp = tmp_out[i]%16;
		if (tmp < 10) {
			out_str[i] = '0' + i;
		} else {
			out_str[i] = 'A' + i - 10;
		}
	}
}


void FormatSerialHexUTF16(uint8_t* in_data, size_t in_size, uint16_t* out_str, size_t out_size) {
	uint8_t tmp_out[out_size];
	memset(tmp_out,0,sizeof(tmp_out));
	for (int i = 0 ; i < in_size; i++) {
		// Add or Xor them, what looks better?
		//tmp_out[i%out_size] += in_data[i%in_size];
		tmp_out[i%out_size] ^= in_data[i%in_size];
	}
	for (int i = 0 ; i < out_size; i++) {
		uint8_t  tmp = tmp_out[i]%16;
		if (tmp < 10) {
			out_str[i] = '0' + tmp;
		} else {
			out_str[i] = 'A' + tmp - 10;
		}
	}
}

void FormatSerialStringASCII(uint8_t* in_data, size_t in_size, uint8_t* out_str, size_t out_size) {
	uint8_t tmp_out[out_size];
	memset(tmp_out,0,sizeof(tmp_out));
	for (int i = 0 ; i < in_size; i++) {
		tmp_out[i%out_size] += in_data[i%in_size];
	}
	for (int i = 0 ; i < out_size; i++) {
		uint8_t  tmp = tmp_out[i]%36;
		if (tmp < 10) {
			out_str[i] = '0' + i;
		} else {
			out_str[i] = 'A' + i - 10;
		}
	}
}

void FormatSerialStringUTF16(uint8_t* in_data, size_t in_size, uint16_t* out_str, size_t out_size) {
	uint8_t tmp_out[out_size];
	memset(tmp_out,0,sizeof(tmp_out));
	for (int i = 0 ; i < in_size; i++) {
		// Add or Xor them, what looks better?
		//tmp_out[i%out_size] += in_data[i%in_size];
		tmp_out[i%out_size] ^= in_data[i%in_size];
	}
	for (int i = 0 ; i < out_size; i++) {
		uint8_t  tmp = tmp_out[i]%36;
		if (tmp < 10) {
			out_str[i] = '0' + tmp;
		} else {
			out_str[i] = 'A' + tmp - 10;
		}
	}
}

void GetHardwareSerial(uint8_t** ptr, size_t* size) {
	// We need a hardware family header thing
#if defined STM32F103xB
	*ptr= (uint8_t*)0x1FFFF7E8;
	*size = 12;
#elif defined STM32L151xC
	*ptr= (uint8_t*) 0x1FF800D0;
	*size = 12;
#elif defined NRF52840_XXAA
	*ptr= (uint8_t*) 0x10000060;
	*size = 8;
#else
	// Not supported
	*ptr= NULL;
	*size = 0;
#endif

}

// Convenience functions
void GetSerialStringASCII(uint8_t* out_str, size_t out_size){
	uint8_t* hw_serial_prt; size_t hw_serial_size;
	GetHardwareSerial(&hw_serial_prt,&hw_serial_size);
	FormatSerialStringASCII(hw_serial_prt,hw_serial_size,out_str,out_size);
}
void GetSerialStringUTF16(uint16_t* out_str, size_t out_size){
	uint8_t* hw_serial_prt; size_t hw_serial_size;
	GetHardwareSerial(&hw_serial_prt,&hw_serial_size);
	FormatSerialStringUTF16(hw_serial_prt,hw_serial_size,out_str,out_size);
}


void GetSerialHexASCII(uint8_t* out_str, size_t out_size){
	uint8_t* hw_serial_prt; size_t hw_serial_size;
	GetHardwareSerial(&hw_serial_prt,&hw_serial_size);
	FormatSerialHexASCII(hw_serial_prt,hw_serial_size,out_str,out_size);
}
void GetSerialHexUTF16(uint16_t* out_str, size_t out_size){
	uint8_t* hw_serial_prt; size_t hw_serial_size;
	GetHardwareSerial(&hw_serial_prt,&hw_serial_size);
	FormatSerialHexUTF16(hw_serial_prt,hw_serial_size,out_str,out_size);
}
