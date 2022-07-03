/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <Adafruit_GFX.h>
#include "STM32_TFT_8bit.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


#define _drawString(buf, x, y, size)	{myGLCD.setTextSize(size);myGLCD.setCursor(x, y);myGLCD.print(buf);}


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

#define TFT_GREY						GRAY
#define TFT_BLACK						BLACK
#define TFT_RED							RED
#define TFT_CYAN						CYAN
#define TFT_YELLOW						YELLOW
#define TFT_BLUE						BLUE

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */


	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */

	char uartTxBuffer[50];
	strcpy(uartTxBuffer, "UART 1 is working!\r");
	HAL_UART_Transmit(&huart1, (uint8_t *)uartTxBuffer, strlen(uartTxBuffer), 10);



	STM32_TFT_8bit myGLCD;
	int sbuf[318];
	int cbuf[318];

	myGLCD.begin(0x9325);

	// Clear the screen and draw the frame
	myGLCD.setRotation(3);
	myGLCD.setFont();
	myGLCD.setTextSize(1);

	myGLCD.fillScreen(TFT_BLACK);
	myGLCD.fillRect(0, 0, 319, 14, TFT_RED);
	myGLCD.fillRect(0, 226, 319, 14, TFT_GREY);

	myGLCD.setTextColor(TFT_BLACK, TFT_RED);
	myGLCD.setCursor(100, 4);
	myGLCD.print("* STM32_TFT_8bit *");
	myGLCD.setTextColor(TFT_YELLOW, TFT_GREY);
	myGLCD.setCursor(100, 230);
	myGLCD.print("Adapted by nopnop2002");

	myGLCD.drawRect(0, 14, 319, 211, TFT_BLUE);

	// Draw crosshairs
	myGLCD.drawLine(159, 15, 159, 224, TFT_BLUE);
	myGLCD.drawLine(1, 119, 318, 119, TFT_BLUE);
	for(int i = 9; i < 310; i += 10) {
		myGLCD.drawLine(i, 117, i, 121, TFT_BLUE);
	}
	for(int i = 19; i < 220; i += 10) {
		myGLCD.drawLine(157, i, 161, i, TFT_BLUE);
	}

	// Draw sin-lines, cos-lines
	myGLCD.setTextColor(TFT_CYAN);
	_drawString("Sin", 5, 15, 2);
	for(int i = 1; i < 318; i = i + 2) {
		sbuf[i - 1] = 119 + (sin(((i * 1.13) * 3.14) / 180) * 95);
		myGLCD.drawPixel(i, sbuf[i - 1], TFT_CYAN);
	}
	myGLCD.setTextColor(TFT_RED);
	_drawString("Cos", 5, 30, 2);
	for(int i = 1; i < 318; i = i + 2) {
		cbuf[i - 1] = 119 + (cos(((i * 1.13) * 3.14) / 180) * 95);
		myGLCD.drawPixel(i, cbuf[i - 1], TFT_YELLOW);
	}

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while(1) {

		static int delta = 0;

		// Erase sin-lines, cos-lines
		for(int i = 1; i < 318; i = i + 2) {
			myGLCD.drawPixel(i, sbuf[i - 1], TFT_BLACK);
		}
		for(int i = 1; i < 318; i = i + 2) {
			myGLCD.drawPixel(i, cbuf[i - 1], TFT_BLACK);
		}

		// Draw sin-lines, cos-lines
		delta++;
		if(delta == 318) {
			delta = 0;
		}
		for(int i = 1; i < 318; i = i + 2) {
			sbuf[i - 1] = 119 + (sin((((i + delta) * 1.13) * 3.14) / 180) * 95);
			myGLCD.drawPixel(i, sbuf[i - 1], TFT_CYAN);
		}
		for(int i = 1; i < 318; i = i + 2) {
			cbuf[i - 1] = 119 + (cos((((i + delta) * 1.13) * 3.14) / 180) * 95);
			myGLCD.drawPixel(i, cbuf[i - 1], TFT_YELLOW);
		}

		myGLCD.setTextColor(TFT_CYAN);
		//  myGLCD.drawString("Sin", 5, 15,2);
		_drawString("Sin", 5, 15, 2);
		myGLCD.setTextColor(TFT_YELLOW);
		//  myGLCD.drawString("Cos", 5, 30,2);
		_drawString("Cos", 5, 30, 2);
		myGLCD.drawLine(159, 15, 159, 224, TFT_BLUE);
		myGLCD.drawLine(1, 119, 318, 119, TFT_BLUE);
	}
	/* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Initializes the CPU, AHB and APB busses clocks
	*/
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
	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
