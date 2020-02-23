/*
	File:		serialnumber.h
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
