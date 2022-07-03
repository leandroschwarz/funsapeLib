
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "funsapeLibGlobalDefines.hpp"
#include "util/funsapeLibCircularBuffer.hpp"
#include "STM32_TFT_8bit.h"

#define TFT_GREY						GRAY
#define TFT_BLACK						BLACK
#define TFT_RED							RED
#define TFT_CYAN						CYAN
#define TFT_YELLOW						YELLOW
#define TFT_BLUE						BLUE

#define _drawString(buf, x, y, size)	{display.setTextSize(size);display.setCursor(x, y);display.print(buf);}

void SystemClock_Config(void);

typedef union systemFlags_f {
	struct {
		bool		updateDisplay		: 1;
		uint32_t	unusedFlags			: 31;
	};
	uint32_t		allFlags;
} systemFlags_f;

#define ADC_DMA_FULL					(10 * 5)
#define ADC_DMA_HALF					(ADC_DMA_FULL / 2)

systemFlags_f systemFlags;
CircularBuffer<uint8_t> tftCircBuffer(400, true);
CircularBuffer<uint16_t> adcCircBufferCh1(400, true);
CircularBuffer<uint16_t> adcCircBufferCh2(400, true);
CircularBuffer<uint16_t> adcCircBufferCh3(400, true);
CircularBuffer<uint16_t> adcCircBufferCh4(400, true);
CircularBuffer<uint16_t> adcCircBufferCh5(400, true);
uint16_t adcDmaBuffer[ADC_DMA_FULL];
uint16_t frameCount = 0;

int main(void)
{
	// -------------------------------------------------------------------------
	// HAL initialization ------------------------------------------------------

	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USB_DEVICE_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_CRC_Init();
	MX_I2C1_Init();
	MX_I2C2_Init();
	MX_SPI1_Init();
	MX_SPI2_Init();
	MX_SPI3_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();

	systemFlags.allFlags = 0;

	// -------------------------------------------------------------------------
	// TFT display configuration -----------------------------------------------

	STM32_TFT_8bit display;

	display.begin(0x9325);

	// Clear the screen and draw the frame
	display.setRotation(3);
	display.setFont();
	display.setTextSize(1);

	display.fillScreen(TFT_BLACK);
	display.fillRect(0, 0, 319, 14, TFT_RED);
	display.fillRect(0, 226, 319, 14, TFT_GREY);

	display.setTextColor(TFT_BLACK, TFT_RED);
	display.setCursor(100, 4);
	display.print("* STM32_TFT_8bit *");
	display.setTextColor(TFT_YELLOW, TFT_GREY);
	display.setCursor(100, 230);
	display.print("Adapted by nopnop2002");

	display.drawRect(0, 14, 319, 211, TFT_BLUE);

	if(tftCircBuffer.getLastError() != Error::NONE) {
		ledOn();
	}
	if(adcCircBufferCh1.getLastError() != Error::NONE) {
		ledOn();
	}
	if(adcCircBufferCh2.getLastError() != Error::NONE) {
		ledOn();
	}
	if(adcCircBufferCh3.getLastError() != Error::NONE) {
		ledOn();
	}
	if(adcCircBufferCh4.getLastError() != Error::NONE) {
		ledOn();
	}
	if(adcCircBufferCh5.getLastError() != Error::NONE) {
		ledOn();
	}

	uint8_t tftAuxBuffer[317];
	uint8_t tftCurrentScreen[317];

	for(uint16_t i = 1; i < 318; i++) {
		tftCircBuffer.push(119 + (sin(((i * 1.13) * 3.14) / 180) * 95));
	}
	tftCircBuffer.popBuffer(tftAuxBuffer, 317, true);
	memcpy(tftCurrentScreen, tftAuxBuffer, 317);

	// -------------------------------------------------------------------------
	// TIMER configuration -----------------------------------------------------

	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);

	// -------------------------------------------------------------------------
	// ADC configuration -------------------------------------------------------

	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcDmaBuffer, ADC_DMA_FULL);

	while(1) {
		if(systemFlags.updateDisplay) {
			setDebug7();
			setDebug3();
			_drawString(++frameCount, 1, 20, 1);
			tftCircBuffer.popBuffer(tftAuxBuffer, 317, false);
			for(uint16_t i = 0; i < 317; i++) {
				// Clear trace old pixel
				display.drawPixel(i + 1, tftCurrentScreen[i], TFT_BLACK);
				// Paint trace new pixel
				display.drawPixel(i + 1, tftAuxBuffer[i], TFT_CYAN);
				// Register pixel
				tftCurrentScreen[i] = tftAuxBuffer[i];
			}
			clrDebug3();
			systemFlags.updateDisplay = false;
			clrDebug7();
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	setDebug7();
	if(htim->Instance == TIM1) {
		cplDebug0();
	} else if(htim->Instance == TIM2) {
		cplDebug1();
		systemFlags.updateDisplay = true;
	} else if(htim->Instance == TIM3) {
		cplDebug2();
	}
	clrDebug7();
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	setDebug7();
	setDebug4();
	for(uint16_t i = 0; i < ADC_DMA_HALF; i = i + 5) {
		adcCircBufferCh1.push(adcDmaBuffer[i]);
		tftCircBuffer.push(200 - (uint8_t)(adcDmaBuffer[i] / 32));
		adcCircBufferCh2.push(adcDmaBuffer[i + 1]);
		adcCircBufferCh3.push(adcDmaBuffer[i + 2]);
		adcCircBufferCh4.push(adcDmaBuffer[i + 3]);
		adcCircBufferCh5.push(adcDmaBuffer[i + 4]);
	}
	clrDebug4();
	clrDebug7();
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	setDebug7();
	setDebug5();
	for(uint16_t i = 0; i < ADC_DMA_HALF; i = i + 5) {
		adcCircBufferCh1.push(adcDmaBuffer[i + ADC_DMA_HALF]);
		tftCircBuffer.push(200 - (uint8_t)(adcDmaBuffer[i + ADC_DMA_HALF] / 32));
		adcCircBufferCh2.push(adcDmaBuffer[i + ADC_DMA_HALF + 1]);
		adcCircBufferCh3.push(adcDmaBuffer[i + ADC_DMA_HALF + 2]);
		adcCircBufferCh4.push(adcDmaBuffer[i + ADC_DMA_HALF + 3]);
		adcCircBufferCh5.push(adcDmaBuffer[i + ADC_DMA_HALF + 4]);
	}
	clrDebug5();
	clrDebug7();
}


void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_USB;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}

void Error_Handler(void)
{
}
