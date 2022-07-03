/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibBus.hpp
 * Module:          Generic Bus Stream for FunSAPE++ Embedded Library project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_BUS_HPP
#define __FUNSAPE_LIB_BUS_HPP                   __BUILD_NUMBER__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error [funsapeLibBus.hpp] Error 12 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_NUMBER__
#   error [funsapeLibBus.hpp] Error 13 - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibBus.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error   [funsapeLibBus.hpp] Error 14 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error   [funsapeLibBus.hpp] Error 15 - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibBus.hpp)!
#endif

#include "../funsapeLibGpio.hpp"
#ifndef __FUNSAPE_LIB_GPIO_HPP
#   error   [funsapeLibBus.hpp] Error 14 - Required module (funsapeLibGpio.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GPIO_HPP != __BUILD_NUMBER__
#   error   [funsapeLibBus.hpp] Error 15 - Build mismatch between required module (funsapeLibGpio.hpp) and current header file (funsapeLibBus.hpp)!
#endif

// =============================================================================
// Platform verification
// =============================================================================

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

// NONE

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
// Bus - Class declaration
// =============================================================================

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
class Bus
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------

public:
    /**
     * @brief
     *
     * Detailed description of this element.
     *
     */
    enum class BusType {
        NONE                            = 0,    //!< Bus is not initialized
        OWI                             = 1,    //!< One Wire Interface
        PARALLEL                        = 2,    //!< Simple parallel bitbang interface
        SERIAL                          = 3,    //!< Simple serial bitbang interface
        SPI                             = 4,    //!< Serial Peripheral Interface
        TWI                             = 5,    //!< Two Wire Interface (I2C)
        UART                            = 6,    //!< Universal Sync./Async. Receiver Transmitter
        USB                             = 7,    //!< Universal Serial Bus interface
    };

private:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Operator overloading ----------------------------------------------------

public:
    // NONE

private:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Constructors ------------------------------------------------------------

public:
    // NONE

    // -------------------------------------------------------------------------
    // Methods - Inherited methods ---------------------------------------------

public:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------

public:
    //     ////////////////////    DATA TRANSFER     ////////////////////     //

    /**
     * @brief
     *
     * Detailed description of this element.
     *
     * @param[in] reg_p description of this parameter
     * @param[out] buffData_p description of this parameter
     * @param[in] buffSize_p description of this parameter
     * @return true
     * @return false
     */
    virtual bool readReg(
            cuint8_t reg_p,
            uint8_t *buffData_p,
            cuint16_t buffSize_p
    ) {
        // Mark passage for debugging purpose
        debugMark("Bus::readReg(cuint8_t, uint8_t *, cuint16_t)", DEBUG_CODE_BUS);

        // Returns error
        this->_lastError = Error::FEATURE_UNSUPPORTED;
        debugMessage(Error::FEATURE_UNSUPPORTED);
        return false;
    };

    /**
     * @brief
     *
     * Detailed description of this element.
     *
     * @param[in,out] buffData_p description of this parameter
     * @param[in] buffSize_p description of this parameter
     * @return true
     * @return false
     */
    virtual bool sendData(
            uint8_t *buffData_p,
            cuint16_t buffSize_p
    ) {
        // Mark passage for debugging purpose
        debugMark("Bus::sendData(uint8_t *, cuint16_t)", DEBUG_CODE_BUS);

        // Returns error
        this->_lastError = Error::FEATURE_UNSUPPORTED;
        debugMessage(Error::FEATURE_UNSUPPORTED);
        return false;
    };

    /**
     * @brief
     *
     * Detailed description of this element.
     *
     * @param[in] txBuffData_p description of this parameter
     * @param[out] rxBuffData_p description of this parameter
     * @param[in] buffSize_p description of this parameter
     * @return true
     * @return false
     */
    virtual bool sendData(
            cuint8_t *txBuffData_p,
            uint8_t *rxBuffData_p,
            cuint16_t buffSize_p
    ) {
        // Mark passage for debugging purpose
        debugMark("Bus::sendData(cuint8_t *, uint8_t *, cuint16_t)", DEBUG_CODE_BUS);

        // Returns error
        this->_lastError = Error::FEATURE_UNSUPPORTED;
        debugMessage(Error::FEATURE_UNSUPPORTED);
        return false;
    };

    /**
     * @brief
     *
     * Detailed description of this element.
     *
     * @param[in] reg_p description of this parameter
     * @param[in] buffData_p description of this parameter
     * @param[in] buffSize_p description of this parameter
     * @return true
     * @return false
     */
    virtual bool writeReg(
            cuint8_t reg_p,
            cuint8_t *buffData_p,
            cuint16_t buffSize_p
    ) {
        // Mark passage for debugging purpose
        debugMark("Bus::writeReg(cuint8_t, cuint8_t *, cuint16_t)", DEBUG_CODE_BUS);

        // Returns error
        this->_lastError = Error::FEATURE_UNSUPPORTED;
        debugMessage(Error::FEATURE_UNSUPPORTED);
        return false;
    };

    //     ////////////////     TWI PROTOCOL METHODS     ////////////////     //

    /**
     * @brief Set the Device object
     *
     * Detailed description of this element.
     *
     * @param[in] address_p description of this parameter
     * @param[in] useLongAddress_p description of this parameter
     * @return true
     * @return false
     */
    virtual bool setDevice(
            cuint16_t address_p,
            cbool useLongAddress_p = false
    ) {
        // Mark passage for debugging purpose
        debugMark("Bus::setDevice(cuint16_t, cbool)", DEBUG_CODE_BUS);

        // Returns error
        this->_lastError = Error::FEATURE_UNSUPPORTED;
        debugMessage(Error::FEATURE_UNSUPPORTED);
        return false;
    };

    //     ////////////////     SPI PROTOCOL METHODS     ////////////////     //

    /**
     * @brief Set the Device object
     *
     * Detailed description of this element.
     *
     * @param[in] actFunc_p description of this parameter
     * @param[in] deactFunc_p description of this parameter
     * @return true
     * @return false
     */
    virtual bool setDevice(
            void (* actFunc_p)(void),
            void (* deactFunc_p)(void)
    ) {
        // Mark passage for debugging purpose
        debugMark("Bus::setDevice(void *(void), void *(void))", DEBUG_CODE_BUS);

        // Returns error
        this->_lastError = Error::FEATURE_UNSUPPORTED;
        debugMessage(Error::FEATURE_UNSUPPORTED);
        return false;
    };

    /**
     * @brief Set the Device object
     *
     * Detailed description of this element.
     *
     * @param[in] csPin_p description of this parameter
     * @return true
     * @return false
     */
    virtual bool setDevice(
            const Gpio *csPin_p
    ) {
        // Mark passage for debugging purpose
        debugMark("Bus::setDevice(const Gpio *)", DEBUG_CODE_BUS);

        // Returns error
        this->_lastError = Error::FEATURE_UNSUPPORTED;
        debugMessage(Error::FEATURE_UNSUPPORTED);
        return false;
    };

    //     /////////////////     CONTROL AND STATUS     /////////////////     //

    /**
     * @brief Get the Bus Type object
     *
     * Detailed description of this element.
     *
     * @return Bus::BusType
     */
    virtual Bus::BusType getBusType(void) {
        // Mark passage for debugging purpose
        debugMark("Bus::getBusType(void)", DEBUG_CODE_BUS);

        // Returns default handler
        return BusType::NONE;
    }

    /**
     * @brief Get the Last Error object
     *
     * Detailed description of this element.
     *
     * @return Error
     */
    virtual Error getLastError(void) {
        // Returns last error
        return this->_lastError;
    }

private:
    // NONE

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------

public:
    // NONE

private:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    Error   _lastError;

protected:
    // NONE

}; // class Bus

// =============================================================================
// Bus - Class overloading operators
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
// Bus - Class inline function definitions
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

// NONE

#endif // __FUNSAPE_LIB_BUS_HPP

// =============================================================================
// END OF FILE
// =============================================================================
