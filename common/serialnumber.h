/*
 * serialnumber.h
 *
 *  Created on: 1 feb. 2020
 *      Author: andre
 */

#ifndef LIBHALGLUE_COMMON_INC_SERIALNUMBER_H_
#define LIBHALGLUE_COMMON_INC_SERIALNUMBER_H_

#include <stdint.h>
#include <stdlib.h>

void FormatSerialStringASCII(uint8_t* in_data, size_t in_size, uint8_t* out_str, size_t out_size);
void FormatSerialStringUTF16(uint8_t* in_data, size_t in_size, uint16_t* out_str, size_t out_size);
void GetHardwareSerial(uint8_t** ptr, size_t* size);

void GetSerialStringASCII(uint8_t* out_str, size_t out_size);
void GetSerialStringUTF16(uint16_t* out_str, size_t out_size);


#endif /* LIBHALGLUE_COMMON_INC_SERIALNUMBER_H_ */
