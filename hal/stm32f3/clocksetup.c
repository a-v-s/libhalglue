/*

 File: 		clocksetup.c
 Author:	André van Schoubroeck
 License:	MIT


 MIT License

 Copyright (c) 2020, 2022 		André van Schoubroeck <andre@blaatschaap.be>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 */

//----------------------------------------------------------------------------
// Clock Setup for STM32F3xxx
//
// Clock setup for F3x is a copy of clock setup of F1x
// With only some names different
//
// UsbClockSelection -> USBClockSelection
// RCC_PLLSOURCE_HSI_DIV2 -> RCC_PLLSOURCE_HSI
//
// Added  __HAL_RCC_PWR_CLK_ENABLE(); as the example did so.
//
// This code needs verification as to check for any more differences
// between F1x and F3x clocking hardware. On first glance the speed
// limits are the same. TODO: Verify this and update comments if needed.
//
// ClockSetup_HSE8_SYS72 : 8 MHZ Xtal, 72 MHz clock speed, USB capable
// ClockSetup_HSE8_SYS48 : 8 MHZ Xtal, 48 MHz clock speed, USB capable
// ClockSetup_HSI_SYS48  : No Xtal, 48 MHz clock speed, unofficial USB capable
//------------------------------------------------------------------------------

#include "stm32f3xx.h"

// Configure for external HSE 8 Mhz Xtal, System speed to 72 MHz
void ClockSetup_HSE8_SYS72(void) {
	RCC_ClkInitTypeDef clkinitstruct = { 0 };
	RCC_OscInitTypeDef oscinitstruct = { 0 };
	RCC_PeriphCLKInitTypeDef rccperiphclkinit = { 0 };

	// Configure HSE Oscillator for use with 8 MHz Xtal
	// PLL Freq 72 MHz	( 9 * 8 = 72 )
	oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	oscinitstruct.HSEState = RCC_HSE_ON;
	oscinitstruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL9;
	oscinitstruct.PLL.PLLState = RCC_PLL_ON;
	oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	HAL_RCC_OscConfig(&oscinitstruct);

	// USB clock configuration: 72 / 1.5 = 48
	rccperiphclkinit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	rccperiphclkinit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	HAL_RCCEx_PeriphCLKConfig(&rccperiphclkinit);

	// SYS  Clock to 72 MHz  (Max 72 MHz)
	// AHB  Clock to 72 MHz  (Max 72 MHz)
	// APB1 Clock to 36      (Max 36 MHz)
	// APB2 Clock to 72      (Max 72 MHz)
	// Flash Latency to 2.
	// Flash Latency should be increased for each 24 MHz of clock speed.
	// ( 72 / 24 ) - 1 = 2.
	clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;
	clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2);
	 __HAL_RCC_PWR_CLK_ENABLE();
	SystemCoreClockUpdate();
}

//------------------------------------------------------------------------------

// Configure for external HSE 8 Mhz Xtal, System speed to 48 MHz
void ClockSetup_HSE8_SYS48(void) {
	RCC_ClkInitTypeDef clkinitstruct = { 0 };
	RCC_OscInitTypeDef oscinitstruct = { 0 };
	RCC_PeriphCLKInitTypeDef rccperiphclkinit = { 0 };

	// Configure HSE Oscillator for use with 8 MHz Xtal
	// PLL Freq 72 MHz	( 6 * 8 = 48 )
	// PLL Freq 48 MHz
	oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	oscinitstruct.HSEState = RCC_HSE_ON;
	oscinitstruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL6;
	oscinitstruct.PLL.PLLState = RCC_PLL_ON;
	oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	HAL_RCC_OscConfig(&oscinitstruct);

	// USB clock configuration: 48 = 48
	rccperiphclkinit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	rccperiphclkinit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
	HAL_RCCEx_PeriphCLKConfig(&rccperiphclkinit);

	// SYS  Clock to 48 MHz (Max 72 MHz)
	// AHB  Clock to 48 MHz (Max 72 MHz)
	// APB1 Clock to 24     (Max 36 MHz)
	// APB2 Clock to 48     (Max 72 MHz)
	// Flash Latency to 1.
	// Flash Latency should be increased for each 24 MHz of clock speed.
	// ( 48 / 24 ) - 1 = 1.
	clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;
	clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_1);
	 __HAL_RCC_PWR_CLK_ENABLE();
	SystemCoreClockUpdate();
}
//------------------------------------------------------------------------------

// Configure for internal HSI 8 Mhz Xtal, System speed to 48 MHz
void ClockSetup_HSI_SYS48(void) {
	RCC_ClkInitTypeDef clkinitstruct = { 0 };
	RCC_OscInitTypeDef oscinitstruct = { 0 };
	RCC_PeriphCLKInitTypeDef rccperiphclkinit = { 0 };

	// Configure HSI Internal RC Oscillator. It runs at 8 MHz, but when used
	// as an PLL input, it is divided by 2, so this gives 4 MHz
	// 12 * 4 = 48
	oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	oscinitstruct.HSIState = RCC_HSI_ON;
	oscinitstruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL12;
	oscinitstruct.PLL.PLLState = RCC_PLL_ON;
	oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	HAL_RCC_OscConfig(&oscinitstruct);

	// USB clock configuration: 48 = 48
	rccperiphclkinit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	rccperiphclkinit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
	HAL_RCCEx_PeriphCLKConfig(&rccperiphclkinit);

	// SYS  Clock to 48 MHz (Max 72 MHz)
	// AHB  Clock to 48 MHz (Max 72 MHz)
	// APB1 Clock to 24     (Max 36 MHz)
	// APB2 Clock to 48     (Max 72 MHz)
	// Flash Latency to 1.
	// Flash Latency should be increased for each 24 MHz of clock speed.
	// ( 48 / 24 ) - 1 = 1.
	clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;
	clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_1);
	 __HAL_RCC_PWR_CLK_ENABLE();
	SystemCoreClockUpdate();
}


