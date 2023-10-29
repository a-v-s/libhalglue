/*
 * bshal_uart.c
 *
 *  Created on: 28 jan. 2022
 *      Author: andre
 */

#include <stdbool.h>

#include "bshal_uart.h"

void bshal_uart_recv_cb(bshal_uart_async_t *info, size_t size) {
	// WARNING: This code is not safe yet!
	// TODO: Check buffer bounds while processing

	static bool synced = false;
	static int sync_begin_pos = 0 ;
	static int sync_begin_cnt = 0 ;
	static int sync_end_pos = 0 ;
	static int sync_end_cnt = 0 ;
	//static int recv_buff_pos = 0;
	static int proc_buff_pos = 0;

	int recv_buff_pos = 0;

	if (size <= 0) return;

	if (proc_buff_pos >= info->process_buffer_len) {
		// INVALID
		synced = false;
		sync_begin_pos = 0 ;
		sync_begin_cnt = 0 ;
		sync_end_pos = 0 ;
		sync_end_cnt = 0 ;
		recv_buff_pos = 0;
		proc_buff_pos = 0;
	}

	recv_buff_pos = 0;

	while (recv_buff_pos <= size)

	if (synced) {
		info->process_buffer[proc_buff_pos] = info->receive_buffer[recv_buff_pos];
		if (recv_buff_pos >= size) return;
		if (info->sync_end_len) {
			if (info->receive_buffer[recv_buff_pos] == info->sync_end[sync_end_cnt]) {
				if (!sync_end_cnt) sync_end_pos = recv_buff_pos;
				sync_end_cnt++;
				if (sync_end_cnt == info->sync_end_len) {
					// Synchronisation completed

					char * begin = info->process_buffer;
					size_t len = sync_end_pos - sync_begin_pos;
					if (!info->sync_begin_include) {
						begin += info->sync_begin_len-1;
						len -= info->sync_begin_len-1;
					}

					if (info->sync_end_include)
						len += info->sync_end_len;

					if (info->null_terminated_string) {
						info->process_buffer[proc_buff_pos+1]=0;

						//len++;
//						if (info->sync_end_include) {
//							info->process_buffer[proc_buff_pos+1]=0;
//						} else {
//							info->process_buffer[sync_end_pos-info->sync_end_len]=0;
//						}
					}

					// well... len is incorrectly set
					len = strlen(begin); // temp fix
					info->callback(begin, len);

					synced = false;
					sync_begin_pos = sync_end_pos =
					sync_begin_cnt = sync_end_cnt = proc_buff_pos = 0;
					memset(info->process_buffer, 0, info->process_buffer_len);
					continue;



				}
			}
		} else {
			// No end-of-message marker
			// Not handled yet
			// We should trigger the callback on a new transmission mark
		}

		proc_buff_pos++;
		recv_buff_pos++;
	} else {
		// Not synced, search for synchronisation marker

		if (info->sync_begin_len) {
			if (info->receive_buffer[recv_buff_pos] == info->sync_begin_data[sync_begin_pos]) {
				if (!sync_begin_cnt)
					sync_begin_pos = recv_buff_pos;
				sync_begin_cnt++;
				recv_buff_pos++;
				if (sync_begin_cnt == info->sync_begin_len) {
					synced = true;
					if (info->sync_begin_include) {
						memcpy(info->process_buffer + proc_buff_pos, info->receive_buffer + sync_begin_pos, info->sync_begin_len);
						proc_buff_pos += info->sync_begin_len;
					}
				}
			} else {
				recv_buff_pos++;
			}
		} else {
			// No begin-of-message marker
			// Not handled yet
			// We should trigger the callback on an end of transmission marker
			// And then begin the new message anyways

			// Would setting the synced flag suffice if we have an end merker ?
			synced = true;
		}

	}

}



