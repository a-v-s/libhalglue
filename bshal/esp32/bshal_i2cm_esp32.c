#include "bshal_i2cm_esp32.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "bshal_i2cm.h"

#include <driver/i2c.h>

int bshal_i2cm_send(bshal_i2cm_instance_t *i2c_instance, uint8_t address,
		void *p_data, uint8_t length, bool nostop) {
	// TODO: this functions takes no flags to pass, so we cannot do "nostop" right now
	// Might have to look into queue mode
	return i2c_master_write_to_device(i2c_instance->hw_nr, address, p_data,
			length, 1);
}
int bshal_i2cm_recv(bshal_i2cm_instance_t *i2c_instance, uint8_t address,
		void *p_data, uint8_t length, bool nostop) {
	// TODO: this functions takes no flags to pass, so we cannot do "nostop" right now
	// Might have to look into queue mode
	return i2c_master_read_from_device(i2c_instance->hw_nr, address, p_data,
			length, 1);
}
int bshal_i2cm_isok(bshal_i2cm_instance_t *i2c_instance, uint8_t address) {
	// Might have to look into queue mode for implementing this
	uint8_t c;
	return bshal_i2cm_recv(i2c_instance, address, &c, 1, false);

}
int bshal_i2cm_send_reg(bshal_i2cm_instance_t *i2c_instance, uint8_t address,
		uint8_t reg, void *p_data, uint8_t length) {
	// Might have to look into queue mode for a better implementation not requiring this much stack space
	uint8_t buff[length + 1];
	buff[0] = reg;
	memcpy(buff + 1, p_data, length);
	return i2c_master_write_to_device(i2c_instance->hw_nr, address, buff,
			1 + length, 1);

}
int bshal_i2cm_recv_reg(bshal_i2cm_instance_t *i2c_instance, uint8_t address,
		uint8_t reg, void *p_data, uint8_t length) {
	return i2c_master_write_read_device(i2c_instance->hw_nr, address, &reg, 1,
			p_data, length, 1);
}

int bshal_i2cm_init(bshal_i2cm_instance_t *i2c_instance) {
	i2c_config_t conf = { .mode = I2C_MODE_MASTER, .sda_io_num =
			i2c_instance->sda_pin, .sda_pullup_en = GPIO_PULLUP_ENABLE,
			.scl_io_num = i2c_instance->scl_pin, .scl_pullup_en =
					GPIO_PULLUP_ENABLE, .master.clk_speed =
					i2c_instance->speed_hz, .clk_flags = 0, };

	i2c_param_config(i2c_instance->hw_nr, &conf);
	return i2c_driver_install(i2c_instance->hw_nr, conf.mode, 0, 0, 0);
}
