

#include "u8x8_i2c.h"

//----------------------------------------------------------------------//
// https://github.com/olikraus/u8g2/wiki/Porting-to-new-MCU-platform    //
//----------------------------------------------------------------------//

static u8g2_t *mp_u8g2;
static bshal_i2cm_instance_t *mp_i2c;

void bshal_u8x8_i2c_init(u8g2_t *p_u8g2, bshal_i2cm_instance_t *p_i2c) {
	mp_u8g2 = p_u8g2;
	mp_i2c = p_i2c;
}


// As per documentation, these are mainly for bit banging.
// As this file provides hardware i2c support these are not
// really used, but I believe some delay function have been called.
uint8_t bshal_u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg,
		uint8_t arg_int, void *arg_ptr) {
	switch (msg) {
	case U8X8_MSG_GPIO_AND_DELAY_INIT: 	// called once during init phase of u8g2/u8x8
										// can be used to setup pins
		break;
	case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
			// Well.. if we need nanosecond precision delays
			// I guess we won't be running on a simple microcontroller
		bshal_delay_us(arg_int/1000);
		break;
	case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
			// We might be able to implement a delay in this precision
			// But for now, we'll have to do with microseconds
		bshal_delay_us(arg_int/10);
		break;
	case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
		bshal_delay_us(10*arg_int);
		break;
	case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
		bshal_delay_ms(arg_int);
		break;
	case U8X8_MSG_DELAY_I2C:	// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
								// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
		break;
	case U8X8_MSG_GPIO_D0:		// D0 or SPI clock pin: Output level in arg_int
		//case U8X8_MSG_GPIO_SPI_CLOCK:
		break;
	case U8X8_MSG_GPIO_D1:		// D1 or SPI data pin: Output level in arg_int
		//case U8X8_MSG_GPIO_SPI_DATA:
		break;
	case U8X8_MSG_GPIO_D2:				// D2 pin: Output level in arg_int
		break;
	case U8X8_MSG_GPIO_D3:				// D3 pin: Output level in arg_int
		break;
	case U8X8_MSG_GPIO_D4:				// D4 pin: Output level in arg_int
		break;
	case U8X8_MSG_GPIO_D5:				// D5 pin: Output level in arg_int
		break;
	case U8X8_MSG_GPIO_D6:				// D6 pin: Output level in arg_int
		break;
	case U8X8_MSG_GPIO_D7:				// D7 pin: Output level in arg_int
		break;
	case U8X8_MSG_GPIO_E:				// E/WR pin: Output level in arg_int
		break;
	case U8X8_MSG_GPIO_CS:		// CS (chip select) pin: Output level in arg_int
		break;
	case U8X8_MSG_GPIO_DC:// DC (data/cmd, A0, register select) pin: Output level in arg_int
		break;
	case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
		break;
	case U8X8_MSG_GPIO_CS1:	// CS1 (chip select) pin: Output level in arg_int
		break;
	case U8X8_MSG_GPIO_CS2:	// CS2 (chip select) pin: Output level in arg_int
		break;
	case U8X8_MSG_GPIO_I2C_CLOCK:	// arg_int=0: Output low at I2C clock pin
		break;		// arg_int=1: Input dir with pullup high for I2C clock pin
	case U8X8_MSG_GPIO_I2C_DATA:		// arg_int=0: Output low at I2C data pin
		break;		// arg_int=1: Input dir with pullup high for I2C data pin
	case U8X8_MSG_GPIO_MENU_SELECT:
		u8x8_SetGPIOResult(u8x8, /* get menu select pin state */0);
		break;
	case U8X8_MSG_GPIO_MENU_NEXT:
		u8x8_SetGPIOResult(u8x8, /* get menu next pin state */0);
		break;
	case U8X8_MSG_GPIO_MENU_PREV:
		u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */0);
		break;
	case U8X8_MSG_GPIO_MENU_HOME:
		u8x8_SetGPIOResult(u8x8, /* get menu home pin state */0);
		break;
	default:
		u8x8_SetGPIOResult(u8x8, 1);			// default return value
		break;
	}
	return 1;
}

uint8_t bshal_u8x8_byte_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr) {
	static uint8_t buffer[32]; /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
	static uint8_t buf_idx;
	uint8_t *data;

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
		bshal_i2cm_send(mp_i2c, u8g2_GetI2CAddress(mp_u8g2) >> 1, buffer,
				buf_idx, false);
		break;
	default:
		return 0;
	}
	return 1;
}
