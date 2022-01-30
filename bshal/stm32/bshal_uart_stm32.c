/*
 * bshal_uart_stm32.c
 *
 *  Created on: 28 jan. 2022
 *      Author: andre
 */

#include "hal.h"
#include "bshal_uart.h"

// Note: Still have to see if we need to diversify between families
#if defined USART3
#define STM32_USART_COUNT 3
#elif defined USART2
#define STM32_USART_COUNT 2
#elif defined USART1
#define STM32_USART_COUNT 1
#else
#error "NO USART HARDWARE AVAILBLE"
#endif

//#define STM32_USART_COUNT (defined(USART1) + defined(USART2) + defined(USART3))
static UART_HandleTypeDef m_uart[STM32_USART_COUNT];

bshal_uart_instance_t *__hacky_uart_instance;

int bshal_stm32_uart_init(bshal_uart_instance_t *uart_instance) {

	__hacky_uart_instance = uart_instance;



	// TODO: configure pins and hardware
	UART_HandleTypeDef *handle = NULL;
	switch (uart_instance->hw_nr) {
#ifdef USART1
	case 1:
		handle = &m_uart[0];
		handle->Instance = USART1;
		__HAL_RCC_USART1_CLK_ENABLE();
		NVIC_EnableIRQ(USART1_IRQn);
		break;
#endif

#ifdef USART2
	case 2:
		handle = &m_uart[1];
		handle->Instance = USART2;
		__HAL_RCC_USART2_CLK_ENABLE();
		NVIC_EnableIRQ(USART2_IRQn);
		break;
#endif

#ifdef USART3
	case 3:
		handle = &m_uart[2];
		handle->Instance = USART3;
		__HAL_RCC_USART3_CLK_ENABLE();
		NVIC_EnableIRQ(USART3_IRQn);
		break;
#endif
	default:
		break;
	}
	if (!handle)
		return -1;

	uart_instance->drv_specific = handle;

	handle->Init.BaudRate = uart_instance->bps;
	handle->Init.HwFlowCtl = uart_instance->fc == bshal_uart_flow_control_hw ? UART_HWCONTROL_RTS_CTS : UART_HWCONTROL_NONE;
	handle->Init.Mode = UART_MODE_TX_RX; // TODO: support TX and RX-only modes
	handle->Init.OverSampling = UART_OVERSAMPLING_16;
	switch (uart_instance->parity) {
	case bshal_uart_parity_none:
		handle->Init.Parity = UART_PARITY_NONE;
		break;
	case bshal_uart_parity_even:
		handle->Init.Parity = UART_PARITY_EVEN;
		break;
	case bshal_uart_parity_odd:
		handle->Init.Parity = UART_PARITY_ODD;
		break;
	}

	handle->Init.StopBits; // TODO, for now leave empty for 1, we can add support for 2

	// TODO: How does wordlength work, as the choice is 8 or 9,
	// And UART has 7 or 8 databits, so is this start + data or what?
	handle->Init.WordLength = UART_WORDLENGTH_8B;

	// TODO Pins

	GPIO_InitTypeDef  GPIO_InitStruct = {0};
	GPIO_TypeDef *port = NULL;
	uint16_t pin;

	bshal_gpio_decode_pin(uart_instance->txd_pin, &port, &pin);
	bshal_gpio_port_enable_clock(uart_instance->txd_pin);
	GPIO_InitStruct.Pin       = pin;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(port, &GPIO_InitStruct);

	bshal_gpio_decode_pin(uart_instance->rxd_pin, &port, &pin);
	bshal_gpio_port_enable_clock(uart_instance->rxd_pin);
	GPIO_InitStruct.Pin 	  = pin;
	GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
	HAL_GPIO_Init(port, &GPIO_InitStruct);

	HAL_UART_Init(handle);

	if (uart_instance->async) {
		//HAL_UART_Receive_IT(uart_instance->drv_specific, uart_instance->async->buffer, uart_instance->async->buffer_len);
		HAL_UARTEx_ReceiveToIdle_IT(uart_instance->drv_specific, uart_instance->async->receive_buffer, uart_instance->async->receive_buffer_len);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {


}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	bshal_uart_recv_cb(__hacky_uart_instance->async,  Size);
	HAL_UARTEx_ReceiveToIdle_IT(__hacky_uart_instance->drv_specific, __hacky_uart_instance->async->receive_buffer, __hacky_uart_instance->async->receive_buffer_len);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){

	HAL_UARTEx_ReceiveToIdle_IT(__hacky_uart_instance->drv_specific, __hacky_uart_instance->async->receive_buffer, __hacky_uart_instance->async->receive_buffer_len);

}

void USART1_IRQHandler(void) {
  HAL_UART_IRQHandler(&m_uart[0]);
}

void USART2_IRQHandler(void) {
  HAL_UART_IRQHandler(&m_uart[1]);
}

void USART3_IRQHandler(void) {
  HAL_UART_IRQHandler(&m_uart[2]);
}

void test_uart_send(char* str, size_t len) {
	HAL_UART_Transmit(__hacky_uart_instance->drv_specific, str, len, 1000);
}
