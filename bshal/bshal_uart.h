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
#include <stdbool.h>

typedef void (*bshal_uart_async_cb)(void* data, size_t size);

typedef struct {
	uint8_t    * receive_buffer;
	size_t    receive_buffer_len;

	uint8_t    * preprocess_buffer;
	size_t    preprocess_buffer_len;
	uint32_t preprocess_buff_read;
	uint32_t preprocess_buff_write;

	uint8_t    * postprocess_buffer;
	size_t    postprocess_buffer_len;

	bshal_uart_async_cb callback;

	uint8_t * sync_begin_data;
	size_t    sync_begin_len;
	uint8_t   sync_begin_include;
	uint8_t * sync_end;
	size_t    sync_end_len;
	uint8_t   sync_end_include;
	uint8_t   max_data_len;
	uint8_t   null_terminated_string;


} bshal_uart_async_t;

typedef struct {
	uint8_t    * receive_buffer;
	size_t    receive_buffer_len;

	uint8_t    * preprocess_buffer;
	size_t    preprocess_buffer_len;
	uint32_t preprocess_buff_read;
	uint32_t preprocess_buff_write;

	uint8_t    * postprocess_buffer;
	size_t    postprocess_buffer_len;

	bshal_uart_async_cb command_callback;
	bshal_uart_async_cb prompt_callback;
	bshal_uart_async_cb data_callback;

	bool data_mode;


} bshal_uart_async_at_t;


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

    bshal_uart_async_t * async; // async data structure according the the appropriate handler

} bshal_uart_instance_t;

void bshal_uart_at_recv_process(bshal_uart_async_at_t *info) ;

#endif /* LIBHALGLUE_BSHAL_UART_H_ */
