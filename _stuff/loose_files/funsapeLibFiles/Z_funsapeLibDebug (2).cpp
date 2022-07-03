/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibDebug.cpp
 * Module:          Debug Module for FunSAPE++ Embedded Library project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error [funsapeLibDebug.cpp] Error - Header file (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error [funsapeLibDebug.cpp] Error - Build mismatch between source (funsapeLibDebug.cpp) and header (funsapeLibDebug.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

cuint16_t debugDefaultTimeOut = 30;

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

Debug debug;

// =============================================================================
// Static functions declarations
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

/**
 * @brief
 * @details
 */
Debug::Debug(void)
{
    // Resets data members
    this->_busHandler                   = nullptr;
    // FIXIT Implement automatic bus detection
    // this->_busType                      = BusType::UNDEFINED;
    this->_hideSuccessMessages          = true;
    this->_initialized                  = false;
    this->_stopOnFirstError             = true;
    this->_timeOut                      = debugDefaultTimeOut;
    for (uint8_t i = 0; i < codeIndexMax; i++) {
        this->_list[i]                  = false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    return;
}

/**
 * @brief
 * @details
 */
Debug::~Debug(void)
{
    // TODO: Implement destructor function

    // Returns successfully
    return;
}

// =============================================================================
// Inherited methods - Public
// =============================================================================

// NONE

// =============================================================================
// Inherited methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Public
// =============================================================================

//     ///////////////////     INITIALIZATION     ///////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[out] handler_p            description of this parameter
 * @param[in] hideSuccess_p         description of this parameter
 * @param[in] stopOnError_p         description of this parameter
 * @return true
 * @return false
 */
bool_t Debug::init(UART_HandleTypeDef *handler_p, cbool_t hideSuccess_p, cbool_t stopOnError_p)
{
    // Checks for errors
    if (!isPointerValid(handler_p)) {
        this->_lastError = Error::HANDLER_POINTER_NULL;
        return false;
    }

    // FIXIT Implement automatic bus detection

    // Update data members
    this->_busHandler                   = handler_p;
    this->_hideSuccessMessages          = hideSuccess_p;
    this->_initialized                  = true;
    this->_stopOnFirstError             = stopOnError_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

//     /////////////////     CONTROL AND STATUS     /////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @return Error
 */
Error Debug::getLastError(void)
{
    // Returns last error
    return this->_lastError;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] enabled_p description of this parameter
 * @return true
 * @return false
 */
bool_t Debug::hideSuccess(cbool_t enabled_p)
{
    // Update data members
    this->_hideSuccessMessages = enabled_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] timeOut_p description of this parameter
 * @return true
 * @return false
 */
bool_t Debug::setTimeOut(cuint16_t timeOut_p)
{
    // Update data members
    this->_timeOut = timeOut_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] enabled_p description of this parameter
 * @return true
 * @return false
 */
bool_t Debug::stopOnError(cbool_t enabled_p)
{
    // Update data members
    this->_stopOnFirstError = enabled_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    return true;
}

//     ////////////////////    LOGS AND MESSAGES     ////////////////////     //

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in]   message_p
 */
bool_t Debug::printMessage(char *message_p)
{
    // FIXIT Implement automatic bus detection
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(HAL_UART_MODULE_ENABLED)
    HAL_StatusTypeDef halError;

    // Logs message
    halError = HAL_UART_Transmit((UART_HandleTypeDef *)this->_busHandler,
                    (uint8_t *)message_p,
                    strlen(message_p),
                    this->_timeOut
            );

    // Returns
    switch (halError) {
    case HAL_ERROR:
        this->_lastError = Error::COMMUNICATION_FAILED;
        return false;
    case HAL_BUSY:
        this->_lastError = Error::BUSY;
        return false;
    case HAL_TIMEOUT:
        this->_lastError = Error::COMMUNICATION_TIMEOUT;
        return false;
    case HAL_OK:
        break;
    }

#   else
    // TODO: Non-UART HAL ARM implementation
#   endif
#elif defined(_FUNSAPE_PLATFORM_AVR)
    // TODO: AVR implementation
#endif

    // Return successfully
    this->_lastError = Error::NONE;
    return true;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in]   file_p          description of this parameter
 * @param[in]   line_p          description of this parameter
 * @param[in]   identifier_p    description of this parameter
 */
void Debug::logMark(cuint8_t *file_p, cuint16_t line_p, cuint8_t *identifier_p, CodeIndex module_p)
{
    // Local variables
    char strTest[200];

    // Just exits if user doesn't want to log marks
    if (!(this->_list[(uint8_t)module_p])) {
        return;
    }

    // FIXIT Implement automatic bus detection
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(HAL_UART_MODULE_ENABLED)

    // Logs message
    // FIXIT Use sprintf function without dynamic memory allocation
    sprintf(strTest, "-> %s : %d @ %s\r", (char *)file_p, (uint16_t)line_p, (char *)identifier_p);
    HAL_UART_Transmit((UART_HandleTypeDef *)this->_busHandler, (uint8_t *)strTest, strlen(strTest), this->_timeOut);

#   endif
#endif

    // Returns
    return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in]   errorCode_p description of this parameter
 * @param[in]   file_p      description of this parameter
 * @param[in]   line_p      description of this parameter
 */
void Debug::logMessage(cuint16_t errorCode_p, cuint8_t *file_p, cuint16_t line_p)
{
    char strTest[200];

    // Just exits if user doesn't want to log success messages
    if (this->_hideSuccessMessages) {
        if (errorCode_p == 0) {
            return;
        }
    }

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(HAL_UART_MODULE_ENABLED)

    // Logs message
    sprintf(strTest, "%s:%d - 0x%04X\r", file_p, (uint16_t)line_p, (uint16_t)errorCode_p);
    HAL_UART_Transmit((UART_HandleTypeDef *)this->_busHandler, (uint8_t *)strTest, strlen(strTest), this->_timeOut);

    // If must stop on first error
    if (this->_stopOnFirstError) {
        if (errorCode_p) {
            systemHalt();
        }
    }

#   endif
#endif

    // Returns
    return;
}

//     /////////////////////////    MODULES     /////////////////////////     //

void Debug::debugAdc(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::ADC_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugCan(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::CAN_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugCec(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::CEC_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugCortex(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::CORTEX_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugCrc(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::CRC_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugDac(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::DAC_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugDma(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::DMA_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugEth(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::ETH_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugExti(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::EXTI_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugFlash(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::FLASH_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugGpio(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::GPIO_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugHcd(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::HCD_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugI2s(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::I2S_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugIrda(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::IRDA_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugIwdg(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::IWDG_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugMmc(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::MMC_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugNand(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::NAND_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugNor(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::NOR_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugPcCard(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::PC_CARD_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugPcd(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::PCD_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugPwr(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::PWR_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugRcc(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::RCC_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugRtc(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::RTC_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugSdCard(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::SD_CARD_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugSmartCard(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::SMART_CARD_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugSpi(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::SPI_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugSram(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::SRAM_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugTimer(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::TIMER_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugTm1637(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::TM1637_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugTwi(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::TWI_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugUart(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::UART_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugUsart(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::USART_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugWwdg(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::WWDG_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugBus(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::BUS_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugCircularBuffer(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::CIRCULAR_BUFFER_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugDateTime(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::DATE_TIME_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugDs1307(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::DS1307_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugMax3010x(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::MAX3010X_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugMpu(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::MPU_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugStopwatch(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::STOPWATCH_MODULE] = enabled_p;

    // Returns
    return;
}

void Debug::debugUsb(bool_t enabled_p)
{
    // Update debug list
    this->_list[(uint8_t)CodeIndex::USB_MODULE] = enabled_p;

    // Returns
    return;
}















/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] markLevel_p   description of this parameter
 * @return true
 * @return false
 */
// bool_t Debug::setMarkLevel(cuint32_t markLevel_p)
// {
//     // Update data members
//     this->_markLevel = markLevel_p;

//     // Returns successfully
//     this->_lastError = Error::NONE;
//     return true;
// }

// =============================================================================
// Class own methods - Private
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

// =============================================================================
// END OF FILE
// =============================================================================
