
#include "main.h"
#include "usart.h"
#include "gpio.h"

#include <Adafruit_GFX.h>
#include "STM32_TFT_8bit.h"

#define TFT_GREY						GRAY
#define TFT_BLACK						BLACK
#define TFT_RED							RED
#define TFT_CYAN						CYAN
#define TFT_YELLOW						YELLOW
#define TFT_BLUE						BLUE

void SystemClock_Config(void);
void _drawString(char *buf, int16_t x, int16_t y, int16_t size);

STM32_TFT_8bit myGLCD;
int sbuf[318];
int cbuf[318];

int main()
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
//	MX_USART1_UART_Init();

	while(1) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(300);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(300);
	}

	ledOn();

	char uartTxBuffer[50];
	strcpy(uartTxBuffer, "UART 1 is working!\r");
	HAL_UART_Transmit(&huart1, (uint8_t *)uartTxBuffer, strlen(uartTxBuffer), 10);
	// uint32_t ID = myGLCD.readID();
	// Serial.print("Device ID: 0x"); Serial.println(ID, HEX);
	myGLCD.begin(0x9325);

	uint32_t width = myGLCD.width();
	// Serial.print("Width: "); Serial.println(width);
	uint32_t height = myGLCD.height();
	// Serial.print("Height: "); Serial.println(height);

	int buf[318];

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

	return 0;
}

void _drawString(char *buf, int16_t x, int16_t y, int16_t size)
{
	myGLCD.setTextSize(size);
	myGLCD.setCursor(x, y);
	myGLCD.print(buf);
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}

void Error_Handler(void)
{
}
