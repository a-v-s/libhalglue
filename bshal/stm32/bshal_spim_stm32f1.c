#include "hal.h"
#include "bshal_spim.h"
#include "bshal_gpio_stm32.h"

static SPI_HandleTypeDef m_spim[4];

int bshal_spim_config(bshal_spim_t *config) {
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

	if (clk_div <= 2)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	if (clk_div <= 4)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	if (clk_div <= 8)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	if (clk_div <= 16)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	if (clk_div <= 32)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	if (clk_div <= 64)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	if (clk_div <= 128)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	if (clk_div > 128)
		handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;

	switch (config->mode) {
	case 0:
		handle->Init.CLKPolarity = SPI_POLARITY_LOW;
		handle->Init.CLKPhase = SPI_PHASE_1EDGE;
		break;
	case 1:
		// TODO
		break;
	case 2:
		// TODO
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

int bshal_spim_init(bshal_spim_t *config) {
	int result = bshal_spim_config(config);
	if (result) return result;

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_TypeDef *port = NULL;
	uint16_t pin = -1;

	bshal_gpio_decode_pin(config->mosi_pin, &port, &pin);
	if (!port)
		return -1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin = pin;
	HAL_GPIO_Init(port, &GPIO_InitStruct);

	bshal_gpio_decode_pin(config->sck_pin, &port, &pin);
	if (!port)
		return -1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin = pin;
	HAL_GPIO_Init(port, &GPIO_InitStruct);

	bshal_gpio_decode_pin(config->miso_pin, &port, &pin);
	if (port) {
		GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
		GPIO_InitStruct.Pin = pin;
		HAL_GPIO_Init(port, &GPIO_InitStruct);
	}



	// nss, ncd as output
	// irq as input (or as pin interrupt)

}
