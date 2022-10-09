#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "hal.h"
#include "bshal_spim.h"
#include "bshal_gpio_stm32.h"



static SPI_HandleTypeDef m_spim[4];

int bshal_spim_config(bshal_spim_instance_t *config) {

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// PCLK1 to APB1 Peripherals (page 93)
	// PCLK2 to APB2 Peripherals (page 93)

	// SPI1  	is  on APB2 (page 47) thus uses PCLK2_Frequency
	// SPI2,3   are on APB1 (page 47) thus use  PCLK1_Frequency

	SPI_HandleTypeDef *handle;
	int spi_clock;
	switch (config->hw_nr) {
#ifdef SPI1
	case 1:
		m_spim[1].Instance = SPI1;
		handle = &m_spim[1];
		spi_clock = HAL_RCC_GetPCLK2Freq();
		__HAL_RCC_SPI1_CLK_ENABLE();
		break;
#endif
#ifdef SPI2
	case 2:
		m_spim[2].Instance = SPI2;
		spi_clock = HAL_RCC_GetPCLK1Freq();
		handle = &m_spim[2];
		__HAL_RCC_SPI2_CLK_ENABLE();
		break;
#endif
#ifdef SPI3
	case 3:
		m_spim[3].Instance = SPI3;
		spi_clock = HAL_RCC_GetPCLK1Freq();
		handle = &m_spim[3];
		__HAL_RCC_SPI3_CLK_ENABLE();
		break;
#endif
	default:
		return -1;
	}

	config->drv_specific = handle;

	handle->Init.Mode = SPI_MODE_MASTER;
	handle->Init.Direction = SPI_DIRECTION_2LINES;
	handle->Init.DataSize = SPI_DATASIZE_8BIT;
	handle->Init.NSS = SPI_NSS_SOFT;
	handle->Init.TIMode = SPI_TIMODE_DISABLE;

	if (config->bit_order)
		handle->Init.FirstBit = SPI_FIRSTBIT_LSB;
	else
		handle->Init.FirstBit = SPI_FIRSTBIT_MSB;

	int clk_div = spi_clock / config->frequency;
	int clk_mod = spi_clock % config->frequency;
	if (clk_mod)
		clk_div++;



	handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	if (clk_div <= 128)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	if (clk_div <= 64)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	if (clk_div <= 32)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	if (clk_div <= 16)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	if (clk_div <= 8)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	if (clk_div <= 4)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	if (clk_div <= 2)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;


	switch (config->mode) {
	case 0:
		handle->Init.CLKPolarity = SPI_POLARITY_LOW;
		handle->Init.CLKPhase = SPI_PHASE_1EDGE;
		break;
	case 1:
		handle->Init.CLKPolarity = SPI_POLARITY_LOW;
		handle->Init.CLKPhase = SPI_PHASE_2EDGE;
		break;
	case 2:
		handle->Init.CLKPolarity = SPI_POLARITY_LOW;
		handle->Init.CLKPhase = SPI_PHASE_2EDGE;
		break;
	case 3:
		handle->Init.CLKPolarity = SPI_POLARITY_HIGH;
		handle->Init.CLKPhase = SPI_PHASE_2EDGE;
		break;
	default:
		return -1;
	}

	return HAL_SPI_Init(handle);
}

int bshal_spim_init(bshal_spim_instance_t *config) {
	int result = bshal_spim_config(config);
	if (result)
		return result;

	GPIO_InitTypeDef GPIO_InitStruct;
	// Common configuration for all channels
	//GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_TypeDef *port = NULL;
	uint16_t pin = -1;

	bool at_least_miso_or_mosi = false;

	bshal_gpio_decode_pin(config->mosi_pin, &port, &pin);
	bshal_gpio_port_enable_clock(config->mosi_pin);
	if (port) {

		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pin = pin;
		HAL_GPIO_Init(port, &GPIO_InitStruct);
		at_least_miso_or_mosi = true;
	}

	bshal_gpio_decode_pin(config->miso_pin, &port, &pin);
	bshal_gpio_port_enable_clock(config->miso_pin);
	if (port) {
		GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitStruct.Pin = pin;
		HAL_GPIO_Init(port, &GPIO_InitStruct);
		at_least_miso_or_mosi = true;
	}

	if (!at_least_miso_or_mosi) {
		return -1;
	}

	bshal_gpio_decode_pin(config->sck_pin, &port, &pin);
	bshal_gpio_port_enable_clock(config->sck_pin);
	if (port) {
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pin = pin;
		HAL_GPIO_Init(port, &GPIO_InitStruct);
	} else {
		return -1;
	}

	bshal_gpio_decode_pin(config->cs_pin, &port, &pin);
	bshal_gpio_port_enable_clock(config->cs_pin);
	if (port) {
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pin = pin;
		HAL_GPIO_Init(port, &GPIO_InitStruct);
		bshal_gpio_write_pin(config->cs_pin, !config->cs_pol);
	}

	bshal_gpio_decode_pin(config->rs_pin, &port, &pin);
	bshal_gpio_port_enable_clock(config->rs_pin);
	if (port) {
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pin = pin;
		HAL_GPIO_Init(port, &GPIO_InitStruct);
		bshal_gpio_write_pin(config->rs_pin, !config->rs_pol);
	}

	return 0;
}

int bshal_spim_transceive(bshal_spim_instance_t *bshal_spim, void *data, size_t size,
		bool nostop) {
		int result;
//	result = bshal_spim_config(bshal_spim);
//	if (result)
//		return result;

	bshal_gpio_write_pin(bshal_spim->cs_pin, bshal_spim->cs_pol);
	result = HAL_SPI_TransmitReceive(bshal_spim->drv_specific, data, data,
			size, 1000);
	if (!nostop)
			bshal_gpio_write_pin(bshal_spim->cs_pin, !bshal_spim->cs_pol);
	return result;
}

int bshal_spim_transceive_split(bshal_spim_instance_t *bshal_spim, void *txdata,void * rxdata, size_t size,
		bool nostop) {
		int result;
//	int result = bshal_spim_config(bshal_spim);
//	if (result)
//		return result;

	bshal_gpio_write_pin(bshal_spim->cs_pin, bshal_spim->cs_pol);
	result = HAL_SPI_TransmitReceive(bshal_spim->drv_specific, txdata, rxdata,
			size, 1000);
	if (!nostop)
			bshal_gpio_write_pin(bshal_spim->cs_pin, !bshal_spim->cs_pol);
	return result;
}


int bshal_spim_transmit(bshal_spim_instance_t *bshal_spim, void *data, size_t size,
		bool nostop) {

	int result;

	// This was a fix for the display problems with u8g2lib,
	// but it breaks the rfid lib.

//	static bshal_spim_instance_t *prev = NULL;
//	if (bshal_spim != prev) {
//		if (prev) {
//			bshal_gpio_write_pin(bshal_spim->cs_pin, !bshal_spim->cs_pol);
//		}
//		result = bshal_spim_config(bshal_spim);
//		prev = bshal_spim;
//	}

	// Well, it might break thm3060 and pn5180?

//	result = bshal_spim_config(bshal_spim);
//	if (result)
//		return result;

	bshal_gpio_write_pin(bshal_spim->cs_pin, bshal_spim->cs_pol);

	result = HAL_SPI_Transmit(bshal_spim->drv_specific, data, size, 1000);
	if (!nostop)
		bshal_gpio_write_pin(bshal_spim->cs_pin, !bshal_spim->cs_pol);

	return result;
}
int bshal_spim_receive(bshal_spim_instance_t *bshal_spim, void *data, size_t size,
		bool nostop) {
	int result;
//	result = bshal_spim_config(bshal_spim);
//	if (result)
//		return result;

	bshal_gpio_write_pin(bshal_spim->cs_pin, bshal_spim->cs_pol);
	result = HAL_SPI_Receive(bshal_spim->drv_specific, data, size, 1000);
	if (!nostop)
		bshal_gpio_write_pin(bshal_spim->cs_pin, !bshal_spim->cs_pol);
	return result;
}

