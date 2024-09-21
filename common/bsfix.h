/*
 * bsfix.h
 *
 *  Created on: 21 sep. 2024
 *      Author: andre
 */

#ifndef LIBHALGLUE_COMMON_BSFIX_H_
#define LIBHALGLUE_COMMON_BSFIX_H_

#if (__SACCUM_IBIT__ == 8) && (__SACCUM_FBIT__== 7)
typedef short _Accum accum_8_7_t;
#endif

#if (__ACCUM_IBIT__ == 16) && (__ACCUM_FBIT__ == 15)
typedef _Accum accum_16_15_t;
#endif

#if (__USACCUM_IBIT__ == 8) && (__USACCUM_FBIT__ == 8)
typedef short unsigned _Accum uaccum_8_8_t;
#endif

#if (__UACCUM_IBIT__ == 16) && (__UACCUM_FBIT__ ==16)
typedef unsigned _Accum uaccum_16_16_t;
#endif

#endif /* LIBHALGLUE_COMMON_BSFIX_H_ */
