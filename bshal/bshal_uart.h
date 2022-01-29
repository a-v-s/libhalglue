/*
 * bshal_uart.h
 *
 *  Created on: 28 jan. 2022
 *      Author: andre
 */

#ifndef LIBHALGLUE_BSHAL_UART_H_
#define LIBHALGLUE_BSHAL_UART_H_

#include <stdlib.h>
#include <stdint.h>

typedef void (*bshal_uart_async_cb)(void* data, size_t size);

typedef struct {
	uint8_t    * receive_buffer;
	size_t    receive_buffer_len;
	uint8_t    * process_buffer;
	size_t    process_buffer_len;
	uint8_t * sync_begin_data;
	size_t    sync_begin_len;
	uint8_t   sync_begin_include;
	uint8_t * sync_end;
	size_t    sync_end_len;
	uint8_t   sync_end_include;
	uint8_t   max_data_len;
	uint8_t   null_terminated_string;

	bshal_uart_async_cb callback;
} bshal_uart_async_t;

typedef enum {
	bshal_uart_flow_control_none,
	bshal_uart_flow_control_hw
} bshal_uart_flow_control_t;

typedef enum {
	bshal_uart_parity_none,
	bshal_uart_parity_even,
	bshal_uart_parity_odd,
} bshal_uart_flow_parity_t;

typedef struct {
	void*       drv_specific;
	uint8_t     hw_nr; 	       ///< Hardware number
	bshal_uart_flow_control_t fc : 4;
	bshal_uart_flow_parity_t parity : 4;
	uint32_t    bps;
	uint8_t     data_bits;
	uint8_t     stop_bits;

	uint8_t     txd_pin;            ///< TXD pin number.
    uint8_t     rxd_pin;            ///< RXD pin number.
    uint8_t     cts_pin;            ///< CTS pin number.
    uint8_t     rts_pin;            ///< RTS pin number.
    bshal_uart_async_t * async;

} bshal_uart_instance_t;



#endif /* LIBHALGLUE_BSHAL_UART_H_ */
