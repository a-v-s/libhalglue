/*
 File:		endian.h
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

// Implementation of POSIX-style Endian Conversion functions for 
// NON-POSIX (e.g. embedded) enviorements
//
// For information about these functions see https://linux.die.net/man/3/endian 


#ifndef __ENDIAN_H_
#define __ENDIAN_H_

#include <stdint.h>



static inline uint16_t htobe16(uint16_t host_16bits);
static inline uint16_t htole16(uint16_t host_16bits);
static inline uint16_t be16toh(uint16_t big_endian_16bits);
static inline uint16_t le16toh(uint16_t little_endian_16bits);

static inline uint32_t htobe32(uint32_t host_32bits);
static inline uint32_t htole32(uint32_t host_32bits);
static inline uint32_t be32toh(uint32_t big_endian_32bits);
static inline uint32_t le32toh(uint32_t little_endian_32bits);

static inline uint64_t htobe64(uint64_t host_64bits);
static inline uint64_t htole64(uint64_t host_64bits);
static inline uint64_t be64toh(uint64_t big_endian_64bits);
static inline uint64_t le64toh(uint64_t little_endian_64bits);


// These are defined on GCC, but might be missing on other compilers
#ifndef __ORDER_LITTLE_ENDIAN__
#define __ORDER_LITTLE_ENDIAN__ 1234
#endif 

#ifndef __ORDER_PDP_ENDIAN__
#define __ORDER_PDP_ENDIAN__ 3412
#endif 

#ifndef __ORDER_BIG_ENDIAN__
#define __ORDER_BIG_ENDIAN__ 4321
#endif 

// When we cannot detect the byte order using the preprocessor, 
// We could use this to indicate a fallback to runtime detection
#ifndef __ORDER_RTDETECT__ 
#define __ORDER_RTDETECT__ 1111
#endif 


#ifndef __BYTE_ORDER__
	#ifdef SDCC
	// SDCC does not define any endianness macros
	// but by design SDCC always uses Little Endian
	#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
	#else
	#define __BYTE_ORDER__ __ORDER_RTDETECT__
	#endif
#endif

#if defined (__GNUC__) || defined (__clang__) 
	// Built-in functions in GNU GCC and LLVM CLANG
	// Should we add a minimal version detection to these?
	// We're not generally interested in ancient compiler versions ;)
	#define _bswap16 __builtin_bswap16
	#define _bswap32 __builtin_bswap32
	#define _bswap64 __builtin_bswap64
 #elif defined(_MSC_VER) 
	// Built-in functions in Microsoft C/C++ Compiler
	// Please note, there might be a bug, we might have to ass &&!defined(_DEBUG)
	// https://llvm.org/doxygen/SwapByteOrder_8h_source.html
	// We have to verify to which versions this applies.
   	#define _bswap16 _byteswap_ushort
	#define _bswap32 _byteswap_ulong
	#define _bswap64 _byteswap_uint64
 #else
	// SDCC does not have built in swappers.
		uint16_t _bswap16(uint16_t value) { return ((value & 0xFF00) >> 8) | ((value& 0x00FF) << 8); }
		uint32_t _bswap32(uint32_t value) {
	   		return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) | 
				((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24); 
		}
		uint64_t _bswap64(uint64_t value) {
		   uint64_t Hi = _bswap32(uint32_t(value));
		   uint32_t Lo = _bswap32(uint32_t(value >> 32));
		   return (Hi << 32) | Lo;
		}
#endif





#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
static inline uint16_t htobe16(uint16_t value) { return _bswap16(value);}
static inline uint16_t be16toh(uint16_t value) { return _bswap16(value);}

static inline uint16_t htole16(uint16_t value) { return value;}
static inline uint16_t le16toh(uint16_t value) { return value;}

static inline uint32_t htobe32(uint32_t value) { return _bswap32(value);}
static inline uint32_t be32toh(uint32_t value) { return _bswap32(value);}

static inline uint32_t htole32(uint32_t value) { return value;}
static inline uint32_t le32toh(uint32_t value) { return value;}

static inline uint64_t htobe64(uint64_t value) { return _bswap64(value);}
static inline uint64_t be64toh(uint64_t value) { return _bswap64(value);}

static inline uint64_t htole64(uint64_t value) { return value;}
static inline uint64_t le64toh(uint64_t value) { return value;}

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
static inline uint16_t htobe16(uint16_t value) { return value;}
static inline uint16_t be16toh(uint16_t value) { return value;}

static inline uint16_t htole16(uint16_t value) { return _bswap16(value);}
static inline uint16_t le16toh(uint16_t value) { return _bswap16(value);}

static inline uint32_t htobe32(uint32_t value) { return value;}
static inline uint32_t be32toh(uint32_t value) { return value;}

static inline uint32_t htole32(uint32_t value) { return _bswap32(value);}
static inline uint32_t le32toh(uint32_t value) { return _bswap32(value);}

static inline uint64_t htobe64(uint64_t value) { return value;}
static inline uint64_t be64toh(uint64_t value) { return value;}

static inline uint64_t htole64(uint64_t value) { return _bswap64(value);}
static inline uint64_t le64toh(uint64_t value) { return _bswap64(value);}
#elif __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
#error "PDP Endian not supported!"a
// These implementations fail to detect PDP Endianness, 
// however, I don't expect PDP-11 machines in the wild.
#elif __BYTE_ORDER__ == __ORDER_RTDETECT__

// Runtime detection of endianness: Set the memory per 8-bit and read it back
// as 32 bit, and see what numeric value the results holds. The value chosen
// to mimmick the __BYTE_ORDER__ defines by gcc.
typedef union {
	uint8_t _8bit[4];
    uint32_t _32bit;
} detect_endian_t;
static detect_endian_t endian = {._8bit = {4,3,2,1} };

#define __DETECT_LITTLE__ 	0x01020304
#define __DETECT_PDP__ 		0x03040102
#define __DETECT_BIG__ 		0x04030201

// These implementations fail to detect PDP Endianness, 
// however, I don't expect PDP-11 machines in the wild.

static inline uint16_t htobe16(uint16_t value) {
	return (endian._32_bit == __DETECT_BIG__) ? value : _bswap_16(value);
}

static inline uint16_t be16toh(uint16_t value) {
	return (endian._32_bit == __DETECT_BIG__) ? value : _bswap_16(value);
}

static inline uint16_t htole16(uint16_t value) {
	return (endian._32_bit == __DETECT_LITTLE__) ? value : _bswap_16(value);
}

static inline uint16_t bl16toh(uint16_t value) {
	return (endian._32_bit == __DETECT_LITLE__) ? value : _bswap_16(value);
}
static inline uint32_t htobe32(uint32_t value) {
	return (endian._32_bit == __DETECT_BIG__) ? value : _bswap_32(value);
}

static inline uint32_t be32toh(uint32_t value) {
	return (endian._32_bit == __DETECT_BIG__) ? value : _bswap_32(value);
}

static inline uint32_t htole32(uint32_t value) {
	return (endian._32_bit == __DETECT_LITTLE__) ? value : _bswap_32(value);
}

static inline uint32_t bl32toh(uint32_t value) {
	return (endian._32_bit == __DETECT_LITLE__) ? value : _bswap_32(value);
}

static inline uint64_t htobe64(uint64_t value) {
	return (endian._32_bit == __DETECT_BIG__) ? value : _bswap_64(value);
}

static inline uint64_t be64toh(uint64_t value) {
	return (endian._32_bit == __DETECT_BIG__) ? value : _bswap_64(value);
}

static inline uint64_t htole64(uint64_t value) {
	return (endian._32_bit == __DETECT_LITTLE__) ? value : _bswap_64(value);
}

static inline uint64_t bl64toh(uint64_t value) {
	return (endian._32_bit == __DETECT_LITLE__) ? value : _bswap_64(value);
}


#endif

#endif //__ENDIAN_H_

