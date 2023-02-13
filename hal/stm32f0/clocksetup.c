#include <stdint.h>
#include "system.h"
#include "hal.h"

void ClockSetup_HSE8_SYS48(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};

	// Configure HSE Oscillator for use with 8 MHz Xtal
	// PLL Freq 48 MHz	( 6 * 8 = 48 )
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

	// USB clock configuration: PLL is at 48 Mhz, thus 48 = 48
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	// TODO: These are the values for STM32F1. Look up the correct values for F0
	// SYS  Clock to 48 MHz (Max 72 MHz)
	// AHB  Clock to 48 MHz (Max 72 MHz)
	// APB1 Clock to 24     (Max 36 MHz)	F0 Seens to configure ABP1 to 48
	// Flash Latency to 1. 
	// Flash Latency should be increased for each 24 MHz of clock speed. 
	// ( 48 / 24 ) - 1 = 1.
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  // Enable Power Controller clock 
  __HAL_RCC_PWR_CLK_ENABLE();
}



void ClockSetup_HSE8_SYS24(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};

	// Configure HSE Oscillator for use with 8 MHz Xtal
	// PLL Freq 24 MHz	( 3 * 8 = 24 )
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

/*
	// USB clock configuration: PLL is at 48 Mhz, thus 48 = 48
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
*/

	
	// TODO: Check flash latency, this is for 48 MHz, 
	// verify we can put flash latency to 0
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  // Enable Power Controller clock 
  __HAL_RCC_PWR_CLK_ENABLE();
}


void ClockSetup_HSE8_SYS32(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};

	// Configure HSE Oscillator for use with 8 MHz Xtal
	// PLL Freq 24 MHz	( 4 * 8 = 32 )
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

/*
	// USB clock configuration: PLL is at 48 Mhz, thus 48 = 48
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
*/

	// TODO: Check flash latency, this is for 48 MHz, 
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  // Enable Power Controller clock 
  __HAL_RCC_PWR_CLK_ENABLE();
}






void ClockSetup_HSI48_SYS48(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};

  static RCC_CRSInitTypeDef RCC_CRSInitStruct = {0};

	// Enable internal 48 MHz Oscillator
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

	// Select internal 48 MHz oscillator as clock source. 48 = 48
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	// Select the internal oscillator as system clock source and configure
	// all clocks.
	// TODO: These are the values for STM32F1. Look up the correct values for F0
	// SYS  Clock to 48 MHz (Max 72 MHz)
	// AHB  Clock to 48 MHz (Max 72 MHz)
	// APB1 Clock to 24     (Max 36 MHz)	F0 Seens to configure ABP1 to 48
	// Flash Latency to 1. 
	// Flash Latency should be increased for each 24 MHz of clock speed. 
	// ( 48 / 24 ) - 1 = 1.
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  
  // Enable the Clock Recovery System CLock
  __HAL_RCC_CRS_CLK_ENABLE();

  // Default Synchro Signal division factor (not divided) 
  RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;

  // Set the SYNCSRC[1:0] bits according to CRS_Source value 
  RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;

  // HSI48 is synchronized with USB SOF at 1KHz rate 
  RCC_CRSInitStruct.ReloadValue =  __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
  RCC_CRSInitStruct.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT;

  // Set the TRIM[5:0] to the default value
  RCC_CRSInitStruct.HSI48CalibrationValue = 0x20;

  // Start automatic synchronization 
  HAL_RCCEx_CRSConfig (&RCC_CRSInitStruct);

  // Enable Power Controller clock 
  __HAL_RCC_PWR_CLK_ENABLE();
}



void ClockSetup_HSI8_SYS48(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};

	// Configure HSI Oscillator 
	// PLL Freq 48 MHz	( 6 * 8 = 48 )
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

	// USB clock configuration: PLL is at 48 Mhz, thus 48 = 48
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	// TODO: These are the values for STM32F1. Look up the correct values for F0
	// SYS  Clock to 48 MHz (Max 72 MHz)
	// AHB  Clock to 48 MHz (Max 72 MHz)
	// APB1 Clock to 24     (Max 36 MHz)	F0 Seens to configure ABP1 to 48
	// Flash Latency to 1. 
	// Flash Latency should be increased for each 24 MHz of clock speed. 
	// ( 48 / 24 ) - 1 = 1.
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  // Enable Power Controller clock 
  __HAL_RCC_PWR_CLK_ENABLE();
}
