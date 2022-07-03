


#ifndef __MAIN_DEFS_H
#define __MAIN_DEFS_H

#include <stddef.h>
#include <stdint.h>

#define ADC0_GPIO_Port GPIOA
#define ADC0_Pin GPIO_PIN_0
#define ADC1_GPIO_Port GPIOA
#define ADC1_Pin GPIO_PIN_1
#define ADC2_GPIO_Port GPIOB
#define ADC2_Pin GPIO_PIN_0
#define ADC3_GPIO_Port GPIOC
#define ADC3_Pin GPIO_PIN_0
#define ADC4_GPIO_Port GPIOC
#define ADC4_Pin GPIO_PIN_1
#define ADC5_GPIO_Port GPIOC
#define ADC5_Pin GPIO_PIN_2
#define ADC6_GPIO_Port GPIOC
#define ADC6_Pin GPIO_PIN_3
#define BUTTON_GPIO_Port GPIOC
#define BUTTON_Pin GPIO_PIN_13
#define DAC_GPIO_Port GPIOA
#define DAC_Pin GPIO_PIN_4
#define LED_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_5
#define SPI1_SS0_GPIO_Port GPIOC
#define SPI1_SS0_Pin GPIO_PIN_9
#define SPI1_SS1_GPIO_Port GPIOC
#define SPI1_SS1_Pin GPIO_PIN_8
#define SPI1_SS2_GPIO_Port GPIOB
#define SPI1_SS2_Pin GPIO_PIN_8
#define SPI1_SS3_GPIO_Port GPIOC
#define SPI1_SS3_Pin GPIO_PIN_6
#define SPI1_SS4_GPIO_Port GPIOB
#define SPI1_SS4_Pin GPIO_PIN_9
#define SPI1_SS5_GPIO_Port GPIOC
#define SPI1_SS5_Pin GPIO_PIN_5
#define SPI1_SS6_GPIO_Port GPIOC
#define SPI1_SS6_Pin GPIO_PIN_7
// #define TCK_GPIO_Port GPIOA
// #define TCK_Pin GPIO_PIN_14
// #define TMS_GPIO_Port GPIOA
// #define TMS_Pin GPIO_PIN_13
#define USART_RX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_TX_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2

#define ledTurnOn()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define ledTurnOff()	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define ledToggle()		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5)
#define _delay_ms(x)	HAL_Delay(x)

typedef union systemFlags_f {
	struct {
		uint32_t	usart2TxBusy	: 1;
		uint32_t	triggerAlarmA	: 1;
		uint32_t	triggerAlarmB	: 1;
		uint32_t	trigger200Hz	: 1;
		uint32_t	unusedBits		: 28;
	};
	uint32_t allFlags;
} systemFlags_f;

#endif
