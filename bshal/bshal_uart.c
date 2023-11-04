/*
 * bshal_uart.c
 *
 *  Created on: 28 jan. 2022
 *      Author: andre
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "bshal_uart.h"

void bshal_uart_recv_cb(bshal_uart_async_t *info, size_t size) {
	if (size < (info->preprocess_buffer_len-info->preprocess_buff_write) ) {
		memcpy(info->preprocess_buffer+info->preprocess_buff_write, info->receive_buffer,size);
		info->preprocess_buff_write += size;
	} else {
		int first_part = (info->preprocess_buffer_len-info->preprocess_buff_write);
		int remaining = size - first_part;
		memcpy(info->preprocess_buffer+info->preprocess_buff_write, info->receive_buffer,first_part);
		memcpy(info->preprocess_buffer, info->receive_buffer+first_part,remaining);
		info->preprocess_buff_write=remaining;
	}
}


void bshal_uart_recv_process(bshal_uart_async_t *info) {

	// be synced in inital state. need to make this a
	// configurable option
	static bool synced = true;

	static int sync_begin_pos = 0;
	static int sync_begin_cnt = 0;
	static int sync_end_pos = 0;
	static int sync_end_cnt = 0;
	static int proc_buff_pos = 0;

	static int resync_begin_pos = 0;
	static int resync_begin_cnt = 0;

	static int resync_retry_cnt = 0;



	if (proc_buff_pos >= info->postprocess_buffer_len) {
		// INVALID
		synced = false;
		sync_begin_pos = 0;
		sync_begin_cnt = 0;
		sync_end_pos = 0;
		sync_end_cnt = 0;
		proc_buff_pos = 0;
	}

	while (info->preprocess_buff_read !=  info->preprocess_buff_write)
		if (synced) {

			info->postprocess_buffer[proc_buff_pos] =
					info->preprocess_buffer[info->preprocess_buff_read];

			//--resync

			if (info->sync_begin_len) {
				if (info->preprocess_buffer[info->preprocess_buff_read]
										 == info->sync_begin_data[resync_begin_cnt]) {
					if (!resync_begin_pos) resync_begin_pos = proc_buff_pos;

					resync_retry_cnt++;
					if (resync_retry_cnt > 1)
						asm("bkpt 0");

					if (info->sync_begin_len) {
						synced = true;
						sync_begin_pos = resync_begin_pos;
					} else {
						resync_begin_cnt = 1;
						resync_begin_pos = proc_buff_pos;
					}

					info->preprocess_buff_read++;
					info->preprocess_buff_read%=info->preprocess_buffer_len;
					continue;
				} else {
					resync_begin_pos = resync_begin_cnt = 0;
				}
			}

			//--resync

			if (info->preprocess_buff_read ==  info->preprocess_buff_write)
				return;
			if (info->sync_end_len) {
				if (info->preprocess_buffer[info->preprocess_buff_read]
										 == info->sync_end[sync_end_cnt]) {
//					if (!sync_end_cnt)
//						sync_end_pos = recv_buff_pos;
					sync_end_cnt++;
					if (sync_end_cnt == info->sync_end_len) {
						// Synchronisation completed

						char *begin = info->postprocess_buffer;
						size_t len = proc_buff_pos;
//						if ((int) len < 0)
//							asm("bkpt 0");

						if (!info->sync_begin_include) {
							begin += info->sync_begin_len - 1;
							len -= info->sync_begin_len - 1;
						}

						if (info->sync_end_include) {
							len += info->sync_end_len;
						}

						if (info->null_terminated_string) {

							if (info->sync_end_include) {
								info->postprocess_buffer[proc_buff_pos
													 + info->sync_end_len] = 0;
							} else {
								info->postprocess_buffer[proc_buff_pos] = 0;
							}
						}

						info->callback(begin, len);

						synced = false;
						sync_begin_pos = sync_end_pos = sync_begin_cnt =
								sync_end_cnt = proc_buff_pos = 0;
						memset(info->postprocess_buffer, 0,
								info->postprocess_buffer_len);
						continue;

					}
				}
			} else {
				// No end-of-message marker
				// Not handled yet
				// We should trigger the callback on a new transmission mark
			}

			proc_buff_pos++;
			info->preprocess_buff_read++;
			info->preprocess_buff_read%=info->preprocess_buffer_len;
		} else {
			// Not synced, search for synchronisation marker

			if (info->sync_begin_len) {
				if (info->preprocess_buffer[info->preprocess_buff_read]
										 == info->sync_begin_data[sync_begin_pos]) {
//					if (!sync_begin_cnt)
//						sync_begin_pos = recv_buff_pos;
					sync_begin_cnt++;
					info->preprocess_buff_read++;
					info->preprocess_buff_read%=info->preprocess_buffer_len;
					if (sync_begin_cnt == info->sync_begin_len) {
						synced = true;
						resync_retry_cnt=0;
//						if (info->sync_begin_include) {
//							memcpy(info->postprocess_buffer + proc_buff_pos,
//									info->preprocess_buffer + sync_begin_pos,
//									info->sync_begin_len);
//							proc_buff_pos += info->sync_begin_len;
//						}
					}
				} else {
					info->preprocess_buff_read++;
					info->preprocess_buff_read%=info->preprocess_buffer_len;
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

/*
void bshal_uart_recv_cb(bshal_uart_async_t *info, size_t size) {
//	static bool synced = false;

	// be synced in inital state. need to make this a
	// configurable option
	static bool synced = true;

	static int sync_begin_pos = 0;
	static int sync_begin_cnt = 0;
	static int sync_end_pos = 0;
	static int sync_end_cnt = 0;
	//static int recv_buff_pos = 0;
	static int proc_buff_pos = 0;

	int recv_buff_pos = 0;

	static int resync_begin_pos = 0;
	static int resync_begin_cnt = 0;

	static int resync_retry_cnt = 0;


	//	if (strstr(info->receive_buffer, "M590")) asm("bkpt 0");
	if (size <= 0)
		return;

	if (proc_buff_pos >= info->postprocess_buffer_len) {
		// INVALID
		synced = false;
		sync_begin_pos = 0;
		sync_begin_cnt = 0;
		sync_end_pos = 0;
		sync_end_cnt = 0;
		recv_buff_pos = 0;
		proc_buff_pos = 0;
	}

	while (recv_buff_pos < size)
		if (synced) {

			info->postprocess_buffer[proc_buff_pos] =
					info->receive_buffer[recv_buff_pos];

			//--resync

			if (info->sync_begin_len) {
				if (info->receive_buffer[recv_buff_pos]
										 == info->sync_begin_data[resync_begin_cnt]) {
					if (!resync_begin_pos) resync_begin_pos = recv_buff_pos;

					resync_retry_cnt++;
					if (resync_retry_cnt > 1)
						asm("bkpt 0");

					if (info->sync_begin_len) {
						synced = true;
						sync_begin_pos = resync_begin_pos;
					} else {
						resync_begin_cnt = 1;
						resync_begin_pos = recv_buff_pos;
					}

					recv_buff_pos++;
					continue;
				} else {
					resync_begin_pos = resync_begin_cnt = 0;
				}
			}

			//--resync

			if (recv_buff_pos >= size)
				return;
			if (info->sync_end_len) {
				if (info->receive_buffer[recv_buff_pos]
										 == info->sync_end[sync_end_cnt]) {
					if (!sync_end_cnt)
						sync_end_pos = proc_buff_pos;	//recv_buff_pos;
					sync_end_cnt++;
					if (sync_end_cnt == info->sync_end_len) {
						// Synchronisation completed

						char *begin = info->postprocess_buffer;
						size_t len = proc_buff_pos;
//						if ((int) len < 0)
//							asm("bkpt 0");

						if (!info->sync_begin_include) {
							begin += info->sync_begin_len - 1;
							len -= info->sync_begin_len - 1;
						}

						if (info->sync_end_include) {
							len += info->sync_end_len;
						}

						if (info->null_terminated_string) {

							if (info->sync_end_include) {
								info->postprocess_buffer[proc_buff_pos
													 + info->sync_end_len] = 0;
							} else {
								info->postprocess_buffer[proc_buff_pos] = 0;
							}
						}

						info->callback(begin, len);

						synced = false;
						sync_begin_pos = sync_end_pos = sync_begin_cnt =
								sync_end_cnt = proc_buff_pos = 0;
						memset(info->postprocess_buffer, 0,
								info->postprocess_buffer_len);
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
				if (info->receive_buffer[recv_buff_pos]
										 == info->sync_begin_data[sync_begin_pos]) {
					if (!sync_begin_cnt)
						sync_begin_pos = recv_buff_pos;
					sync_begin_cnt++;
					recv_buff_pos++;
					if (sync_begin_cnt == info->sync_begin_len) {
						synced = true;
						resync_retry_cnt=0;
						if (info->sync_begin_include) {
							memcpy(info->postprocess_buffer + proc_buff_pos,
									info->receive_buffer + sync_begin_pos,
									info->sync_begin_len);
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
*/
