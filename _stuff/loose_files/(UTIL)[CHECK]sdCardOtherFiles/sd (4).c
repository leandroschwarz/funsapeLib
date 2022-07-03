
// *INDENT-OFF*

#include "sd.h"

#define __STM32L0XX_NUCLEO_BSP_VERSION_MAIN   (0x02)
#define __STM32L0XX_NUCLEO_BSP_VERSION_SUB1   (0x01)
#define __STM32L0XX_NUCLEO_BSP_VERSION_SUB2   (0x02)
#define __STM32L0XX_NUCLEO_BSP_VERSION_RC     (0x00)
#define __STM32L0XX_NUCLEO_BSP_VERSION         ((__STM32L0XX_NUCLEO_BSP_VERSION_MAIN << 24)\
                                             |(__STM32L0XX_NUCLEO_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32L0XX_NUCLEO_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32L0XX_NUCLEO_BSP_VERSION_RC))

#define SD_DUMMY_BYTE            0xFF
#define SD_NO_RESPONSE_EXPECTED  0x80


// Global variables
GPIO_TypeDef* LED_PORT[LEDn] = {LED2_GPIO_PORT};
const uint16_t LED_PIN[LEDn] = {LED2_PIN};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {USER_BUTTON_GPIO_PORT };
const uint16_t BUTTON_PIN[BUTTONn] = {USER_BUTTON_PIN };
const uint8_t BUTTON_IRQn[BUTTONn] = {USER_BUTTON_EXTI_IRQn };

#ifdef HAL_SPI_MODULE_ENABLED
	uint32_t SpixTimeout = NUCLEO_SPIx_TIMEOUT_MAX;
	static SPI_HandleTypeDef hnucleo_Spi;
#endif

#ifdef HAL_ADC_MODULE_ENABLED
	static ADC_HandleTypeDef hnucleo_Adc;
	static ADC_ChannelConfTypeDef sConfig;
#endif

#ifdef HAL_SPI_MODULE_ENABLED
	static void               SPIx_Init(void);
	static void               SPIx_Write(uint8_t Value);
	static void               SPIx_WriteData(uint8_t *DataIn, uint16_t DataLength);
	static void               SPIx_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLegnth);
	static void               SPIx_Error (void);
	static void               SPIx_MspInit(void);

	void                      SD_IO_Init(void);
	void                      SD_IO_CSState(uint8_t state);
	void                      SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength);
	void                      SD_IO_ReadData(uint8_t *DataOut, uint16_t DataLength);
	void                      SD_IO_WriteData(const uint8_t *Data, uint16_t DataLength);
	uint8_t                   SD_IO_WriteByte(uint8_t Data);
	uint8_t                   SD_IO_ReadByte(void);

	void                      LCD_IO_Init(void);
	void                      LCD_IO_WriteData(uint8_t Data);
	void                      LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size);
	void                      LCD_IO_WriteReg(uint8_t LCDReg);
	void                      LCD_Delay(uint32_t delay);
#endif

#ifdef HAL_ADC_MODULE_ENABLED
	static HAL_StatusTypeDef               ADCx_Init(void);
	static void               ADCx_DeInit(void);
	static void               ADCx_MspInit(ADC_HandleTypeDef *hadc);
	static void               ADCx_MspDeInit(ADC_HandleTypeDef *hadc);
#endif

uint32_t BSP_GetVersion(void)
{
	return __STM32L0XX_NUCLEO_BSP_VERSION;
}

void BSP_LED_Init(Led_TypeDef Led)
{
	GPIO_InitTypeDef  gpioinitstruct;

	// Enable the GPIO_LED Clock
	LEDx_GPIO_CLK_ENABLE(Led);

	// Configure the GPIO_LED pin
	gpioinitstruct.Pin = LED_PIN[Led];
	gpioinitstruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioinitstruct.Pull = GPIO_NOPULL;
	gpioinitstruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	HAL_GPIO_Init(LED_PORT[Led], &gpioinitstruct);

	// Reset PIN to switch off the LED
	HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

void BSP_LED_DeInit(Led_TypeDef Led)
{
	GPIO_InitTypeDef  gpio_init_structure;

	// Turn off LED
	HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
	// DeInit the GPIO_LED pin
	gpio_init_structure.Pin = LED_PIN[Led];
	HAL_GPIO_DeInit(LED_PORT[Led], gpio_init_structure.Pin);
}

void BSP_LED_On(Led_TypeDef Led)
{
	HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);
}

void BSP_LED_Off(Led_TypeDef Led)
{
	HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

void BSP_LED_Toggle(Led_TypeDef Led)
{
	HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
}

void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
	GPIO_InitTypeDef gpioinitstruct;

	// Enable the BUTTON Clock
	BUTTONx_GPIO_CLK_ENABLE(Button);

	gpioinitstruct.Pin = BUTTON_PIN[Button];
	gpioinitstruct.Pull = GPIO_NOPULL;
	gpioinitstruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

	if (ButtonMode == BUTTON_MODE_GPIO) {
		// Configure Button pin as input
		gpioinitstruct.Mode = GPIO_MODE_INPUT;

		HAL_GPIO_Init(BUTTON_PORT[Button], &gpioinitstruct);
	}

	if (ButtonMode == BUTTON_MODE_EXTI) {
		// Configure Button pin as input with External interrupt
		gpioinitstruct.Mode   = GPIO_MODE_IT_FALLING;
		HAL_GPIO_Init(BUTTON_PORT[Button], &gpioinitstruct);

		// Enable and set Button EXTI Interrupt to the lowest priority
		HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0);
		HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
	}
}

void BSP_PB_DeInit(Button_TypeDef Button)
{
	GPIO_InitTypeDef gpio_init_structure;

	gpio_init_structure.Pin = BUTTON_PIN[Button];
	HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
	HAL_GPIO_DeInit(BUTTON_PORT[Button], gpio_init_structure.Pin);
}

uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
	return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}


#ifdef HAL_SPI_MODULE_ENABLED

static void SPIx_MspInit(void)
{
	GPIO_InitTypeDef  gpioinitstruct = {0};

	//// Configure the GPIOs
	// Enable GPIO clock
	NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE();
	NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE();

	// Configure SPI SCK
	gpioinitstruct.Pin = NUCLEO_SPIx_SCK_PIN;
	gpioinitstruct.Mode = GPIO_MODE_AF_PP;
	gpioinitstruct.Pull  = GPIO_PULLUP;
	gpioinitstruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpioinitstruct.Alternate = NUCLEO_SPIx_SCK_AF;
	HAL_GPIO_Init(NUCLEO_SPIx_SCK_GPIO_PORT, &gpioinitstruct);

	// Configure SPI MISO and MOSI
	gpioinitstruct.Pin = NUCLEO_SPIx_MOSI_PIN;
	gpioinitstruct.Alternate = NUCLEO_SPIx_MISO_MOSI_AF;
	gpioinitstruct.Pull  = GPIO_PULLDOWN;
	HAL_GPIO_Init(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, &gpioinitstruct);

	gpioinitstruct.Pin = NUCLEO_SPIx_MISO_PIN;
	HAL_GPIO_Init(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, &gpioinitstruct);

	//// Configure the SPI peripheral
	// Enable SPI clock
	NUCLEO_SPIx_CLK_ENABLE();
}

static void SPIx_Init(void)
{
	if (HAL_SPI_GetState(&hnucleo_Spi) == HAL_SPI_STATE_RESET) {
		// SPI Config
		hnucleo_Spi.Instance = NUCLEO_SPIx;

		hnucleo_Spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
		hnucleo_Spi.Init.Direction = SPI_DIRECTION_2LINES;
		hnucleo_Spi.Init.CLKPhase = SPI_PHASE_1EDGE;
		hnucleo_Spi.Init.CLKPolarity = SPI_POLARITY_LOW;
		hnucleo_Spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
		hnucleo_Spi.Init.CRCPolynomial = 7;
		hnucleo_Spi.Init.DataSize = SPI_DATASIZE_8BIT;
		hnucleo_Spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
		hnucleo_Spi.Init.NSS = SPI_NSS_SOFT;
		hnucleo_Spi.Init.TIMode = SPI_TIMODE_DISABLE;
		hnucleo_Spi.Init.Mode = SPI_MODE_MASTER;

		SPIx_MspInit();
		HAL_SPI_Init(&hnucleo_Spi);
	}
}

static void SPIx_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_SPI_TransmitReceive(&hnucleo_Spi, (uint8_t*) DataIn, DataOut, DataLength, SpixTimeout);

	// Check the communication status
	if (status != HAL_OK) {
		// Execute user timeout callback
		SPIx_Error();
	}
}

static void SPIx_WriteData(uint8_t *DataIn, uint16_t DataLength)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_SPI_Transmit(&hnucleo_Spi, DataIn, DataLength, SpixTimeout);

	// Check the communication status
	if (status != HAL_OK) {
		// Execute user timeout callback
		SPIx_Error();
	}
}

static void SPIx_Write(uint8_t Value)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t data;

	status = HAL_SPI_TransmitReceive(&hnucleo_Spi, (uint8_t*) &Value, &data, 1, SpixTimeout);

	// Check the communication status
	if (status != HAL_OK) {
		// Execute user timeout callback
		SPIx_Error();
	}
}

static void SPIx_Error (void)
{
	// De-initialize the SPI communication BUS
	HAL_SPI_DeInit(&hnucleo_Spi);

	// Re-Initiaize the SPI communication BUS
	SPIx_Init();
}

void SD_IO_Init(void)
{
	GPIO_InitTypeDef  gpioinitstruct = {0};
	uint8_t counter = 0;

	// SD_CS_GPIO Periph clock enable
	SD_CS_GPIO_CLK_ENABLE();

	// Configure SD_CS_PIN pin: SD Card CS pin
	gpioinitstruct.Pin = SD_CS_PIN;
	gpioinitstruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioinitstruct.Pull = GPIO_PULLUP;
	gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SD_CS_GPIO_PORT, &gpioinitstruct);

	// Configure LCD_CS_PIN pin: LCD Card CS pin
	gpioinitstruct.Pin = LCD_CS_PIN;
	gpioinitstruct.Mode = GPIO_MODE_OUTPUT_PP;
	gpioinitstruct.Pull = GPIO_NOPULL;
	gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SD_CS_GPIO_PORT, &gpioinitstruct);
	LCD_CS_HIGH();
	//------------Put SD in SPI mode--------------
	// SD SPI Config
	SPIx_Init();

	// SD chip select high
	SD_CS_HIGH();

	// Send dummy byte 0xFF, 10 times with CS high
	// Rise CS and MOSI for 80 clocks cycles
	for (counter = 0; counter <= 9; counter++) {
		// Send dummy byte 0xFF
		SD_IO_WriteByte(SD_DUMMY_BYTE);
	}
}

void SD_IO_CSState(uint8_t val)
{
	if (val == 1) {
		SD_CS_HIGH();
	} else {
		SD_CS_LOW();
	}
}

void SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
	// Send the byte
	SPIx_WriteReadData(DataIn, DataOut, DataLength);
}

uint8_t SD_IO_WriteByte(uint8_t Data)
{
	uint8_t tmp;

	// Send the byte
	SPIx_WriteReadData(&Data, &tmp, 1);
	return tmp;
}

void SD_IO_ReadData(uint8_t *DataOut, uint16_t DataLength)
{
	// Send the byte
	SD_IO_WriteReadData(DataOut, DataOut, DataLength);
}

void SD_IO_WriteData(const uint8_t *Data, uint16_t DataLength)
{
	// Send the byte
	SPIx_WriteData((uint8_t *)Data, DataLength);
}

#endif /* HAL_SPI_MODULE_ENABLED */

/******************************* LINK JOYSTICK ********************************/
#ifdef HAL_ADC_MODULE_ENABLED

static void ADCx_MspInit(ADC_HandleTypeDef *hadc)
{
	GPIO_InitTypeDef  gpioinitstruct = {0};

	//// Configure the GPIOs
	// Enable GPIO clock
	NUCLEO_ADCx_GPIO_CLK_ENABLE();

	// Configure ADC1 Channel8 as analog input
	gpioinitstruct.Pin = NUCLEO_ADCx_GPIO_PIN ;
	gpioinitstruct.Mode = GPIO_MODE_ANALOG;
	gpioinitstruct.Pull   = GPIO_NOPULL;
	HAL_GPIO_Init(NUCLEO_ADCx_GPIO_PORT, &gpioinitstruct);

	//// Configure the ADC peripheral
	// Enable ADC clock
	NUCLEO_ADCx_CLK_ENABLE();
}

static void ADCx_MspDeInit(ADC_HandleTypeDef *hadc)
{
	GPIO_InitTypeDef  gpioinitstruct;

	//// DeInit the ADC peripheral
	// Disable ADC clock
	NUCLEO_ADCx_CLK_DISABLE();

	// Configure the selected ADC Channel as analog input
	gpioinitstruct.Pin = NUCLEO_ADCx_GPIO_PIN ;
	HAL_GPIO_DeInit(NUCLEO_ADCx_GPIO_PORT, gpioinitstruct.Pin);

	// Disable GPIO clock has to be done by the application
	// NUCLEO_ADCx_GPIO_CLK_DISABLE();
}

static HAL_StatusTypeDef ADCx_Init(void)
{
	// Set ADC instance
	hnucleo_Adc.Instance = NUCLEO_ADCx;

	if (HAL_ADC_GetState(&hnucleo_Adc) == HAL_ADC_STATE_RESET) {
		// ADC Config
		hnucleo_Adc.Instance = NUCLEO_ADCx;
		hnucleo_Adc.Init.OversamplingMode      = DISABLE;
		hnucleo_Adc.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2; // (must not exceed 16MHz)
		hnucleo_Adc.Init.LowPowerAutoPowerOff  = DISABLE;
		hnucleo_Adc.Init.LowPowerFrequencyMode = ENABLE;
		hnucleo_Adc.Init.LowPowerAutoWait      = ENABLE;
		hnucleo_Adc.Init.Resolution            = ADC_RESOLUTION_12B;
		hnucleo_Adc.Init.SamplingTime          = ADC_SAMPLETIME_1CYCLE_5;
		hnucleo_Adc.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
		hnucleo_Adc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
		hnucleo_Adc.Init.ContinuousConvMode    = DISABLE;
		hnucleo_Adc.Init.DiscontinuousConvMode = DISABLE;
		hnucleo_Adc.Init.ExternalTrigConv       = ADC_SOFTWARE_START;            // Trig of conversion start done manually by software, without external event
		hnucleo_Adc.Init.ExternalTrigConvEdge   = ADC_EXTERNALTRIGCONVEDGE_NONE; // Parameter discarded because trig by software start
		hnucleo_Adc.Init.EOCSelection           = ADC_EOC_SEQ_CONV;
		hnucleo_Adc.Init.DMAContinuousRequests  = DISABLE;

		// Initialize MSP related to ADC
		ADCx_MspInit(&hnucleo_Adc);

		// Initialize ADC
		if (HAL_ADC_Init(&hnucleo_Adc) != HAL_OK) {
			return HAL_ERROR;
		}

		if (HAL_ADCEx_Calibration_Start(&hnucleo_Adc, ADC_SINGLE_ENDED) != HAL_OK) {
			return HAL_ERROR;
		}
	}

	return HAL_OK;
}

static void ADCx_DeInit(void)
{
	hnucleo_Adc.Instance   = NUCLEO_ADCx;

	HAL_ADC_DeInit(&hnucleo_Adc);
	ADCx_MspDeInit(&hnucleo_Adc);
}

uint8_t BSP_JOY_Init(void)
{
	if (ADCx_Init() != HAL_OK) {
		return (uint8_t) HAL_ERROR;
	}

	// Select Channel 8 to be converted
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;

	// Return Joystick initialization status
	return (uint8_t) HAL_ADC_ConfigChannel(&hnucleo_Adc, &sConfig);
}

void BSP_JOY_DeInit(void)
{
	ADCx_DeInit();
}

JOYState_TypeDef BSP_JOY_GetState(void)
{
	JOYState_TypeDef state = JOY_NONE;
	uint16_t  keyconvertedvalue = 0;

	// Start the conversion process
	HAL_ADC_Start(&hnucleo_Adc);

	// Wait for the end of conversion
	if (HAL_ADC_PollForConversion(&hnucleo_Adc, 10) != HAL_TIMEOUT) {
		// Get the converted value of regular channel
		keyconvertedvalue = HAL_ADC_GetValue(&hnucleo_Adc);
	}

	if ((keyconvertedvalue > 2010) && (keyconvertedvalue < 2090)) {
		state = JOY_UP;
	} else if ((keyconvertedvalue > 680) && (keyconvertedvalue < 780)) {
		state = JOY_RIGHT;
	} else if ((keyconvertedvalue > 1270) && (keyconvertedvalue < 1350)) {
		state = JOY_SEL;
	} else if ((keyconvertedvalue > 50) && (keyconvertedvalue < 130)) {
		state = JOY_DOWN;
	} else if ((keyconvertedvalue > 3570) && (keyconvertedvalue < 3800)) {
		state = JOY_LEFT;
	} else {
		state = JOY_NONE;
	}

	// Return the code of the Joystick key pressed
	return state;
}
#endif

// *INDENT-ON*
