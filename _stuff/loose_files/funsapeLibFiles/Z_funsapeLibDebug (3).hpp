/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibDebug.hpp
 * Module:          Debug Module for FunSAPE++ Embedded Library project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_DEBUG_HPP
#define __FUNSAPE_LIB_DEBUG_HPP                 __BUILD_NUMBER__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error [funsapeLibDebug.hpp] Error - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_NUMBER__
#   error [funsapeLibDebug.hpp] Error - Build mismatch between (funsapeLibDebug.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

// NONE

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

// NONE

// =============================================================================
// Macro-function definitions
// *INDENT-OFF*
// =============================================================================

#define debugMessage(code)              do{debug.logMessage((uint16_t)(code), (uint8_t *)__FILE__, __LINE__);}while(0)
#define debugMark(identifier, module_p)    do{debug.logMark((uint8_t *)__FILE__, __LINE__, (uint8_t *)identifier, (Debug::CodeIndex)module_p);}while (0)

// #define debugMark(identifier, level)    
// #define debugMark(identifier, level)    do{if(debug.getMarkLevel() & level){debug.logMark((uint8_t *)__FILE__, __LINE__, (uint8_t *)identifier);}}while(0)
// #define debugMark(identifier, level)    do{if(debug.getMarkLevel() >= level){debug.logMark((uint8_t *)__FILE__, __LINE__, (uint8_t *)identifier);}}while(0)

// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// Debug - Class declaration
// =============================================================================

class Debug
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------

public:
    enum class CodeIndex {
        ADC_MODULE                      = 0,
        CAN_MODULE                      = 1,
        CEC_MODULE                      = 2,
        CORTEX_MODULE                   = 3,
        CRC_MODULE                      = 4,
        DAC_MODULE                      = 5,
        DMA_MODULE                      = 6,
        ETH_MODULE                      = 7,
        EXTI_MODULE                     = 8,
        FLASH_MODULE                    = 9,
        GPIO_MODULE                     = 10,
        HCD_MODULE                      = 11,
        I2S_MODULE                      = 12,
        IRDA_MODULE                     = 13,
        IWDG_MODULE                     = 14,
        MMC_MODULE                      = 15,
        NAND_MODULE                     = 16,
        NOR_MODULE                      = 17,
        PC_CARD_MODULE                  = 18,
        PCD_MODULE                      = 19,
        PWR_MODULE                      = 20,
        RCC_MODULE                      = 21,
        RTC_MODULE                      = 22,
        SD_CARD_MODULE                  = 23,
        SMART_CARD_MODULE               = 24,
        SPI_MODULE                      = 25,
        SRAM_MODULE                     = 26,
        TIMER_MODULE                    = 27,
        TM1637_MODULE                   = 28,
        TWI_MODULE                      = 29,
        UART_MODULE                     = 30,
        USART_MODULE                    = 31,
        WWDG_MODULE                     = 32,
        BUS_MODULE                      = 33,
        CIRCULAR_BUFFER_MODULE          = 34,
        DATE_TIME_MODULE                = 35,
        DS1307_MODULE                   = 36,
        MAX3010X_MODULE                 = 37,
        MPU_MODULE                      = 38,
        STOPWATCH_MODULE                = 39,
        USB_MODULE                      = 40,
    };

    cuint8_t codeIndexMax               = 64;

private:
    // FIXIT Implement automatic bus detection
    // enum class BusType {
    //     UNDEFINED                       = 0,
    //     UART_BUS                        = 1,
    // };

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Operators overloading ---------------------------------------------------

public:
    // NONE

private:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Constructors ------------------------------------------------------------

public:
    Debug(
            void
    );
    ~Debug(
            void
    );

    // -------------------------------------------------------------------------
    // Methods - Inherited methods ---------------------------------------------

public:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------

public:
    //     ///////////////////     INITIALIZATION     ///////////////////     //

    bool_t init(
            UART_HandleTypeDef *handler_p,
            cbool_t hideSuccess_p               = true,
            cbool_t stopOnError_p               = true
    );

// #if defined(_FUNSAPE_PLATFORM_ARM_STM32)
//     // ARM implementation
// #   if defined(HAL_UART_MODULE_ENABLED)
//     // UART HAL ARM implementation
//     bool_t init(
//             UART_HandleTypeDef *handler_p,
//             cbool_t hideSuccess_p               = true,
//             cbool_t stopOnError_p               = true,
//             cuint32_t markLevel_p               = 0
//     );
// #   else
//     // Non-UART HAL ARM implementation
//     // TODO: Non-UART HAL ARM implementation
//     bool_t init(
//             uint16_t *handler_p,
//             cbool_t hideSuccess_p               = true,
//             cbool_t stopOnError_p               = true,
//             cuint32_t markLevel_p               = 0
//     );
// #   endif
// #elif defined(_FUNSAPE_PLATFORM_AVR)
//     // AVR implementation
//     // TODO: AVR implementation
//     bool_t init(
//             uint16_t *handler_p,
//             cbool_t hideSuccess_p               = true,
//             cbool_t stopOnError_p               = true,
//             cuint32_t markLevel_p               = 0
//     );
// #else
// #   error [funsapeLibDebug.hpp] Error - Platform unsupported!
// #endif

    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    Error getLastError(
            void
    );
    bool_t hideSuccess(
            cbool_t enabled_p
    );
    bool_t setTimeOut(
            cuint16_t timeOut_p
    );
    bool_t stopOnError(
            cbool_t enabled_p
    );

    //     //////////////////    LOGS AND MESSAGES     //////////////////     //
    bool_t printMessage(
            char *message_p
    );
    void logMark(
            cuint8_t *fileName_p,
            cuint16_t lineNumber_p,
            cuint8_t *identifier_p,
            CodeIndex module_p
    );
    void logMessage(
            cuint16_t errorCode_p,
            cuint8_t *fileName_p,
            cuint16_t lineNumber_p
    );

    //     ///////////////////////    MODULES     ///////////////////////     //
    void debugAdc(
            bool_t enabled_p
    );
    void debugCan(
            bool_t enabled_p
    );
    void debugCec(
            bool_t enabled_p
    );
    void debugCortex(
            bool_t enabled_p
    );
    void debugCrc(
            bool_t enabled_p
    );
    void debugDac(
            bool_t enabled_p
    );
    void debugDma(
            bool_t enabled_p
    );
    void debugEth(
            bool_t enabled_p
    );
    void debugExti(
            bool_t enabled_p
    );
    void debugFlash(
            bool_t enabled_p
    );
    void debugGpio(
            bool_t enabled_p
    );
    void debugHcd(
            bool_t enabled_p
    );
    void debugI2s(
            bool_t enabled_p
    );
    void debugIrda(
            bool_t enabled_p
    );
    void debugIwdg(
            bool_t enabled_p
    );
    void debugMmc(
            bool_t enabled_p
    );
    void debugNand(
            bool_t enabled_p
    );
    void debugNor(
            bool_t enabled_p
    );
    void debugPcCard(
            bool_t enabled_p
    );
    void debugPcd(
            bool_t enabled_p
    );
    void debugPwr(
            bool_t enabled_p
    );
    void debugRcc(
            bool_t enabled_p
    );
    void debugRtc(
            bool_t enabled_p
    );
    void debugSdCard(
            bool_t enabled_p
    );
    void debugSmartCard(
            bool_t enabled_p
    );
    void debugSpi(
            bool_t enabled_p
    );
    void debugSram(
            bool_t enabled_p
    );
    void debugTimer(
            bool_t enabled_p
    );
    void debugTm1637(
            bool_t enabled_p
    );
    void debugTwi(
            bool_t enabled_p
    );
    void debugUart(
            bool_t enabled_p
    );
    void debugUsart(
            bool_t enabled_p
    );
    void debugWwdg(
            bool_t enabled_p
    );
    void debugBus(
            bool_t enabled_p
    );
    void debugCircularBuffer(
            bool_t enabled_p
    );
    void debugDateTime(
            bool_t enabled_p
    );
    void debugDs1307(
            bool_t enabled_p
    );
    void debugMax3010x(
            bool_t enabled_p
    );
    void debugMpu(
            bool_t enabled_p
    );
    void debugStopwatch(
            bool_t enabled_p
    );
    void debugUsb(
            bool_t enabled_p
    );

private:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------

public:
    // NONE

private:
    //     /////////////////    PERIPHERAL HANDLER     //////////////////     //
    void        *_busHandler;
    // BusType     _busType;
    // FIXIT Implement automatic bus detection

    //     /////////////////    CONTROL AND STATUS     //////////////////     //
    bool_t      _hideSuccessMessages            : 1;
    bool_t      _initialized                    : 1;
    Error       _lastError;
    bool_t      _stopOnFirstError               : 1;
    uint16_t    _timeOut;
    bool_t      _list[64];

protected:
    // NONE

}; // class Debug

// =============================================================================
// Debug - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// -----------------------------------------------------------------------------
// Externally defined global variables -----------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Internally defined global variables -----------------------------------------

// NONE

// =============================================================================
// Debug - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// General public functions declarations
// =============================================================================

// NONE

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

// =============================================================================
// External default objects
// =============================================================================

extern Debug debug;

#endif // __FUNSAPE_LIB_DEBUG_HPP

// =============================================================================
// END OF FILE
// =============================================================================
