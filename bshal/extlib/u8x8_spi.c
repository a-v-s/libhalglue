
#include "u8x8_spi.h"

#include "bshal_u8x8.h"

#include "bshal_spim.h"
#include "bshal_gpio.h"


#ifndef U8X8_WITH_USER_PTR
#warning please use U8X8_WITH_USER_PTR
static bshal_spim_instance_t *mp_spim;
static u8g2_t *mp_u8g2;
void bshal_u8x8_spi_init(u8g2_t *p_u8g2, bshal_spim_instance_t *p_spim) {
	mp_u8g2 = p_u8g2;
	mp_spim = p_spim;
}
#endif
//----------------------------------------------------------------------//
// https://github.com/olikraus/u8g2/wiki/Porting-to-new-MCU-platform    //
//----------------------------------------------------------------------//


uint8_t bshal_u8x8_byte_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
		void *arg_ptr) {
  
#ifdef U8X8_WITH_USER_PTR
	//bshal_spim_t *p_spim = u8x8->user_ptr;
	bshal_u8x8_t* p_if = u8x8->user_ptr;

#else
	//bshal_spim_t *p_spim = mp_spim;
	bshal_u8x8_t* p_if;
#endif


  switch(msg) {
    case U8X8_MSG_BYTE_SEND:
		// arg_prt = data 
		// arg_int = size
    	// Keep chip select active, as the disable chip select is a separate
    	// action in the u8g2 library.
		bshal_spim_transmit	(&p_if->spim.instance, arg_ptr, arg_int, true);

    	//bshal_spim_transmit	(&p_if->spim.instance, arg_ptr, arg_int, false);
      break;
    case U8X8_MSG_BYTE_INIT:
    	p_if->spim.instance.frequency = u8x8->display_info->sck_clock_hz;
		p_if->spim.instance.bit_order = 0; // 0 = MSB
		p_if->spim.instance.mode = u8x8->display_info->spi_mode;
		p_if->spim.instance.cs_pol = u8x8->display_info->chip_enable_level;
		bshal_spim_init(&p_if->spim.instance);

		bshal_gpio_write_pin(p_if->spim.instance.cs_pin, u8x8->display_info->chip_disable_level);

		bshal_gpio_cfg_out(p_if->spim.ncd_pin);

      break;
    case U8X8_MSG_BYTE_SET_DC:
		bshal_gpio_write_pin(p_if->spim.ncd_pin, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
    	bshal_spim_config(&p_if->spim.instance);

		bshal_gpio_write_pin(p_if->spim.instance.cs_pin, u8x8->display_info->chip_enable_level);
		u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);

		break;
    case U8X8_MSG_BYTE_END_TRANSFER:      
		bshal_gpio_write_pin(p_if->spim.instance.cs_pin, u8x8->display_info->chip_disable_level);
		u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
      break;
    default:
      return 0;
  }  
  return 1;
}
