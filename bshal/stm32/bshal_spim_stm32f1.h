/*
	File: spi.h
    License: MIT

    Copyright (c)  2020 André van Schoubroeck

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



typedef int (*spi_transceive_f)(void* drv_specific, void* sendBuff, void* recvBuff, size_t size);
typedef int (*spi_transmit_f)(void* drv_specific, void* sendBuff, size_t size);
typedef int (*spi_receive_f)(void* drv_specific, void* recvBuff, size_t size);

typeded int (*spi_set_nss_t)(void* drv_specific, bool value);
typeded int (*spi_set_nrs_t)(void* drv_specific, bool value);
typeded int (*spi_set_ncd_t)(void* drv_specific, bool value);



