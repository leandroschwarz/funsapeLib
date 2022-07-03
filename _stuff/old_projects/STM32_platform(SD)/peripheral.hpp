


#ifndef __PERIPHERAL_H
#define __PERIPHERAL_H	1002

#include "STM32\STM32L0xx_HAL_Driver\stm32l0xx_hal.h"
#include "STM32\user_diskio.h" /* defines USER_Driver as external */


#include "ff.h"
#include "..\STM32\ff_gen_drv.h"





extern uint8_t retUSER; /* Return value for USER */
extern char USERPath[4]; /* USER logical drive path */
extern FATFS USERFatFS; /* File system object for USER logical drive */
extern FIL USERFile; /* File object for USER */

extern ADC_HandleTypeDef hadc;
extern DAC_HandleTypeDef hdac;
extern I2C_HandleTypeDef hi2c2;
extern RTC_HandleTypeDef hrtc;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim21;
extern TIM_HandleTypeDef htim22;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern void _Error_Handler(char *, int);

void MX_ADC_Init(void);
void MX_DAC_Init(void);
void MX_DMA_Init(void);
void MX_FATFS_Init(void);
void MX_GPIO_Init(void);
void MX_I2C2_Init(void);
void MX_RTC_Init(void);
void MX_SPI1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM6_Init(void);
void MX_TIM21_Init(void);
void MX_TIM22_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

#endif // __PERIPHERAL_H
