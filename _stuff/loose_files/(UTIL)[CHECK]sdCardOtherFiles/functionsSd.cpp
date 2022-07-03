

#include "functionsSd.h"
#include "sd_diskio.h"
#include "ff.h"


uint32_t SpixTimeout = NUCLEO_SPIx_TIMEOUT_MAX; /*<! Value of Timeout when SPI communication fails */
SPI_HandleTypeDef hnucleo_Spi;

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */



/**
  * @brief  Initialize the SD Card and put it into StandBy State (Ready for 
  *         data transfer).
  * @retval None
  */
void SD_IO_Init(void)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};
  uint8_t counter = 0;

  /* SD_CS_GPIO Periph clock enable */
  SD_CS_GPIO_CLK_ENABLE();

  /* Configure SD_CS_PIN pin: SD Card CS pin */
  gpioinitstruct.Pin = SD_CS_PIN;
  gpioinitstruct.Mode = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull = GPIO_PULLUP;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SD_CS_GPIO_PORT, &gpioinitstruct);

  /* Configure LCD_CS_PIN pin: LCD Card CS pin */
//  gpioinitstruct.Pin = LCD_CS_PIN;
//  gpioinitstruct.Mode = GPIO_MODE_OUTPUT_PP;
//  gpioinitstruct.Pull = GPIO_NOPULL;
//  gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(SD_CS_GPIO_PORT, &gpioinitstruct);
//  LCD_CS_HIGH();
  /*------------Put SD in SPI mode--------------*/
  /* SD SPI Config */
  SPIx_Init();
  
  /* SD chip select high */
  SD_CS_HIGH();
  
  /* Send dummy byte 0xFF, 10 times with CS high */
  /* Rise CS and MOSI for 80 clocks cycles */
  for (counter = 0; counter <= 9; counter++)
  {
    /* Send dummy byte 0xFF */
    SD_IO_WriteByte(SD_DUMMY_BYTE);
  }
}

/**
  * @brief  Set the SD_CS pin.
  * @param  val: pin value.
  * @retval None
  */
void SD_IO_CSState(uint8_t val)
{
  if(val == 1) 
  {
    SD_CS_HIGH();
  }
  else
  {
    SD_CS_LOW();
  }
}

/**
  * @brief  Write byte(s) on the SD
  * @param  DataIn: Pointer to data buffer to write
  * @param  DataOut: Pointer to data buffer for read data
  * @param  DataLength: number of bytes to write
  * @retval None
  */
void SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
  /* Send the byte */
  SPIx_WriteReadData(DataIn, DataOut, DataLength);
}

/**
  * @brief  Write a byte on the SD.
  * @param  Data: byte to send.
  * @retval Data written
  */
uint8_t SD_IO_WriteByte(uint8_t Data)
{
  uint8_t tmp;

  /* Send the byte */
  SPIx_WriteReadData(&Data,&tmp,1);
  return tmp;
}

/**
  * @brief  Write an amount of data on the SD.
  * @param  DataOut: byte to send.
  * @param  DataLength: number of bytes to write
  * @retval none
  */
void SD_IO_ReadData(uint8_t *DataOut, uint16_t DataLength)
{
  /* Send the byte */
  SD_IO_WriteReadData(DataOut, DataOut, DataLength);
  }  
 
/**
  * @brief  Write an amount of data on the SD.
  * @param  Data: byte to send.
  * @param  DataLength: number of bytes to write
  * @retval none
  */
void SD_IO_WriteData(const uint8_t *Data, uint16_t DataLength)
{
  /* Send the byte */
  SPIx_WriteData((uint8_t *)Data, DataLength);
}













/******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/
/**
  * @brief  Initialize SPI MSP.
  * @retval None
  */
void SPIx_MspInit(void)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};
  
  /*** Configure the GPIOs ***/  
  /* Enable GPIO clock */
  NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE();
  NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE();
  
  /* Configure SPI SCK */
  gpioinitstruct.Pin = NUCLEO_SPIx_SCK_PIN;
  gpioinitstruct.Mode = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull  = GPIO_PULLUP;
  gpioinitstruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioinitstruct.Alternate = NUCLEO_SPIx_SCK_AF;
  HAL_GPIO_Init(NUCLEO_SPIx_SCK_GPIO_PORT, &gpioinitstruct);

  /* Configure SPI MISO and MOSI */ 
  gpioinitstruct.Pin = NUCLEO_SPIx_MOSI_PIN;
  gpioinitstruct.Alternate = NUCLEO_SPIx_MISO_MOSI_AF;
  gpioinitstruct.Pull  = GPIO_PULLDOWN;
  HAL_GPIO_Init(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, &gpioinitstruct);
  
  gpioinitstruct.Pin = NUCLEO_SPIx_MISO_PIN;
  HAL_GPIO_Init(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, &gpioinitstruct);

  /*** Configure the SPI peripheral ***/ 
  /* Enable SPI clock */
  NUCLEO_SPIx_CLK_ENABLE();
}

/**
  * @brief  Initialize SPI HAL.
  * @retval None
  */
void SPIx_Init(void)
{
  if(HAL_SPI_GetState(&hnucleo_Spi) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    hnucleo_Spi.Instance = NUCLEO_SPIx;
      /* SPI baudrate is set to 8 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 32/4 = 8 MHz) 
       to verify these constraints:
          - ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
            Since the provided driver doesn't use read capability from LCD, only constraint 
            on write baudrate is considered.
          - SD card SPI interface max baudrate is 25MHz for write/read
          - PCLK2 max frequency is 32 MHz 
       */ 
    hnucleo_Spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
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

/**
  * @brief  SPI Write a byte to device
  * @param  DataIn: value to be written
  * @param  DataOut: read value
  * @param  DataLength: number of bytes to write
  * @retval None
  */
void SPIx_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_TransmitReceive(&hnucleo_Spi, (uint8_t*) DataIn, DataOut, DataLength, SpixTimeout);
    
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/**
  * @brief  SPI Write an amount of data to device
  * @param  DataIn: value to be written
  * @param  DataLength: number of bytes to write
  * @retval None
  */
void SPIx_WriteData(uint8_t *DataIn, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_Transmit(&hnucleo_Spi, DataIn, DataLength, SpixTimeout);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/**
  * @brief  SPI Write a byte to device
  * @param  Value: value to be written
  * @retval None
  */
void SPIx_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t data;

  status = HAL_SPI_TransmitReceive(&hnucleo_Spi, (uint8_t*) &Value, &data, 1, SpixTimeout);
    
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/**
  * @brief  SPI error treatment function
  * @retval None
  */
void SPIx_Error (void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&hnucleo_Spi);
  
  /* Re-Initiaize the SPI communication BUS */
  SPIx_Init();
}

DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */  
}

void MX_FATFS_Init(void) 
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&SD_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */     
  /* USER CODE END Init */
}
