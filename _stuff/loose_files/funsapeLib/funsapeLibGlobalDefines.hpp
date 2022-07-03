/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibGlobalDefines.hpp
 * Module:          Global definitions for FunSAPE++ Embedded Library project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#define __FUNSAPE_LIB_GLOBAL_DEFINES_HPP        __BUILD_NUMBER__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Project configuration file --------------------------------------------------

#include "config/funsapeLibProjectConfig.hpp"
#ifndef __FUNSAPE_LIB_PROJECT_CONFIG_HPP
#   error [funsapeLibGlobalDefines.hpp] Error - Project configuration file (funsapeLibProjectConfig.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_PROJECT_CONFIG_HPP != __BUILD_NUMBER__
#   error [funsapeLibGlobalDefines.hpp] Error - Build mismatch between project configuration file (funsapeLibProjectConfig.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - Basic header for all files -----------------------------------

#include <stddef.h>
#include <sys/_stdint.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// -----------------------------------------------------------------------------
// STM32Cube project header  ---------------------------------------------------

#ifdef _FUNSAPE_PLATFORM_ARM_STM32
#   if __has_include("main.h")
#      include "main.h"
#   endif
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

#undef byteNibble0
#undef byteNibble1
#undef byteSwitchNibbles
#undef clrBit
#undef clrIoPin
#undef clrMask
#undef convertToLowerCase
#undef convertToUpperCase
#undef cplBit
#undef cplIoPin
#undef cplMask
#undef delayMs
#undef delayUs
// #undef die
// #undef doNop
// #undef doNothing
#undef dwordByte0
#undef dwordByte1
#undef dwordByte2
#undef dwordByte3
#undef dwordByte4
#undef dwordByte5
#undef dwordByte6
#undef dwordByte7
#undef dwordHword0
#undef dwordHword1
#undef dwordHword2
#undef dwordHword3
#undef dwordSwitchBytes
#undef dwordSwitchHwords
#undef dwordSwitchWords
#undef dwordWord0
#undef dwordWord1
#undef getMaximumOf
#undef getMinimumOf
#undef getTick
#undef hwordByte0
#undef hwordByte1
#undef hwordSwitchBytes
#undef ignored
#undef inlined
#undef isAsciiCommand
#undef isAsciiLetter
#undef isAsciiLowerCaseLetter
#undef isAsciiNumber
#undef isAsciiSymbol
#undef isAsciiUpperCaseLetter
#undef isBitClr
#undef isBitSet
#undef isMaskMatch
#undef isGpioValid
#undef isPointerValid
#undef setBit
#undef setIoPin
#undef setMask
#undef stringifyName
#undef stringifyValue
// #undef systemHalt
#undef truncateBetween
#undef waitUntilBitIsClear
#undef waitUntilBitIsSet
#undef waitUntilMaskMatch
#undef weakened
#undef wordByte0
#undef wordByte1
#undef wordByte2
#undef wordByte3
#undef wordHword0
#undef wordHword1
#undef wordSwitchBytes
#undef wordSwitchHwords

// =============================================================================
// Text alias
// =============================================================================

// ¬ALIAS inlined (attribute)
#define inlined                         inline __attribute__((always_inline))
// ¬ALIAS ignored (attribute)
#define ignored(var)                    (void)var
// ¬ALIAS weakened (attribute)
#define weakened                        __attribute__((weak))

// =============================================================================
// Constant definitions
// =============================================================================

// NONE

// =============================================================================
// Macro-function definitions
// *INDENT-OFF*
// =============================================================================

// -----------------------------------------------------------------------------
// Bit handling and bit loops --------------------------------------------------

// ¬MACRO clrBit(reg, bit)
#define clrBit(reg, bit)                        ((reg) &= (~(1 << (bit))))
// ¬MACRO cplBit(reg, bit)
#define cplBit(reg, bit)                        ((reg) ^= (1 << (bit)))
// ¬MACRO isBitClr(reg, bit)
#define isBitClr(reg, bit)                      (bool_t)(!(((reg) >> (bit)) & 1))
// ¬MACRO isBitSet(reg, bit)
#define isBitSet(reg, bit)                      (bool_t)(((reg) >> (bit)) & 0x01)
// ¬MACRO setBit(reg, bit)
#define setBit(reg, bit)                        ((reg) |= (1 << (bit)))
// ¬MACRO waitUntilBitIsClear(reg, bit)
#define waitUntilBitIsClear(reg, bit)           do {} while(isBitSet((reg), (bit)))
// ¬MACRO waitUntilBitIsSet(reg, bit)
#define waitUntilBitIsSet(reg, bit)             do {} while(isBitClr((reg), (bit)))

// -----------------------------------------------------------------------------
// Mask handling and mask loops ------------------------------------------------

// ¬MACRO clrMask(reg, mask)
#define clrMask(reg, mask)                      ((reg) &= (~(mask)))
// ¬MACRO clrMaskOffset(reg, mask, offset)
#define clrMaskOffset(reg, mask, offset)        ((reg) &= (~((mask) << (offset))))
// ¬MACRO cplMask(reg, mask)
#define cplMask(reg, mask)                      ((reg) ^= (mask))
// ¬MACRO cplMaskOffset(reg, mask, offset)
#define cplMaskOffset(reg, mask, offset)        ((reg) ^= ((mask) << (offset)))
// ¬MACRO setMask(reg, mask)
#define setMask(reg, mask)                      ((reg) |= (mask))
// ¬MACRO setMaskOffset(reg, mask, offset)
#define setMaskOffset(reg, mask, offset)        ((reg) |= ((mask) << (offset)))

// ¬BUG The functions above does not work as intended
// #define isMaskMatch(reg, mask)                  (bool_t)(((reg) == (mask)) ? true : false)
// #define isMaskOffsetMatch(reg, mask, offset)    (bool_t)(((reg) == (mask)) ? true : false)
// #define waitUntilMaskMatch(reg, mask, offset)   (bool_t)((((reg) >> (offset)) == (mask)) ? true : false)

// -----------------------------------------------------------------------------
// Io Pin handling -------------------------------------------------------------

// ¬MACRO clrIoPin(gpioPort, gpioPin)
// ¬MACRO cplIoPin(gpioPort, gpioPin)
// ¬MACRO setIoPin(gpioPort, gpioPin)
#if defined(_FUNSAPE_PLATFORM_AVR)
#   error AVR implementation is not ready yet
#   define clrIoPin(gpioPort, gpioPin)  {}
#   define cplIoPin(gpioPort, gpioPin)  {}
#   define setIoPin(gpioPort, gpioPin)  {}
// ¬TODO AVR implementation of clrIoPin() macrofunction
// ¬TODO AVR implementation of cplIoPin() macrofunction
// ¬TODO AVR implementation of setIoPin() macrofunction
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   define clrIoPin(GPIOx, GPIO_Pin)    (GPIOx->BRR = (uint32_t)(GPIO_Pin))
#   define cplIoPin(GPIOx, GPIO_Pin)    (((GPIOx->ODR & GPIO_Pin) != 0x00u) ? (GPIOx->BRR = (uint32_t)(GPIO_Pin)) : (GPIOx->BSRR = (uint32_t)(GPIO_Pin)))
#   define setIoPin(GPIOx, GPIO_Pin)    (GPIOx->BSRR = (uint32_t)(GPIO_Pin))
#else
#   error Implementation for the selected platform is unavailable
#endif

// -----------------------------------------------------------------------------
// Data manipulation (8-bits data) ---------------------------------------------

// ¬MACRO byteNibble1(dataByte)
#define byteNibble1(dataByte)           (((dataByte) >> 4) & 0x0F)
// ¬MACRO byteNibble0(dataByte)
#define byteNibble0(dataByte)           ((dataByte) & 0x0F)
// ¬MACRO byteSwitchNibbles(dataByte)
#define byteSwitchNibbles(dataByte)     ((byteNibble0(dataByte) << 4) | \
                                        (byteNibble1(dataByte)))

// -----------------------------------------------------------------------------
// Data manipulation (16-bits data) --------------------------------------------

// ¬MACRO hwordByte1(dataHword)
#define hwordByte1(dataHword)           (((dataHword) >> 8) & 0x00FF)
// ¬MACRO hwordByte0(dataHword)
#define hwordByte0(dataHword)           ((dataHword) & 0x00FF)
// ¬MACRO hwordSwitchBytes(dataHword)
#define hwordSwitchBytes(dataHword)     ((hwordByte0(dataHword) << 8) | \
                                        (hwordByte1(dataHword)))

// -----------------------------------------------------------------------------
// Data manipulation (32-bits data) --------------------------------------------

// ¬MACRO wordByte3(dataWord)
#define wordByte3(dataWord)             (((dataWord) >> 24) & 0x000000FF)
// ¬MACRO wordByte2(dataWord)
#define wordByte2(dataWord)             (((dataWord) >> 16) & 0x000000FF)
// ¬MACRO wordByte1(dataWord)
#define wordByte1(dataWord)             (((dataWord) >> 8) & 0x000000FF)
// ¬MACRO wordByte0(dataWord)
#define wordByte0(dataWord)             ((dataWord) & 0x000000FF)
// ¬MACRO wordSwitchBytes(dataWord)
#define wordSwitchBytes(dataWord)       ((wordByte0(dataWord) << 24)  | \
                                        (wordByte1(dataWord) << 16) | \
                                        (wordByte2(dataWord) << 8)  | \
                                        (wordByte3(dataWord)))
// ¬MACRO wordHword1(dataWord)
#define wordHword1(dataWord)            (((dataWord) >> 16) & 0x0000FFFF)
// ¬MACRO wordHword0(dataWord)
#define wordHword0(dataWord)            ((dataWord) & 0x0000FFFF)
// ¬MACRO wordSwitchHwords(dataWord)
#define wordSwitchHwords(dataWord)      (wordHword0(dataWord) << 16) | \
                                        (wordHword1(dataWord)))

// -----------------------------------------------------------------------------
// Data manipulation (64-bits data) --------------------------------------------

// ¬MACRO dwordByte7(dataDword)
#define dwordByte7(dataDword)           (((dataDword) >> 56) & 0x00000000000000FF)
// ¬MACRO dwordByte6(dataDword)
#define dwordByte6(dataDword)           (((dataDword) >> 48) & 0x00000000000000FF)
// ¬MACRO dwordByte5(dataDword)
#define dwordByte5(dataDword)           (((dataDword) >> 40) & 0x00000000000000FF)
// ¬MACRO dwordByte4(dataDword)
#define dwordByte4(dataDword)           (((dataDword) >> 32) & 0x00000000000000FF)
// ¬MACRO dwordByte3(dataDword)
#define dwordByte3(dataDword)           (((dataDword) >> 24) & 0x00000000000000FF)
// ¬MACRO dwordByte2(dataDword)
#define dwordByte2(dataDword)           (((dataDword) >> 16) & 0x00000000000000FF)
// ¬MACRO dwordByte1(dataDword)
#define dwordByte1(dataDword)           (((dataDword) >> 8) & 0x00000000000000FF)
// ¬MACRO dwordByte0(dataDword)
#define dwordByte0(dataDword)           ((dataDword) & 0x00000000000000FF)
// ¬MACRO dwordSwitchBytes(dataDword)
#define dwordSwitchBytes(dataDword)     ((dwordByte0(dataDword) << 56)  | \
                                        (dwordByte1(dataDword) << 48) | \
                                        (dwordByte2(dataDword) << 40) | \
                                        (dwordByte3(dataDword) << 32) | \
                                        (dwordByte4(dataDword) << 24) | \
                                        (dwordByte5(dataDword) << 16) | \
                                        (dwordByte6(dataDword) << 8)  | \
                                        (dwordByte7(dataDword)))
// ¬MACRO dwordHword3(dataDword)
#define dwordHword3(dataDword)          ((dataDword >> 48) & 0x000000000000FFFF)
// ¬MACRO dwordHword2(dataDword)
#define dwordHword2(dataDword)          ((dataDword >> 32) & 0x000000000000FFFF)
// ¬MACRO dwordHword1(dataDword)
#define dwordHword1(dataDword)          ((dataDword >> 16) & 0x000000000000FFFF)
// ¬MACRO dwordHword0(dataDword)
#define dwordHword0(dataDword)          ((dataDword >> 0) & 0x000000000000FFFF)
// ¬MACRO dwordSwitchHwords(dataDword)
#define dwordSwitchHwords(dataDword)    ((dwordHword0(dataDword) << 48)  | \
                                        (dwordHword1(dataDword) << 32) | \
                                        (dwordHword2(dataDword) << 16) | \
                                        (dwordHword3(dataDword)))
// ¬MACRO dwordWord1(dataDword)
#define dwordWord1(dataDword)           (((dataDword) >> 32) & 0x00000000FFFFFFFF)
// ¬MACRO dwordWord0(dataDword)
#define dwordWord0(dataDword)           ((dataDword) & 0x00000000FFFFFFFF)
// ¬MACRO dwordSwitchWords(dataDword)
#define dwordSwitchWords(dataDword)     ((dwordWord0(dataDword) << 32)  | \
                                        (dwordWord1(dataDword)))

// -----------------------------------------------------------------------------
// Truncate values -------------------------------------------------------------

// ¬MACRO getMaximumOf(var1, var2)
#define getMaximumOf(var1, var2)       (((var1) > (var2))  ? \
                                        (((var1)))          : \
                                        (((var2))))
// ¬MACRO getMinimumOf(var1, var2)
#define getMinimumOf(var1, var2)       (((var1) < (var2))  ? \
                                        (((var1)))          : \
                                        (((var2))))
// ¬MACRO truncateBetween(var, min, max)
#define truncateBetween(var, min, max)  (((var)) < (min))   ? \
                                        (((min)))           : \
                                        (((var) > (max))    ? \
                                        (((max)))           : \
                                        (((var)))))

// -----------------------------------------------------------------------------
// Checking data validity ------------------------------------------------------

// ¬MACRO isGpioPortValid(port)
#define isGpioPortValid(port)              ((bool_t)true)
// ¬MACRO isGpioPinValid(port, first, offset, length)
#define isGpioPinValid(port, first, offset, length)    ((bool_t)true)
// ¬MACRO isPointerValid(ptr)
#define isPointerValid(ptr)             ((bool_t)((ptr) ? (bool_t)true : (bool_t)false))

// -----------------------------------------------------------------------------
// String macros ---------------------------------------------------------------

// ¬MACRO convertToLowerCase(charac)
#define convertToLowerCase(charac)      ((isAsciiUpperCaseLetter(charac)) ? (charac + 0x20) : charac)
// ¬MACRO convertToUpperCase(charac)
#define convertToUpperCase(charac)      ((isAsciiLowerCaseLetter(charac)) ? (charac - 0x20) : charac)
// ¬MACRO stringifyName(constant)
#define stringifyName(constant)         #constant
// ¬MACRO stringifyValue(constant)
#define stringifyValue(constant)        stringifyName(constant)

// -----------------------------------------------------------------------------
// System actions --------------------------------------------------------------

// ¬MACRO delayMs(time)
// ¬NOTE user must implement function to override the weakly defined delay_ms()
#if defined(_FUNSAPE_PLATFORM_AVR)
#   define delayMs(time)                _delay_ms(time)
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#       define delayMs(time)            HAL_Delay(time)
#   else
#       define delayMs(time)            delay_ms(time)
#   endif
#else
#   define delayMs(time)                delay_ms(time)
#endif

// ¬MACRO delayUs(time)
// ¬NOTE user must implement function to override the weakly defined delay_us()
#if defined(_FUNSAPE_PLATFORM_AVR)
#   define delayUs(time)                _delay_us(time)
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#       define delayUs(time)            delay_us(time)
#   else
#       define delayUs(time)            delay_us(time)
#   endif
#else
#   define delayUs(time)                delay_us(time)
#endif

// ¬MACRO getTick()
// ¬NOTE user must implement function to override the weakly defined get_tick()
#if defined(_FUNSAPE_PLATFORM_AVR)
#   define getTick()                    get_tick()
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
#   if defined(USE_HAL_DRIVER)
#       define getTick()                HAL_GetTick()
#   else
#       define getTick()                get_tick()
#   endif
#else
#   define getTick()                    get_tick()
#endif

// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// Single variables ------------------------------------------------------------
//     => Shortening the name of single data types -----------------------------

// ¬TYPEDEF bool_t
typedef bool                            bool_t;
// ¬TYPEDEF cbool_t
typedef const bool                      cbool_t;
// ¬TYPEDEF cint8_t
typedef const int8_t                    cint8_t;
// ¬TYPEDEF cint16_t
typedef const int16_t                   cint16_t;
// ¬TYPEDEF cint32_t
typedef const int32_t                   cint32_t;
// ¬TYPEDEF cint64_t
typedef const int64_t                   cint64_t;
// ¬TYPEDEF cuint8_t
typedef const uint8_t                   cuint8_t;
// ¬TYPEDEF cuint16_t
typedef const uint16_t                  cuint16_t;
// ¬TYPEDEF cuint32_t
typedef const uint32_t                  cuint32_t;
// ¬TYPEDEF cuint64_t
typedef const uint64_t                  cuint64_t;
// ¬TYPEDEF vbool_t
typedef volatile bool                   vbool_t;
// ¬TYPEDEF vint8_t
typedef volatile int8_t                 vint8_t;
// ¬TYPEDEF vint16_t
typedef volatile int16_t                vint16_t;
// ¬TYPEDEF vint32_t
typedef volatile int32_t                vint32_t;
// ¬TYPEDEF vint64_t
typedef volatile int64_t                vint64_t;
// ¬TYPEDEF vuint8_t
typedef volatile uint8_t                vuint8_t;
// ¬TYPEDEF vuint16_t
typedef volatile uint16_t               vuint16_t;
// ¬TYPEDEF vuint32_t
typedef volatile uint32_t               vuint32_t;
// ¬TYPEDEF vuint64_t
typedef volatile uint64_t               vuint64_t;
#if defined(_FUNSAPE_PLATFORM_AVR)
typedef int16_t                         int_t;      // 16-bit (AVR)
typedef uint16_t                        uint_t;     // 16-bit (AVR)
typedef const int16_t                   cint_t;     // 16-bit (AVR)
typedef const uint16_t                  cuint_t;    // 16-bit (AVR)
typedef volatile int16_t                vint_t;     // 16-bit (AVR)
typedef volatile uint16_t               vuint_t;    // 16-bit (AVR)
typedef volatile uint8_t                reg_t;      // 8-bit (AVR)
typedef volatile uint8_t                ioReg_t;    // 8-bit (AVR)
typedef uint8_t                         ioPin_t;    // 8-bit (AVR)
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
// ¬TYPEDEF int_t
typedef int32_t                         int_t;      // 32-bit (ARM)
// ¬TYPEDEF uint_t
typedef uint32_t                        uint_t;     // 32-bit (ARM)
// ¬TYPEDEF cint_t
typedef const int32_t                   cint_t;     // 32-bit (ARM)
// ¬TYPEDEF cuint_t
typedef const uint32_t                  cuint_t;    // 32-bit (ARM)
// ¬TYPEDEF vint_t
typedef volatile int32_t                vint_t;     // 32-bit (ARM)
// ¬TYPEDEF vuint_t
typedef volatile uint32_t               vuint_t;    // 32-bit (ARM)
// ¬TYPEDEF reg_t
typedef volatile uint32_t               reg_t;      // 32-bit (ARM)
// ¬TYPEDEF ioPin_t
typedef uint16_t                        ioPin_t;    // 16-bit (ARM)
#   if defined(USE_HAL_DRIVER)
// ¬TYPEDEF ioReg_t
typedef GPIO_TypeDef                    *ioReg_t;   // 32-bit (ARM)
#   else
typedef volatile uint32_t               ioReg_t;    // 32-bit (ARM)
#   endif
#endif

// -----------------------------------------------------------------------------
// Single variables ------------------------------------------------------------
//     => Small enumerations understood as logic values ------------------------

/**
// ¬ENUM Activation
 * @brief
 *
 * Detailed description of this element.
 *
 */

enum class Activation : bool_t {
    OFF                                 = false,
    ON                                  = true
};

// ¬ENUM DriverMode
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class DriverMode : bool_t {
    PUSH_PULL                           = false,
    OPEN_DRAIN                          = true,
};

// ¬ENUM Edge
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class Edge : bool_t {
    FALLING                             = false,
    RISING                              = true
};

// ¬ENUM IoOperation
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class IoOperation : bool_t {
    WRITE                               = false,
    READ                                = true
};

// ¬ENUM LogicLevel
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class LogicLevel : bool_t {
    LOW                                 = false,
    HIGH                                = true
};

// ¬ENUM PinDirection
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class PinDirection : bool_t {
    INPUT                               = false,
    OUTPUT                              = true,
};

// ¬ENUM Rotation
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class Rotation : bool_t {
    CLOCKWISE                           = false,
    COUNTERCLOCKWISE                    = true
};

// ¬ENUM State
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class State : bool_t {
    OFF                                 = false,
    ON                                  = true
};

// -----------------------------------------------------------------------------
// Single variables ------------------------------------------------------------
//     => Small enumerations that can be understood as a single variable -------

// ¬ENUM Axis
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class Axis : uint8_t {
    //                                     Z-AXIS     Y-AXIS     X-AXIS
    NONE                                = (0 << 2) | (0 << 1) | (0 << 0),
    X                                   = (0 << 2) | (0 << 1) | (1 << 0),
    Y                                   = (0 << 2) | (1 << 1) | (0 << 0),
    XY                                  = (0 << 2) | (1 << 1) | (1 << 0),
    Z                                   = (1 << 2) | (0 << 1) | (0 << 0),
    XZ                                  = (1 << 2) | (0 << 1) | (1 << 0),
    YZ                                  = (1 << 2) | (1 << 1) | (0 << 0),
    ALL                                 = (1 << 2) | (1 << 1) | (1 << 0)
};

inlined Axis operator|(Axis a, Axis b)
{
    return static_cast<Axis>(static_cast<int>(a) | static_cast<int>(b));
}

inlined Axis &operator|=(Axis &a, Axis b)
{
    return a = static_cast<Axis>(static_cast<int>(a) | static_cast<int>(b));
}

inlined Axis operator&(Axis a, Axis b)
{
    return static_cast<Axis>(static_cast<int>(a) & static_cast<int>(b));
}

inlined Axis &operator&=(Axis &a, Axis b)
{
    return a = static_cast<Axis>(static_cast<int>(a) & static_cast<int>(b));
}

// ¬ENUM Direction
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class Direction : uint8_t {
    LEFT                                = 0,
    RIGHT                               = 1,
    UP                                  = 2,
    DOWN                                = 3
};

// ¬ENUM InterfaceMode
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class InterfaceMode : uint8_t {
    BLOCKING_MODE                       = 0,
    INTERRUPT_MODE                      = 1
#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
    ,
    DMA_MODE                            = 2
#endif
};

// ¬ENUM DisplayOrientation
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class DisplayOrientation : bool_t {
    LANDSCAPE                           = false,
    PORTRAIT                            = true
};

// ¬ENUM ThermometricScale
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class ThermometricScale : uint8_t {
    CELSIUS                             = 0,
    DELISLE                             = 1,
    FAHRENHEIT                          = 2,
    KELVIN                              = 3,
    NEWTON                              = 4,
    RANKINE                             = 5,
    REAUMUR                             = 6,
    ROMER                               = 7
};

// -----------------------------------------------------------------------------
// Enumerations ----------------------------------------------------------------
//     => Large enumerations or enumerations with expansible number of elements

// ¬ENUM Error
/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
enum class Error : uint16_t {
    // General error codes
    NONE                                                = 0x0000,   // Success - No errors occurred
    AXIS_INVALID                                        = 0x0001,   // Invalid axis
    BUSY                                                = 0x0002,   // TODO: Describe parameter
    DEVICE_NOT_SUPPORTED                                = 0x0003,   // Device is not currently supported
    FEATURE_NOT_SUPPORTED                               = 0x0004    // Unsupported feature or configuration
    , FUNCTION_POINTER_NULL                             = 0x0005,   // TODO: Describe parameter
    INSTANCE_INVALID                                    = 0x0006,   // Invalid instance
    LOCKED                                              = 0x0007,   // Accessed a locked device
    MEMORY_ALLOCATION_FAILED                            = 0x0008,   // Memory allocation failed
    MODE_NOT_SUPPORTED                                  = 0x0009,   // Mode is not currently supported
    NOT_INITIALIZED                                     = 0x000A,   // Not initialized
    NOT_READY                                           = 0x000B,   // TODO: Describe parameter
    READ_PROTECTED                                      = 0x000C,   // Tried to read a read protected device
    UNDER_DEVELOPMENT                                   = 0x000D,   // This part of the code is still under development
    WRITE_PROTECTED                                     = 0x000E,   // Tried to write a write protected device

    // Function arguments related error codes
    ARGUMENT_CANNOT_BE_ZERO                             = 0x0100,   // TODO: Describe parameter
    ARGUMENT_POINTER_NULL                               = 0x0101,   // NULL pointer was passed as an argument to function
    ARGUMENT_VALUE_INVALID                              = 0x0102,   // TODO: Describe parameter
    ARGUMENTS_MISMATCH                                  = 0x0103,   // A function was called with dubious argument values
    ARGUMENTS_WRONG_NUMBER                              = 0x0104,   // A function was called with wrong number of arguments
//    ARGUMENT_GENERIC_ERROR

    // Buffer related error codes
    BUFFER_EMPTY                                        = 0x0200,   // Buffer is empty
    BUFFER_FULL                                         = 0x0201,   // Buffer is full
    BUFFER_NOT_ENOUGH_ELEMENTS                          = 0x0202,   // Not enough space in buffer to perform operation
    BUFFER_NOT_ENOUGH_SPACE                             = 0x0203,   // Not enough space in buffer to perform operation
    BUFFER_POINTER_NULL                                 = 0x0204,   // Buffer size was set to zero
    BUFFER_SIZE_TOO_LARGE                               = 0x0205,   // Buffer size was set to a large value
    BUFFER_SIZE_TOO_SMALL                               = 0x0206,   // Buffer size was set to a very small value
    BUFFER_SIZE_ZERO                                    = 0x0207,   // Buffer size was set to zero
//    BUFFER_GENERIC_ERROR

    // Clock configuration related error codes
    CLOCK_PRESCALER_CHANGE_FAILED                       = 0x0300,   // Clock prescaler change failed
    CLOCK_PRESCALER_NOT_SUPPORTED                       = 0x0301,   // Clock prescaler configuration unsupported
    CLOCK_SPEED_TOO_HIGH                                = 0x0302,   // TODO: Describe parameter
    CLOCK_SPEED_TOO_LOW                                 = 0x0303,   // TODO: Describe parameter
//    CLOCK_GENERIC_ERROR

    // Communication related error codes
    COMMUNICATION_FAILED                                = 0x0400,   // TODO: Describe parameter
    COMMUNICATION_NO_DEVICE_SELECTED                    = 0x0401,   // TODO: Describe parameter
    COMMUNICATION_PORT_NOT_SET                          = 0x0402,   // TODO: Describe parameter
    COMMUNICATION_TIMEOUT                               = 0x0403,   // The opereation timed out
//    COMMUNICATION_DEVICE_ID_MATCH_FAILED
//    COMMUNICATION_GENERIC_ERROR

    // Communication protocol related error codes
    CHECKSUM_ERROR                                      = 0x0500,   // TODO: Describe parameter
    FRAME_ERROR                                         = 0x0501,   // TODO: Describe parameter
    PACKAGE_AWAITING                                    = 0x0502,   // Try to write data to a ready package
    PACKAGE_NOT_READY                                   = 0x0503,   // Try to read data from a not ready package
//    PROTOCOL_GENERIC_ERROR

    // DateTime class related error codes
    DATETIME_DATE_NOT_INITIALIZED                       = 0x0600,   // TODO: Describe parameter
    DATETIME_HOUR_INVALID                               = 0x0601,   // TODO: Describe parameter
    DATETIME_MILLISECOND_INVALID                        = 0x0602,   // TODO: Describe parameter
    DATETIME_MINUTE_INVALID                             = 0x0603,   // TODO: Describe parameter
    DATETIME_MONTH_DAY_INVALID                          = 0x0604,   // TODO: Describe parameter
    DATETIME_MONTH_INVALID                              = 0x0605,   // TODO: Describe parameter
    DATETIME_SECOND_INVALID                             = 0x0606,   // TODO: Describe parameter
    DATETIME_TIME_NOT_INITIALIZED                       = 0x0607,   // TODO: Describe parameter
    DATETIME_TIMEZONE_INVALID                           = 0x0608,   // TODO: Describe parameter
    DATETIME_YEAR_INVALID                               = 0x0609,   // TODO: Describe parameter
    DATETIME_DATE_INVALID                               = 0x060A,   // TODO: Describe parameter
    DATETIME_TIME_INVALID                               = 0x060B,   // TODO: Describe parameter
//    DATETIME_GENERIC_ERROR

    // Handler related error codes
    HANDLER_NOT_SUPPORTED                               = 0x0700,   // TODO: Describe parameter
    HANDLER_POINTER_NULL                                = 0x0701,   // TODO: Describe parameter
//    HANDLER_GENERIC_ERROR

    // External interrupts related error codes
    INTERRUPT_NOT_AVAILABLE                             = 0x0800,   // TODO: Describe parameter
    INTERRUPT_PORT_OUTPUT_MODE_NOT_SUPPORTED            = 0x0801,   // TODO: Describe parameter
    INTERRUPT_SENSE_MODE_NOT_SUPPORTED                  = 0x0802,   // TODO: Describe parameter
//    INTERRUPT_GENERIC_ERROR

    // Package API class related error codes
    PACKAGE_API_PACKAGE_DATA_FULL                       = 0x0900,   // TODO: Describe parameter
    PACKAGE_API_PACKAGE_NOT_READY                       = 0x0901,   // TODO: Describe parameter
    PACKAGE_API_PAYLOAD_DATA_NULL                       = 0x0902,   // TODO: Describe parameter
    PACKAGE_API_PAYLOAD_SIZE_NULL                       = 0x0903,   // TODO: Describe parameter
    PACKAGE_API_SOURCE_ADDRESS_INVALID                  = 0x0904,   // TODO: Describe parameter
//    PACKAGE_API_GENERIC_ERROR

    // SPI related error codes
    SPI_BUSY_FLAG                                       = 0x0A00,   // TODO: Describe parameter
    SPI_CRC                                             = 0x0A01,   // TODO: Describe parameter
    SPI_DMA                                             = 0x0A02,   // TODO: Describe parameter
    SPI_MODE_FAULT                                      = 0x0A03,   // TODO: Describe parameter
    SPI_OVERRUN                                         = 0x0A04,   // TODO: Describe parameter
//    SPI_GENERIC_ERROR

    // TWI related error codes
    TWI_AF_FLAG                                         = 0x0B00,   // TWI AF flag error
    TWI_ARBITRATION_LOST                                = 0x0B01,   // TWI bus arbitration lost
    TWI_BUS_ERROR                                       = 0x0B02,   // TWI bus error
    TWI_DMA                                             = 0x0B03,   // DMA parameter error
    TWI_DMA_PARAM                                       = 0x0B04,   // DMA transfer error
    TWI_OVERRUN                                         = 0x0B05,   // TWI overrun
    TWI_SIZE_MANAGEMENT_ERROR                           = 0x0B06,   // Size management error
//    TWI_INVALID_DEVICE_ADDRESS
//    TWI_GENERIC_ERROR

    // USART related error codes
    USART_BAUD_RATE_NOT_SUPPORTED                       = 0x0C00,   // TODO: Describe parameter
    USART_CONFIGURATION_PARAMETER_WRONG                 = 0x0C01,   // TODO: Describe parameter
    USART_DATA_BITS_NOT_SUPPORTED                       = 0x0C02,   // TODO: Describe parameter
    USART_FRAME_FORMAT_NOT_SUPPORTED                    = 0x0C03,   // TODO: Describe parameter
    USART_PARITY_NOT_SUPPORTED                          = 0x0C04,   // TODO: Describe parameter
    USART_STOP_BIT_NOT_SUPPORTED                        = 0x0C05,   // TODO: Describe parameter
//    USART_GENERIC_ERROR

    // GPIO related error codes
    GPIO_NO_PIN_SET                                     = 0x0D00,   // No pin selected at Gpio peripheral
    GPIO_PORT_INVALID                                   = 0x0D01,   // Invalid GPIO Port
    GPIO_PIN_INVALID                                    = 0x0D02,   // Invalid GPIO Pin

    // Uncategorized error codes
    LCD_PORTS_NOT_SET                                   = 0xFF00,
    CONTROLLER_NOT_SUPPORTED                            = 0xFF00,   // Unsupported controller
    DEVICE_ID_MATCH_FAILED                              = 0xFF01,   // Device ID doesn't match
    DMA_NOT_SUPPORTED                                   = 0xFF02,   // DMA interface mode is not supported for this module
    DMA_TRANSFER_ERROR                                  = 0xFF03,   // DMA transfer error
    MESSAGE_TOO_LONG                                    = 0xFF04,   // Message is to long to be stored inside buffer
    VALID_DATA_NOT_AVAILABLE                            = 0xFF05,   // Valid data was unavailable
//    PERIPHERAL_NOT_READY
//    STOPWATCH_NOT_STARTED

    // "I don't have the slightest idea" error code
    UNKNOWN                                             = 0xFFFF,   // Unknown error - Ops!!!!
};

// -----------------------------------------------------------------------------
// Flags -----------------------------------------------------------------------
//     => Enumerations that can be combined in the form of flags ---------------

// NONE

// -----------------------------------------------------------------------------
// Structures, Unions and Bit Fields -------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Classes ---------------------------------------------------------------------

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// Inline functions declaration
// =============================================================================

// -----------------------------------------------------------------------------
// Checking data validity ------------------------------------------------------

inlined bool_t isAsciiLowerCaseLetter(char character_p);
inlined bool_t isAsciiUpperCaseLetter(char character_p);
inlined bool_t isAsciiCommand(char character_p);
inlined bool_t isAsciiLetter(char character_p);
inlined bool_t isAsciiNumber(char character_p);
inlined bool_t isAsciiSymbol(char character_p);

// -----------------------------------------------------------------------------
// System actions --------------------------------------------------------------

inlined void die(Error errorCode_p);
// inlined void delayMs(cuint16_t time_p);
// inlined void delayUs(cuint16_t time_p);
inlined void doNop(void);
inlined void doNothing(void);
// inlined uint32_t getTick(void);
inlined void systemHalt(void);

// =============================================================================
// Public functions declarations
// =============================================================================

// NONE

// =============================================================================
// Public functions weakly defined
// =============================================================================

weakened void delay_ms(uint16_t time);
weakened void delay_us(uint16_t time);
weakened uint32_t get_tick(void);

// =============================================================================
// Inline functions definition
// =============================================================================

// -----------------------------------------------------------------------------
// Checking data validity ------------------------------------------------------

// ¬FUNCTION isAsciiLowerCaseLetter
inlined bool_t isAsciiLowerCaseLetter(char character_p)
{
    return ((bool_t)(((character_p >= 'a') && (character_p <= 'z')) ? true : false));
}

// ¬FUNCTION isAsciiUpperCaseLetter(char)
inlined bool_t isAsciiUpperCaseLetter(char character_p)
{
    return ((bool_t)(((character_p >= 'A') && (character_p <= 'Z')) ? true : false));
}

// ¬FUNCTION isAsciiCommand(char)
inlined bool_t isAsciiCommand(char character_p)
{
    return ((bool_t)((character_p < ' ') ? true : false));
}

// ¬FUNCTION isAsciiLetter(char)
inlined bool_t isAsciiLetter(char character_p)
{
    return ((bool_t)((isAsciiLowerCaseLetter(character_p) || (isAsciiUpperCaseLetter(character_p)) ? true : false)));
}

// ¬FUNCTION isAsciiNumber(char)
inlined bool_t isAsciiNumber(char character_p)
{
    return ((bool_t)(((character_p >= '0') && (character_p <= '9')) ? true : false));
}

// ¬FUNCTION isAsciiSymbol(char)
inlined bool_t isAsciiSymbol(char character_p)
{
    return ((bool_t)(((!isAsciiNumber(character_p)) && (!isAsciiCommand(character_p)) &&
                                    (!isAsciiLetter(character_p))) ? true : false));
}

// -----------------------------------------------------------------------------
// System actions --------------------------------------------------------------

// ¬FUNCTION die(Error)
inlined void die(Error errorCode_p)
{
    printf("Failed with error=%u.\r", (cuint16_t)errorCode_p);
    systemHalt();

    return;
}

// ¬FUNCTION doNop()
inlined void doNop(void)
{
    asm volatile("nop");

    return;
}

// ¬FUNCTION doNothing()
inlined void doNothing(void)
{
    {}

    return;
}

// ¬FUNCTION systemHalt()
inlined void systemHalt(void)
{
    while (1) {
    }

    return;
}

// =============================================================================
// Includes Low Level Abstration Layer
// =============================================================================

// NONE

#endif // __FUNSAPE_LIB_GLOBAL_DEFINES_HPP

// =============================================================================
// END OF FILE
// =============================================================================
