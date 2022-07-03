/* =============================================================================
 * Project:			Dry HAL Library
 * File name:		dryHalSpi.cpp
 * Module:			SPI Peripheral Module for Dry HAL Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "dryHalSpi.hpp"
#ifndef __DRY_HAL_SPI_HPP
#	error	[dryHalSpi.cpp] Error 1 - Header file (dryHalSpi.hpp) is missing or corrupted!
#elif __DRY_HAL_SPI_HPP != __BUILD_TO_REPLACE__
#	error	[dryHalSpi.cpp] Error 13 - Build mismatch between source (dryHalSpi.cpp) and header (dryHalSpi.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

// NONE

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// NONE

// =============================================================================
// Static functions declarations
// =============================================================================

static HAL_StatusTypeDef SPI_EndRxTxTransaction(SPI_HandleTypeDef *hspi, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus State, uint32_t Timeout, uint32_t Tickstart);
void dryHalSpiMspInit(SPI_HandleTypeDef *spiHandle);

// =============================================================================
// General public functions definitions
// =============================================================================

HAL_StatusTypeDef dryHalSpiInit(SPI_HandleTypeDef *hspi)
{
	/* Check the SPI handle allocation */
	if(hspi == NULL) {
		return HAL_ERROR;
	}

	/* Check the parameters */
	assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));
	assert_param(IS_SPI_MODE(hspi->Init.Mode));
	assert_param(IS_SPI_DIRECTION(hspi->Init.Direction));
	assert_param(IS_SPI_DATASIZE(hspi->Init.DataSize));
	assert_param(IS_SPI_NSS(hspi->Init.NSS));
	assert_param(IS_SPI_BAUDRATE_PRESCALER(hspi->Init.BaudRatePrescaler));
	assert_param(IS_SPI_FIRST_BIT(hspi->Init.FirstBit));
	/* TI mode is not supported on this device.
	   TIMode parameter is mandatory equal to SPI_TIMODE_DISABLE */
	assert_param(IS_SPI_TIMODE(hspi->Init.TIMode));
	if(hspi->Init.TIMode == SPI_TIMODE_DISABLE) {
		assert_param(IS_SPI_CPOL(hspi->Init.CLKPolarity));
		assert_param(IS_SPI_CPHA(hspi->Init.CLKPhase));
	}
#if (USE_SPI_CRC != 0U)
	assert_param(IS_SPI_CRC_CALCULATION(hspi->Init.CRCCalculation));
	if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE) {
		assert_param(IS_SPI_CRC_POLYNOMIAL(hspi->Init.CRCPolynomial));
	}
#else
	hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
#endif /* USE_SPI_CRC */

	if(hspi->State == HAL_SPI_STATE_RESET) {
		/* Allocate lock resource and initialize it */
		hspi->Lock = HAL_UNLOCKED;

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
		/* Init the SPI Callback settings */
		hspi->TxCpltCallback       = HAL_SPI_TxCpltCallback;       /* Legacy weak TxCpltCallback       */
		hspi->RxCpltCallback       = HAL_SPI_RxCpltCallback;       /* Legacy weak RxCpltCallback       */
		hspi->TxRxCpltCallback     = HAL_SPI_TxRxCpltCallback;     /* Legacy weak TxRxCpltCallback     */
		hspi->TxHalfCpltCallback   = HAL_SPI_TxHalfCpltCallback;   /* Legacy weak TxHalfCpltCallback   */
		hspi->RxHalfCpltCallback   = HAL_SPI_RxHalfCpltCallback;   /* Legacy weak RxHalfCpltCallback   */
		hspi->TxRxHalfCpltCallback = HAL_SPI_TxRxHalfCpltCallback; /* Legacy weak TxRxHalfCpltCallback */
		hspi->ErrorCallback        = HAL_SPI_ErrorCallback;        /* Legacy weak ErrorCallback        */
		hspi->AbortCpltCallback    = HAL_SPI_AbortCpltCallback;    /* Legacy weak AbortCpltCallback    */

		if(hspi->MspInitCallback == NULL) {
			hspi->MspInitCallback = HAL_SPI_MspInit; /* Legacy weak MspInit  */
		}

		/* Init the low level hardware : GPIO, CLOCK, NVIC... */
		hspi->MspInitCallback(hspi);
#else
		/* Init the low level hardware : GPIO, CLOCK, NVIC... */
		dryHalSpiMspInit(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
	}

	hspi->State = HAL_SPI_STATE_BUSY;

	/* Disable the selected SPI peripheral */
	__HAL_SPI_DISABLE(hspi);

	/*----------------------- SPIx CR1 & CR2 Configuration ---------------------*/
	/* Configure : SPI Mode, Communication Mode, Data size, Clock polarity and phase, NSS management,
	Communication speed, First bit and CRC calculation state */
	WRITE_REG(hspi->Instance->CR1, (hspi->Init.Mode | hspi->Init.Direction | hspi->Init.DataSize |
					hspi->Init.CLKPolarity | hspi->Init.CLKPhase | (hspi->Init.NSS & SPI_CR1_SSM) |
					hspi->Init.BaudRatePrescaler | hspi->Init.FirstBit  | hspi->Init.CRCCalculation));

	/* Configure : NSS management */
	WRITE_REG(hspi->Instance->CR2, ((hspi->Init.NSS >> 16U) & SPI_CR2_SSOE));

#if (USE_SPI_CRC != 0U)
	/*---------------------------- SPIx CRCPOLY Configuration ------------------*/
	/* Configure : CRC Polynomial */
	if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE) {
		WRITE_REG(hspi->Instance->CRCPR, hspi->Init.CRCPolynomial);
	}
#endif /* USE_SPI_CRC */

#if defined(SPI_I2SCFGR_I2SMOD)
	/* Activate the SPI mode (Make sure that I2SMOD bit in I2SCFGR register is reset) */
	CLEAR_BIT(hspi->Instance->I2SCFGR, SPI_I2SCFGR_I2SMOD);
#endif /* SPI_I2SCFGR_I2SMOD */

	hspi->ErrorCode = HAL_SPI_ERROR_NONE;
	hspi->State     = HAL_SPI_STATE_READY;

	return HAL_OK;
}

HAL_StatusTypeDef dryHalSpiTransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{
	uint16_t             initial_TxXferCount;
	uint32_t             tmp_mode;
	HAL_SPI_StateTypeDef tmp_state;
	uint32_t             tickstart;

	/* Variable used to alternate Rx and Tx during transfer */
	uint32_t             txallowed = 1U;
	HAL_StatusTypeDef    errorcode = HAL_OK;

	/* Check Direction parameter */
	assert_param(IS_SPI_DIRECTION_2LINES(hspi->Init.Direction));

	/* Process Locked */
	__HAL_LOCK(hspi);

	/* Init tickstart for timeout management*/
	tickstart = HAL_GetTick();

	/* Init temporary variables */
	tmp_state           = hspi->State;
	tmp_mode            = hspi->Init.Mode;
	initial_TxXferCount = Size;

	if(!((tmp_state == HAL_SPI_STATE_READY) || \
					((tmp_mode == SPI_MODE_MASTER) && (hspi->Init.Direction == SPI_DIRECTION_2LINES) && (tmp_state == HAL_SPI_STATE_BUSY_RX)))) {
		errorcode = HAL_BUSY;
		goto error;
	}

	if((pTxData == NULL) || (pRxData == NULL) || (Size == 0U)) {
		errorcode = HAL_ERROR;
		goto error;
	}

	/* Don't overwrite in case of HAL_SPI_STATE_BUSY_RX */
	if(hspi->State != HAL_SPI_STATE_BUSY_RX) {
		hspi->State = HAL_SPI_STATE_BUSY_TX_RX;
	}

	/* Set the transaction information */
	hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pRxData;
	hspi->RxXferCount = Size;
	hspi->RxXferSize  = Size;
	hspi->pTxBuffPtr  = (uint8_t *)pTxData;
	hspi->TxXferCount = Size;
	hspi->TxXferSize  = Size;

	/*Init field not used in handle to zero */
	hspi->RxISR       = NULL;
	hspi->TxISR       = NULL;

#if (USE_SPI_CRC != 0U)
	/* Reset CRC Calculation */
	if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE) {
		SPI_RESET_CRC(hspi);
	}
#endif /* USE_SPI_CRC */

	/* Check if the SPI is already enabled */
	if((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE) {
		/* Enable SPI peripheral */
		__HAL_SPI_ENABLE(hspi);
	}

	/* Transmit and Receive data in 16 Bit mode */
	if(hspi->Init.DataSize == SPI_DATASIZE_16BIT) {
		if((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U)) {
			hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);
			hspi->pTxBuffPtr += sizeof(uint16_t);
			hspi->TxXferCount--;
		}
		while((hspi->TxXferCount > 0U) || (hspi->RxXferCount > 0U)) {
			/* Check TXE flag */
			if((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE)) && (hspi->TxXferCount > 0U) && (txallowed == 1U)) {
				hspi->Instance->DR = *((uint16_t *)hspi->pTxBuffPtr);
				hspi->pTxBuffPtr += sizeof(uint16_t);
				hspi->TxXferCount--;
				/* Next Data is a reception (Rx). Tx not allowed */
				txallowed = 0U;

#if (USE_SPI_CRC != 0U)
				/* Enable CRC Transmission */
				if((hspi->TxXferCount == 0U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)) {
					SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
				}
#endif /* USE_SPI_CRC */
			}

			/* Check RXNE flag */
			if((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) && (hspi->RxXferCount > 0U)) {
				*((uint16_t *)hspi->pRxBuffPtr) = (uint16_t)hspi->Instance->DR;
				hspi->pRxBuffPtr += sizeof(uint16_t);
				hspi->RxXferCount--;
				/* Next Data is a Transmission (Tx). Tx is allowed */
				txallowed = 1U;
			}
			if(((HAL_GetTick() - tickstart) >=  Timeout) && (Timeout != HAL_MAX_DELAY)) {
				errorcode = HAL_TIMEOUT;
				goto error;
			}
		}
	}
	/* Transmit and Receive data in 8 Bit mode */
	else {
		if((hspi->Init.Mode == SPI_MODE_SLAVE) || (initial_TxXferCount == 0x01U)) {
			*((__IO uint8_t *)&hspi->Instance->DR) = (*hspi->pTxBuffPtr);
			hspi->pTxBuffPtr += sizeof(uint8_t);
			hspi->TxXferCount--;
		}
		while((hspi->TxXferCount > 0U) || (hspi->RxXferCount > 0U)) {
			/* Check TXE flag */
			if((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE)) && (hspi->TxXferCount > 0U) && (txallowed == 1U)) {
				*(__IO uint8_t *)&hspi->Instance->DR = (*hspi->pTxBuffPtr);
				hspi->pTxBuffPtr++;
				hspi->TxXferCount--;
				/* Next Data is a reception (Rx). Tx not allowed */
				txallowed = 0U;

#if (USE_SPI_CRC != 0U)
				/* Enable CRC Transmission */
				if((hspi->TxXferCount == 0U) && (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)) {
					SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
				}
#endif /* USE_SPI_CRC */
			}

			/* Wait until RXNE flag is reset */
			if((__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) && (hspi->RxXferCount > 0U)) {
				(*(uint8_t *)hspi->pRxBuffPtr) = hspi->Instance->DR;
				hspi->pRxBuffPtr++;
				hspi->RxXferCount--;
				/* Next Data is a Transmission (Tx). Tx is allowed */
				txallowed = 1U;
			}
			if((((HAL_GetTick() - tickstart) >=  Timeout) && ((Timeout != HAL_MAX_DELAY))) || (Timeout == 0U)) {
				errorcode = HAL_TIMEOUT;
				goto error;
			}
		}
	}

#if (USE_SPI_CRC != 0U)
	/* Read CRC from DR to close CRC calculation process */
	if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE) {
		/* Wait until TXE flag */
		if(SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_RXNE, SET, Timeout, tickstart) != HAL_OK) {
			/* Error on the CRC reception */
			SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);
			errorcode = HAL_TIMEOUT;
			goto error;
		}
		/* Read CRC */
		READ_REG(hspi->Instance->DR);
	}

	/* Check if CRC error occurred */
	if(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_CRCERR) != RESET) {
		/* Check if CRC error is valid or not (workaround to be applied or not) */
		if(SPI_ISCRCErrorValid(hspi) == SPI_VALID_CRC_ERROR) {
			SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_CRC);

			/* Reset CRC Calculation */
			SPI_RESET_CRC(hspi);

			errorcode = HAL_ERROR;
		} else {
			__HAL_SPI_CLEAR_CRCERRFLAG(hspi);
		}
	}
#endif /* USE_SPI_CRC */

	/* Check the end of the transaction */
	if(SPI_EndRxTxTransaction(hspi, Timeout, tickstart) != HAL_OK) {
		errorcode = HAL_ERROR;
		hspi->ErrorCode = HAL_SPI_ERROR_FLAG;
		goto error;
	}

	/* Clear overrun flag in 2 Lines communication mode because received is not read */
	if(hspi->Init.Direction == SPI_DIRECTION_2LINES) {
		__HAL_SPI_CLEAR_OVRFLAG(hspi);
	}

error :
	hspi->State = HAL_SPI_STATE_READY;
	__HAL_UNLOCK(hspi);
	return errorcode;
}



// =============================================================================
// Static functions definitions
// =============================================================================

static HAL_StatusTypeDef SPI_EndRxTxTransaction(SPI_HandleTypeDef *hspi, uint32_t Timeout, uint32_t Tickstart)
{
	/* Control the BSY flag */
	if(SPI_WaitFlagStateUntilTimeout(hspi, SPI_FLAG_BSY, RESET, Timeout, Tickstart) != HAL_OK) {
		SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
		return HAL_TIMEOUT;
	}
	return HAL_OK;
}

static HAL_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus State, uint32_t Timeout, uint32_t Tickstart)
{
	while((__HAL_SPI_GET_FLAG(hspi, Flag) ? SET : RESET) != State) {
		if(Timeout != HAL_MAX_DELAY) {
			if(((HAL_GetTick() - Tickstart) >= Timeout) || (Timeout == 0U)) {
				/* Disable the SPI and reset the CRC: the CRC value should be cleared
				on both master and slave sides in order to resynchronize the master
				and slave for their respective CRC calculation */

				/* Disable TXE, RXNE and ERR interrupts for the interrupt process */
				__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

				if((hspi->Init.Mode == SPI_MODE_MASTER) && ((hspi->Init.Direction == SPI_DIRECTION_1LINE)
								|| (hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY))) {
					/* Disable SPI peripheral */
					__HAL_SPI_DISABLE(hspi);
				}

				/* Reset CRC Calculation */
				if(hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE) {
					SPI_RESET_CRC(hspi);
				}

				hspi->State = HAL_SPI_STATE_READY;

				/* Process Unlocked */
				__HAL_UNLOCK(hspi);

				return HAL_TIMEOUT;
			}
		}
	}

	return HAL_OK;
}

void dryHalSpiMspInit(SPI_HandleTypeDef *spiHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(spiHandle->Instance == SPI1) {
		/* USER CODE BEGIN SPI1_MspInit 0 */

		/* USER CODE END SPI1_MspInit 0 */
		/* SPI2 clock enable */
		__HAL_RCC_SPI1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**SPI2 GPIO Configuration
		PB5     ------> SPI1_SCK
		PB6     ------> SPI1_MISO
		PB7     ------> SPI1_MOSI
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* SPI2 interrupt Init */
		HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(SPI1_IRQn);
		/* USER CODE BEGIN SPI1_MspInit 1 */

		/* USER CODE END SPI1_MspInit 1 */
	}
}
