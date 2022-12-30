#include "bshal_spim_esp8622.h"
#include "bshal_spim.h"


#include "driver/gpio.h"
#include "driver/spi.h"


int bshal_spim_init(bshal_spim_instance_t *bshal_spim) {
	// TODO: set speed, spi mode, etc.

    spi_config_t spi_config= {0};
    // Load default interface parameters
    // CS_EN:1, MISO_EN:1, MOSI_EN:1, BYTE_TX_ORDER:1, BYTE_TX_ORDER:1, BIT_RX_ORDER:0, BIT_TX_ORDER:0, CPHA:0, CPOL:0
    spi_config.interface.val = SPI_DEFAULT_INTERFACE;
    spi_config.interface.cs_en = 0; // manual control

    // Load default interrupt enable
    // TRANS_DONE: true, WRITE_STATUS: false, READ_STATUS: false, WRITE_BUFFER: false, READ_BUFFER: false
    //spi_config.intr_enable.val = SPI_MASTER_DEFAULT_INTR_ENABLE;

    // Set SPI to master mode
    // ESP8266 Only support half-duplex
    spi_config.mode = SPI_MASTER_MODE;
    // Set the SPI clock frequency division factor
    spi_config.clk_div = SPI_10MHz_DIV;
    spi_config.event_cb = NULL;
    spi_init(HSPI_HOST, &spi_config);

    gpio_config_t io_conf = {0};
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = 1 << 15; // TODO
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
	gpio_set_level(15,1); // TODO
	return 0;
}

int bshal_spim_transmit(bshal_spim_instance_t *bshal_spim, void *data,
		size_t amount, bool nostop) {
	if (amount == 0) return 0;

//	puts ( "SPI Transmit:" );
//	for (int i = 0; i < amount ; i++) {
//		printf("%02X ", ((uint8_t*)data)[i] );
//	}
//	puts("");

    spi_trans_t trans = {0};
    trans.bits.mosi = amount * 8;       
    trans.mosi = data;
	gpio_set_level(15,0);
	int result = spi_trans(HSPI_HOST, &trans); 
	if (!nostop)
		gpio_set_level(15,1);
	return result;
}

int bshal_spim_transceive(bshal_spim_instance_t *bshal_spim, void *data,
		size_t amount, bool nostop) {
	if (amount == 0) return 0;

//	puts ( "SPI Transmit: (tranceive)" );
//	for (int i = 0; i < amount ; i++) {
//		printf("%02X ", ((uint8_t*)data)[i] );
//	}
//	puts("");

	spi_trans_t trans = {0};
    trans.bits.mosi = amount * 8;
    trans.bits.miso = amount * 8;              
    trans.miso = data;
    trans.mosi = data;
	gpio_set_level(15,0);
	int result = spi_trans(HSPI_HOST, &trans); 
	if (!nostop)
		gpio_set_level(15,1);

//	puts ( "SPI Receive: (tranceive)" );
//	for (int i = 0; i < amount ; i++) {
//		printf("%02X ", ((uint8_t*)data)[i] );
//	}
//	puts("");

	return result;
}

int bshal_spim_recveive(bshal_spim_instance_t *bshal_spim, void *data,
		size_t amount, bool nostop) {
	if (amount == 0) return 0;
    spi_trans_t trans = {0};
    trans.bits.miso = amount * 8;       
    trans.miso = data;
	gpio_set_level(15,0);
	int result = spi_trans(HSPI_HOST, &trans); 
	if (!nostop)
		gpio_set_level(15,1);

//	puts ( "SPI Receive:" );
//	for (int i = 0; i < amount ; i++) {
//		printf("%02X ", ((uint8_t*)data)[i] );
//	}
//	puts("");

	return result;

}

int bshal_spim_transceive_split(bshal_spim_instance_t *bshal_spim, void *txdata,
		void *rxdata, size_t amount, bool nostop) {
	return -1;
}
