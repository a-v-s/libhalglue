#include "bshal_spim_esp32.h"
#include "bshal_spim.h"

#include "driver/spi_master.h"
#include "driver/gpio.h"


#include <string.h>

int bshal_spim_init(bshal_spim_instance_t *bshal_spim) {
	int ret;
	spi_device_handle_t spi;
	spi_bus_config_t buscfg = { .miso_io_num = bshal_spim->miso_pin,
			.mosi_io_num = bshal_spim->mosi_pin, .sclk_io_num =
					bshal_spim->sck_pin, .quadwp_io_num = -1, .quadhd_io_num =
					-1, .max_transfer_sz = 128, };

//	spi_device_interface_config_t devcfg = {
//	.clock_speed_hz = bshal_spim->frequency, .mode = bshal_spim->mode,
//			.spics_io_num = bshal_spim->cs_pin, .queue_size = 1, // no queue
//			.pre_cb = NULL,	// no callback
//			};
//

	// Software control the CS pin, as hardware control ain't giving the
	// desired results
	spi_device_interface_config_t devcfg = {
		.clock_speed_hz = bshal_spim->frequency, .mode = bshal_spim->mode,
				.spics_io_num = -1, .queue_size = 1, // no queue
				.pre_cb = NULL,	// no callback
				};

	// TODO Initialise CS pin as GPIO, output mode
	gpio_config_t pininit ;
	pininit . pin_bit_mask = 1 << bshal_spim->cs_pin;
	pininit . mode = GPIO_MODE_OUTPUT;
	pininit . pull_up_en = 0;
	pininit . pull_down_en = 0;
	pininit . intr_type = GPIO_INTR_DISABLE;
	gpio_config(&pininit);

	gpio_set_level(bshal_spim->cs_pin, !bshal_spim->cs_pol);

	bshal_spim->drv_specific = spi;
	ret = spi_bus_initialize(bshal_spim->hw_nr, &buscfg, SPI_DMA_CH_AUTO);
	if (ret)
		return ret;
	ret = spi_bus_add_device(bshal_spim->hw_nr, &devcfg, &spi);
	if (ret)
		return ret;
	bshal_spim->drv_specific = spi;
	return ret;
}

int bshal_spim_transmit(bshal_spim_instance_t *bshal_spim, void *data,
		size_t amount, bool nostop) {
	spi_transaction_t t;
	if (amount == 0)
		return 0;             //no need to send anything
	memset(&t, 0, sizeof(t));       //Zero out the transaction
	t.length = amount * 8;     //Len is in bytes, transaction length is in bits.
	t.tx_buffer = data;               //Data
//	if (nostop) {
//		t.flags = SPI_TRANS_CS_KEEP_ACTIVE; //Keep CS active after data transfer
//	}
	gpio_set_level(bshal_spim->cs_pin, bshal_spim->cs_pol);
	int ret =  spi_device_transmit(bshal_spim->drv_specific, &t); //Transmit!
	if (!nostop)
		gpio_set_level(bshal_spim->cs_pin, !bshal_spim->cs_pol);

	return ret;

}

int bshal_spim_transceive(bshal_spim_instance_t *bshal_spim, void *data,
		size_t amount, bool nostop) {
	spi_transaction_t t;
	if (amount == 0)
		return 0;             //no need to send anything
	memset(&t, 0, sizeof(t));       //Zero out the transaction
	t.length = amount * 8;     //Len is in bytes, transaction length is in bits.
	t.tx_buffer = data;               //Data

	t.rx_buffer = t.tx_buffer;
	t.rxlength = t.length;
//	if (nostop) {
//		t.flags = SPI_TRANS_CS_KEEP_ACTIVE; //Keep CS active after data transfer
//	}
	gpio_set_level(bshal_spim->cs_pin, bshal_spim->cs_pol);
	int ret =  spi_device_transmit(bshal_spim->drv_specific, &t); //Transmit!
	if (!nostop)
			gpio_set_level(bshal_spim->cs_pin, !bshal_spim->cs_pol);
	return ret;

}

int bshal_spim_recveive(bshal_spim_instance_t *bshal_spim, void *data,
		size_t amount, bool nostop) {
	return bshal_spim_transceive(bshal_spim, data, amount, nostop);
}

int bshal_spim_transceive_split(bshal_spim_instance_t *bshal_spim, void *txdata,
		void *rxdata, size_t amount, bool nostop) {
	return -1;
}
