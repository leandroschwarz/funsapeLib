/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibSpi.hpp"
#include "funsapeLibGpio.hpp"
#include "devices/funsapeLibMpu.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef union {
    uint32_t allFlags;
    struct {
        bool_t ledReady                 : 1;
        uint32_t unusedFlags            : 31;
    };
} systemFlags_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

systemFlags_t systemFlags;
Gpio led(NLED_GPIO_Port, NLED_Pin);
Gpio accelCs[4];
Spi spi2;
Error funsapeError;
cuint8_t thisFileErrorCode = 2;
Mpu accelDevice[4];

GPIO_TypeDef *constAccelCsPorts[] = {DEBUG0_GPIO_Port, DEBUG1_GPIO_Port, DEBUG2_GPIO_Port, DEBUG3_GPIO_Port};
cuint16_t constAccelCsPins[] = {DEBUG0_Pin, DEBUG1_Pin, DEBUG2_Pin, DEBUG3_Pin};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void showError(cuint8_t errorCode_p = thisFileErrorCode, GPIO_TypeDef *ledPort_p = NLED_GPIO_Port,
        cuint16_t ledPin_p = NLED_Pin);

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
    MX_SPI2_Init();
    /* USER CODE BEGIN 2 */

    // =========================================================================
    //     <<<<<<<<<<<<<<<<    INITIAL SETUP - BEGIN     >>>>>>>>>>>>>>>>     //
    // =========================================================================

    // -------------------------------------------------------------------------
    // Setting up debug module -------------------------------------------------
    do {
        debug.init(&huart1);
        debug.hideMarks(true);
        debug.hideSuccess(true);
        debug.setTimeOut(100);
        debug.stopOnError(false);
        debug.debugGpio(true);
        debug.debugSpi(true);
        debug.debugUsart(true);
        char errorMessage[] = "[  OK  ] Debug module configured!\r";
        debug.printMessage(errorMessage);
    } while (0);

    // -------------------------------------------------------------------------
    // Setting up GPIOs --------------------------------------------------------
    do {
        // LED
        do {
            // Checking object creation <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            funsapeError = led.getLastError();
            if (funsapeError != Error::NONE) {
                debugMessage(funsapeError);
                showError();
            }
            char errorMessage[] = "[  OK  ] LED pin configured!\r";
            debug.printMessage(errorMessage);
        } while (0);
        // AccelCs
        char errorMessage[] = "[  OK  ] GpioX pin configured!\r";
        for (uint8_t i = 0; i < 4; i++) {
            // Checking object creation <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            funsapeError = accelCs[i].getLastError();
            if (funsapeError != Error::NONE) {
                debugMessage(funsapeError);
                showError();
            }
            // Initialize objects <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            if (!accelCs[i].init(constAccelCsPorts[i], constAccelCsPins[i])) {
                debugMessage(accelCs[i].getLastError());
                showError();
            }
            errorMessage[13] = '0' + i;
            debug.printMessage(errorMessage);
        }
    } while (0);

    // -------------------------------------------------------------------------
    // Setting up SPI module ---------------------------------------------------
    do {
        // Checking object creation <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        funsapeError = spi2.getLastError();
        if (funsapeError != Error::NONE) {
            debugMessage(funsapeError);
            showError();
        }
        if (!spi2.init(&hspi2)) {
            debugMessage(spi2.getLastError());
            showError();
        }
        char errorMessage[] = "[  OK  ] SPI module configured!\r";
        debug.printMessage(errorMessage);
    } while (0);

    // -------------------------------------------------------------------------
    // Setting up Mpu-9250 module ----------------------------------------------
    do {
        char errorMessage[] = "[  OK  ] MpuX pin configured!\r";
        for (uint8_t i = 0; i < 4; i++) {
            // Checking object creation <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            funsapeError = accelDevice[i].getLastError();
            if (funsapeError != Error::NONE) {
                debugMessage(funsapeError);
                showError();
            }
            // Initialize objects <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            if (!accelDevice[i].init(&spi2, &accelCs[i])) {
                debugMessage(accelDevice[i].getLastError());
                showError();
            }
            errorMessage[13] = '0' + i;
            debug.printMessage(errorMessage);
        }
    } while (0);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin);
        delayMs(50);
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
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

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
    /** Enables the Clock Security System
    */
    HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */

void showError(cuint8_t errorCode_p, GPIO_TypeDef *ledPort_p, cuint16_t ledPin_p)
{
    // Turns off LED
    setIoPin(ledPort_p, ledPin_p);

    // Loops
    while (1) {
        // Pause for a while
        delayMs(800);
        // Send pulse train
        for (uint8_t i = 0; i < errorCode_p; i++) {
            clrIoPin(ledPort_p, ledPin_p);
            delayMs(200);
            setIoPin(ledPort_p, ledPin_p);
            delayMs(200);
        }
    }

    return;
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
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
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

