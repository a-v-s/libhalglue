

#include "u8x8_i2c.h"

//----------------------------------------------------------------------//
// https://github.com/olikraus/u8g2/wiki/Porting-to-new-MCU-platform    //
//----------------------------------------------------------------------//



#ifndef U8X8_WITH_USER_PTR
#error please use U8X8_WITH_USER_PTR
static bshal_i2cm_instance_t *mp_i2c;
static u8g2_t *mp_u8g2;
void bshal_u8x8_i2c_init(u8g2_t *p_u8g2, bshal_i2cm_instance_t *p_i2c) {
	mp_u8g2 = p_u8g2;
	mp_i2c = p_i2c;
}
#endif


uint8_t bshal_u8x8_byte_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr) {
	static uint8_t buffer[32]; /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
	static uint8_t buf_idx;
	uint8_t *data;

#ifdef U8X8_WITH_USER_PTR
	bshal_i2cm_instance_t *p_i2cm = u8x8->user_ptr;
#else
	bshal_i2cm_instance_t *p_i2cm = mp_i2c;
#endif

	switch (msg) {
	case U8X8_MSG_BYTE_SEND:
		data = (uint8_t*) arg_ptr;
		while (arg_int > 0) {
			buffer[buf_idx++] = *data;
			data++;
			arg_int--;
		}
		break;
	case U8X8_MSG_BYTE_INIT:
		/* add your custom code to init i2c subsystem */
		// For our use, we expect an already initialised I²C bus being provided
		// Why? We assume the I²C bus is being shared, and it's beyond the
		// responsibility of the graphics library interfacing  layer
		break;
	case U8X8_MSG_BYTE_SET_DC:
		/* ignored for i2c */
		break;
	case U8X8_MSG_BYTE_START_TRANSFER:
		buf_idx = 0;
		break;
	case U8X8_MSG_BYTE_END_TRANSFER:
		// u8x8 uses 8 bit addressing
		// bshal uses 7 bit addressing
		bshal_i2cm_send(p_i2cm, u8x8->i2c_address >> 1, buffer,
				buf_idx, false);
		break;
	default:
		return 0;
	}
	return 1;
}
