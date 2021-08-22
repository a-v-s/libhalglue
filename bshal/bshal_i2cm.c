/*
	License:	MIT
	File:		bshal_i2c.c

    Copyright (c) 2018, 2019, 2021
								André van Schoubroeck <andre@blaatschaap.be>

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

#include "bshal_i2cm.h"

#include <stdint.h>
#include <string.h>

int bshal_i2cm_send(bshal_i2cm_instance_t *i2c_instance, uint8_t address,
		void *p_data, uint8_t length, bool nostop) {
	if (!i2c_instance)
		return -1;
	return i2c_instance->i2cm_send ?
			i2c_instance->i2cm_send(i2c_instance->drv_specific, address, p_data,
					length, nostop) :
			I2C_NO_INIT;
}
int bshal_i2cm_recv(bshal_i2cm_instance_t *i2c_instance, uint8_t address,
		void *p_data, uint8_t length, bool nostop) {
	if (!i2c_instance)
		return -1;
	return i2c_instance->i2cm_recv ?
			i2c_instance->i2cm_recv(i2c_instance->drv_specific, address, p_data,
					length, nostop) :
			I2C_NO_INIT;
}
int bshal_i2cm_isok(bshal_i2cm_instance_t *i2c_instance, uint8_t address) {
	if (!i2c_instance)
		return -1;
	return i2c_instance->i2cm_isok ?
			i2c_instance->i2cm_isok(i2c_instance->drv_specific, address) :
			I2C_NO_INIT;
}


// Convenience functions
int bshal_i2cm_send_reg(bshal_i2cm_instance_t* i2c_instance, uint8_t address, uint8_t reg, uint8_t * p_data, uint8_t length){
    int result;
    /*
    result = i2c_send(i2c_instance, address, &reg, 1, true);
    if (result) return result;
    return i2c_send(i2c_instance, address, p_data, length, false);
    */
    uint8_t buffer[1+length];
    buffer[0] = reg;
    memcpy(buffer+1, p_data, length);
    return bshal_i2cm_send(i2c_instance, address, buffer, 1+length, false);
}

int bshal_i2cm_recv_reg(bshal_i2cm_instance_t* i2c_instance, uint8_t address, uint8_t reg, uint8_t * p_data, uint8_t length){
    int result;
    result = bshal_i2cm_send(i2c_instance, address, &reg, 1, true);
    if (result) return result;
    return bshal_i2cm_recv(i2c_instance, address, p_data, length, false);
}
