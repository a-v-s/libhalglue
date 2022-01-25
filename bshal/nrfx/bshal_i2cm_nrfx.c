/*
 * bshal_i2cm_nrfx.c
 *
 *  Created on: 25 jan. 2022
 *      Author: André van Schoubroeck <andre@blaatschaap.be>
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "hal.h"
#include "bshal_i2cm.h"

#include "nrfx.h"
#include "nrfx_twim.h"

bool m_busy = false;
static void nrfx_twim_evt_handler(nrfx_twim_evt_t const *p_event,
		void *p_context) {
	m_busy = false;
}

int bshal_nrfx_i2cm_send(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop) {

	nrfx_twim_xfer_desc_t xfer;
	xfer.address = address;
	xfer.p_primary_buf = p_data;
	xfer.p_secondary_buf = NULL;
	xfer.primary_length = p_data;
	xfer.secondary_length = 0;
	xfer.type = NRFX_TWIM_XFER_TX;

	int status = nrfx_twim_xfer(drv_specific, &xfer,
			nostop ? NRFX_TWIM_FLAG_TX_NO_STOP : 0);
	if (status)
		return I2C_ERROR;
	// TODO We need to be able to escape on hang
	while (m_busy)
		;
}
int bshal_nrfx_i2cm_recv(void *drv_specific, uint8_t address, void *p_data,
		uint8_t length, bool nostop) {
	nrfx_twim_xfer_desc_t xfer;

	xfer.address = address;
	xfer.p_primary_buf = p_data;
	xfer.p_secondary_buf = NULL;
	xfer.primary_length = length;
	xfer.secondary_length = 0;
	xfer.type = NRFX_TWIM_XFER_RX;

	m_busy = true;
	int16_t status = nrfx_twim_xfer(drv_specific, &xfer, 0);
	if (status)
		return I2C_ERROR;
	// TODO We need to be able to escape on hang
	while (m_busy)
		;

}

int bshal_nrfx_i2cm_isok(void *drv_specific, uint8_t address) {

}

int bshal_nrfx_i2cm_init(bshal_i2cm_instance_t *i2c_instance) {
	i2c_instance->i2cm_isok = bshal_nrfx_i2cm_isok;
	i2c_instance->i2cm_recv = bshal_nrfx_i2cm_recv;
	i2c_instance->i2cm_send = bshal_nrfx_i2cm_send;

	nrfx_twim_config_t config;
	config.hold_bus_uninit = true;
	config.interrupt_priority = 6;
	config.scl = i2c_instance->scl_pin;
	config.sda = i2c_instance->sda_pin;
	config.frequency = NRF_TWIM_FREQ_400K;

	nrfx_twim_init(i2c_instance->drv_specific, &config, nrfx_twim_evt_handler,
			NULL);
	nrfx_twim_enable(i2c_instance->drv_specific);

}
