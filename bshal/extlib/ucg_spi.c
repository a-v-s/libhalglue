/*
 * ucglib_hal.c
 *
 *  Created on: 21 okt. 2020
 *      Author: andre
 */

#include "hal.h"

#include "ucg_spi.h"

#include "bshal_spim.h"
#include "bshal_delay.h"
#include <string.h>

//static bshal_spim_t *&((bshal_ucg_t*)(ucg_GetUserPtr(ucg)))->spim.instance;
//
//void ucg_com_bshal_set_config(bshal_spim_t *bsspim) {
//	&((bshal_ucg_t*)(ucg_GetUserPtr(ucg)))->spim.instance = bsspim;
//}

int16_t ucg_com_bshal(ucg_t *ucg, int16_t msg, uint16_t arg, uint8_t *data) {

	switch (msg) {
	case UCG_COM_MSG_POWER_UP:
		/* "data" is a pointer to ucg_com_info_t structure with the following information: */
		/*	((ucg_com_info_t *)data)->serial_clk_speed value in nanoseconds */
		/*	((ucg_com_info_t *)data)->parallel_clk_speed value in nanoseconds */
		/* "arg" is not used */

		/* This message is sent once at the uC startup and for power up. */
		/* setup i/o or do any other setup */


		bshal_gpio_cfg_out( ((bshal_ucg_t*) ucg_GetUserPtr(ucg))->spim.ncd_pin, 0, 1 );
		bshal_gpio_cfg_out(  ((bshal_ucg_t*) ucg_GetUserPtr(ucg))->spim.instance.rs_pin, 0, 1  );


		break;

	case UCG_COM_MSG_POWER_DOWN:
		/* "data" and "arg" are not used*/
		/* This message is sent for a power down request */

		// TODO
		break;
	case UCG_COM_MSG_DELAY:
		/* "data" is not used */
		/* "arg" contains the number of microseconds for the delay */
		/* By receiving this message, the following code should delay by */
		/* "arg" microseconds. One microsecond is 0.000001 second */

		bshal_delay_us(arg);

		break;
	case UCG_COM_MSG_CHANGE_RESET_LINE:
		/* "data" is not used */
		/* "arg" = 1: set the reset output line to 1 */
		/* "arg" = 0: set the reset output line to 0 */

		//HAL_GPIO_WritePin(GPIOA, RESET_PIN,arg);
		bshal_gpio_write_pin(
				((bshal_ucg_t*) (ucg_GetUserPtr(ucg)))->spim.instance.rs_pin,
				arg);

		break;
	case UCG_COM_MSG_CHANGE_CD_LINE:
		/* "ucg->com_status"  bit 0 contains the old level for the CD line */
		/* "data" is not used */
		/* "arg" = 1: set the command/data (a0) output line to 1 */
		/* "arg" = 0: set the command/data (a0) output line to 0 */

		bshal_gpio_write_pin(
				((bshal_ucg_t*) (ucg_GetUserPtr(ucg)))->spim.ncd_pin, arg>0);

		break;
	case UCG_COM_MSG_CHANGE_CS_LINE:
		/* "ucg->com_status"  bit 1 contains the old level for the CS line */
		/* "data" is not used */
		/* "arg" = 1: set the chipselect output line to 1 */
		/* "arg" = 0: set the chipselect output line to 0 */

		bshal_gpio_write_pin(
				((bshal_ucg_t*) (ucg_GetUserPtr(ucg)))->spim.instance.cs_pin,
				arg);
		break;
	case UCG_COM_MSG_SEND_BYTE:
		/* "data" is not used */
		/* "arg" contains one byte, which should be sent to the display */
		/* The current status of the CD line is available */
		/* in bit 0 of u8g->com_status */
	{
		int res = bshal_spim_transmit(
				&((bshal_ucg_t*) (ucg_GetUserPtr(ucg)))->spim.instance, &arg, 1,
				true);
//		if (res)
//			__BKPT(0);
	}
		break;
	case UCG_COM_MSG_REPEAT_1_BYTE:
		/* "data[0]" contains one byte */
		/* repeat sending the byte in data[0] "arg" times */
		/* The current status of the CD line is available */
		/* in bit 0 of u8g->com_status */
		arg++;

		{
			uint8_t buff[arg + 1];
			memset(buff, data[0], arg);
			int res = bshal_spim_transmit(
					&((bshal_ucg_t*) (ucg_GetUserPtr(ucg)))->spim.instance,
					buff, arg, true);
//			if (res)
//				__BKPT(0);
		}

		break;
	case UCG_COM_MSG_REPEAT_2_BYTES:

		/* "data[0]" contains first byte */
		/* "data[1]" contains second byte */
		/* repeat sending the two bytes "arg" times */
		/* The current status of the CD line is available */
		/* in bit 0 of u8g->com_status */

		//if (!arg) arg=1;//break; // we can get requests of 0 repeats?!?
		arg++;

		{
			uint8_t buff[2 * arg];
			for (int i = 0; i < arg; i++) {
				buff[2 * i + 0] = data[0];
				buff[2 * i + 1] = data[1];
			}

			int res = bshal_spim_transmit(
					&((bshal_ucg_t*) (ucg_GetUserPtr(ucg)))->spim.instance,
					buff, 2 * arg, true);
//			if (res)
//				__BKPT(0);
		}

		break;
	case UCG_COM_MSG_REPEAT_3_BYTES:
		/* "data[0]" contains first byte */
		/* "data[1]" contains second byte */
		/* "data[2]" contains third byte */
		/* repeat sending the three bytes "arg" times */
		/* The current status of the CD line is available */
		/* in bit 0 of u8g->com_status */
		//if (!arg) arg=1;//break; // we can get a request for zero times ?!?!?
		arg++;

		if (arg < 1)
			break;
		{
			uint8_t buff[3 * arg];
			for (int i = 0; i < arg; i++) {
				buff[3 * i + 0] = data[0];
				buff[3 * i + 1] = data[1];
				buff[3 * i + 2] = data[2];
			}

			int res = bshal_spim_transmit(
					&((bshal_ucg_t*) (ucg_GetUserPtr(ucg)))->spim.instance,
					buff, 3 * arg, true);
//			if (res)
//				__BKPT(0);
		}

		break;
	case UCG_COM_MSG_SEND_STR:
		/* "data" is an array with "arg" bytes */
		/* send "arg" bytes to the display */
	{
		int res = bshal_spim_transmit(
				&((bshal_ucg_t*) (ucg_GetUserPtr(ucg)))->spim.instance, data,
				arg, true);
//		if (res)
//			__BKPT(0);
	}
		break;
	case UCG_COM_MSG_SEND_CD_DATA_SEQUENCE:
		/* "data" is a pointer to two bytes, which contain the cd line */
		/* status and display data */
		/* "arg" contains the number of these two byte tuples which need to */
		/* be analysed and sent. Bellow is a example sequence */
		/* The content of bit 0 in u8g->com_status is undefined for this message */

		while (arg > 0) {
			if (*data != 0) {
				if (*data == 1) {
					/* set CD (=D/C=A0) line to low */
					bshal_gpio_write_pin(
							((bshal_ucg_t*) (ucg_GetUserPtr(ucg)))->spim.ncd_pin,
							0);
				} else {
					/* set CD (=D/C=A0) line to high */
					bshal_gpio_write_pin(
							((bshal_ucg_t*) (ucg_GetUserPtr(ucg)))->spim.ncd_pin,
							1);
				}
			}
			data++;
			/* send *data to the display */
			data++;
			arg--;
		}
		break;
	}
	return 1;
}

