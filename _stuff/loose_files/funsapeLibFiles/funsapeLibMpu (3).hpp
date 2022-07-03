/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibMpu.hpp
 * Module:          MPU module interface for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_LIST__
 * Build:           __BUILD_NUMBER__
 * Last edition:    __CURRENT_DATE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_MPU_HPP
#define __FUNSAPE_LIB_MPU_HPP                   __BUILD_NUMBER__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error   [funsapeLibMpu.hpp] Error 12 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_NUMBER__
#   error   [funsapeLibMpu.hpp] Error 13 - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibMpu.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../src/funsapeLibBus.hpp"
#ifndef __FUNSAPE_LIB_BUS_HPP
#   error   [funsapeLibMpu.hpp] Error 14 - Required module (funsapeLibBus.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_BUS_HPP != __BUILD_NUMBER__
#   error   [funsapeLibMpu.hpp] Error 15 - Build mismatch between required module (funsapeLibBus.hpp) and current header file (funsapeLibMpu.hpp)!
#endif

#include "../src/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#   error   [funsapeLibMpu.hpp] Error 14 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_NUMBER__
#   error   [funsapeLibMpu.hpp] Error 15 - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibMpu.hpp)!
#endif

#include "../funsapeLibGpio.hpp"
#ifndef __FUNSAPE_LIB_GPIO_HPP
#   error   [funsapeLibMpu.hpp] Error 14 - Required module (funsapeLibGpio.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GPIO_HPP != __BUILD_NUMBER__
#   error   [funsapeLibMpu.hpp] Error 15 - Build mismatch between required module (funsapeLibGpio.hpp) and current header file (funsapeLibMpu.hpp)!
#endif

#include "../util/Int32Q16.hpp"

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
// Mpu - Class declaration
// =============================================================================

class Mpu
{
    // -------------------------------------------------------------------------
    // New data types ----------------------------------------------------------

public:
    //     ////////////     ACCELEROMETER DIGITAL FILTER     ////////////     //
    enum class AccelDlpf : uint8_t {
        DLPF_OFF                        = 0,
        DLPF_218_HZ                     = 1,
        DLPF_99_HZ                      = 2,
        DLPF_44_HZ                      = 3,
        DLPF_21_HZ                      = 4,
        DLPF_10_HZ                      = 5,
        DLPF_5_HZ                       = 6,
        DLPF_420_HZ                     = 7,
    };
    //     //////////////     ACCELEROMETER FULL SCALE     //////////////     //
    enum class AccelFullScale : uint8_t {
        FS_2G                           = 0,
        FS_4G                           = 1,
        FS_8G                           = 2,
        FS_16G                          = 3,
    };
    //     //////////////     EXTERNAL SYNCHRONIZATION     //////////////     //
    enum class ExternalSync : uint8_t {
        OFF                             = 0,
        TEMP_OUT_L                      = 1,
        GYRO_XOUT_L                     = 2,
        GYRO_YOUT_L                     = 3,
        GYRO_ZOUT_L                     = 4,
        ACCEL_XOUT_L                    = 5,
        ACCEL_YOUT_L                    = 6,
        ACCEL_ZOUT_L                    = 7,
    };
    //     //////////////     GYROSCOPE DIGITAL FILTER     //////////////     //
    enum class GyroDlpf : uint8_t {
        DLPF_OFF                        = 0,
        DLPF_188_HZ                     = 1,
        DLPF_98_HZ                      = 2,
        DLPF_42_HZ                      = 3,
        DLPF_20_HZ                      = 4,
        DLPF_10_HZ                      = 5,
        DLPF_5_HZ                       = 6,
    };
    //     ////////////////     GYROSCOPE FULL SCALE     ////////////////     //
    enum class GyroFullScale : uint8_t {
        FS_250_DPS                      = 0,
        FS_500_DPS                      = 1,
        FS_1000_DPS                     = 2,
        FS_2000_DPS                     = 3,
    };
    //     ///////////////////    INTERRUPT FLAGS     ///////////////////     //
    typedef enum InterruptFlags : uint8_t {
        NONE                            = 0x00,
        FIFO_OVERFLOW                   = 0x01,
        FSYNC                           = 0x02,
        RAW_DATA_READY                  = 0x04,
        WAKE_UP_ON_MOTION               = 0x08,
        ALL                             = 0x0F
    } InterruptFlags;
    //     ///////////////////////    PART ID     ///////////////////////     //
    enum class PartId : uint8_t {
        MPU6050                         = 0,
        MPU6500                         = 1,
        MPU9150                         = 2,
        MPU9250                         = 3,
    };

private:
    //     ////////////////////    BIT POSITIONS     ////////////////////     //
    enum class Bit : uint8_t {
        SELF_TEST_X_GYRO                = 0,
        SELF_TEST_Y_GYRO                = 0,
        SELF_TEST_Z_GYRO                = 0,

        SELF_TEST_X_ACCEL               = 0,
        SELF_TEST_Y_ACCEL               = 0,
        SELF_TEST_Z_ACCEL               = 0,

        XG_OFFSET_H                     = 0,
        XG_OFFSET_L                     = 0,
        YG_OFFSET_H                     = 0,
        YG_OFFSET_L                     = 0,
        ZG_OFFSET_H                     = 0,
        ZG_OFFSET_L                     = 0,

        SMPLRT_DIV                      = 0,

        CONFIG_FIFO_MODE                = 6,
        CONFIG_EXT_SYNC_SET             = 3,
        CONFIG_DLPF_CFG                 = 0,

        GYRO_CONFIG_XGYRO_CTEN          = 7,
        GYRO_CONFIG_YGYRO_CTEN          = 6,
        GYRO_CONFIG_ZGYRO_CTEN          = 5,
        GYRO_CONFIG_GYRO_FS_SEL         = 3,
        GYRO_CONFIG_FCHOICE_B           = 0,

        ACCEL_CONFIG_AX_ST_EN           = 7,
        ACCEL_CONFIG_AY_ST_EN           = 6,
        ACCEL_CONFIG_AZ_ST_EN           = 5,
        ACCEL_CONFIG_ACCEL_FS_SEL       = 3,

        ACCEL_CONFIG2_ACCEL_FCHOICE_B   = 3,
        ACCEL_CONFIG2_A_DLPFCFG         = 0,

        LP_ACCEL_ODR_LPOSC_CLKSEL       = 0,

        WOM_THR                         = 0,

        FIFO_EN_TEMP_OUT                = 7,
        FIFO_EN_GYRO_XOUT               = 6,
        FIFO_EN_GYRO_YOUT               = 5,
        FIFO_EN_GYRO_ZOUT               = 4,
        FIFO_EN_ACCEL                   = 3,
        FIFO_EN_SLV_2                   = 2,
        FIFO_EN_SLV_1                   = 1,
        FIFO_EN_SLV_0                   = 0,

        I2C_MST_CTRL_MULT_MST_EN        = 7,
        I2C_MST_CTRL_WAIT_FOR_ES        = 6,
        I2C_MST_CTRL_SLV_3_FIFO_EN      = 5,
        I2C_MST_CTRL_I2C_MST_P_NSR      = 4,
        I2C_MST_CTRL_I2C_MST_CLK        = 0,

        I2C_SLV0_ADDR_I2C_SLV0_RNW      = 7,
        I2C_SLV0_ADDR_I2C_ID_0          = 0,

        I2C_SLV0_REG                    = 0,

        I2C_SLV0_CTRL_I2C_SLV0_EN       = 7,
        I2C_SLV0_CTRL_I2C_SLV0_BYTE_SW  = 6,
        I2C_SLV0_CTRL_I2C_SLV0_REG_DIS  = 5,
        I2C_SLV0_CTRL_I2C_SLV0_GRP      = 4,
        I2C_SLV0_CTRL_I2C_SLV0_LENG     = 0,

        I2C_SLV1_ADDR_I2C_SLV1_RNW      = 7,
        I2C_SLV1_ADDR_I2C_ID_1          = 0,

        I2C_SLV1_REG                    = 0,

        I2C_SLV1_CTRL_I2C_SLV1_EN       = 7,
        I2C_SLV1_CTRL_I2C_SLV1_BYTE_SW  = 6,
        I2C_SLV1_CTRL_I2C_SLV1_REG_DIS  = 5,
        I2C_SLV1_CTRL_I2C_SLV1_GRP      = 4,
        I2C_SLV1_CTRL_I2C_SLV1_LENG     = 0,

        I2C_SLV2_ADDR_I2C_SLV2_RNW      = 7,
        I2C_SLV2_ADDR_I2C_ID_2          = 0,

        I2C_SLV2_REG                    = 0,

        I2C_SLV2_CTRL_I2C_SLV2_EN       = 7,
        I2C_SLV2_CTRL_I2C_SLV2_BYTE_SW  = 6,
        I2C_SLV2_CTRL_I2C_SLV2_REG_DIS  = 5,
        I2C_SLV2_CTRL_I2C_SLV2_GRP      = 4,
        I2C_SLV2_CTRL_I2C_SLV2_LENG     = 0,

        I2C_SLV3_ADDR_I2C_SLV3_RNW      = 7,
        I2C_SLV3_ADDR_I2C_ID_3          = 0,

        I2C_SLV3_REG                    = 0,

        I2C_SLV3_CTRL_I2C_SLV3_EN       = 7,
        I2C_SLV3_CTRL_I2C_SLV3_BYTE_SW  = 6,
        I2C_SLV3_CTRL_I2C_SLV3_REG_DIS  = 5,
        I2C_SLV3_CTRL_I2C_SLV3_GRP      = 4,
        I2C_SLV3_CTRL_I2C_SLV3_LENG     = 0,

        I2C_SLV4_ADDR_I2C_SLV4_RNW      = 7,
        I2C_SLV4_ADDR_I2C_ID_4          = 0,

        I2C_SLV4_REG                    = 0,

        I2C_SLV4_DO                     = 0,

        I2C_SLV4_CTRL_I2C_SLV4_EN       = 7,
        I2C_SLV4_CTRL_SLV4_DONE_INT_EN  = 6,
        I2C_SLV4_CTRL_I2C_SLV4_REG_DIS  = 5,
        I2C_SLV4_CTRL_I2C_MST_DLY       = 0,

        I2C_SLV4_DI                     = 0,

        I2C_MST_STATUS_PASS_THROUGH     = 7,
        I2C_MST_STATUS_I2C_SLV4_DONE    = 6,
        I2C_MST_STATUS_I2C_LOST_ARB     = 5,
        I2C_MST_STATUS_I2C_SLV4_NACK    = 4,
        I2C_MST_STATUS_I2C_SLV3_NACK    = 3,
        I2C_MST_STATUS_I2C_SLV2_NACK    = 2,
        I2C_MST_STATUS_I2C_SLV1_NACK    = 1,
        I2C_MST_STATUS_I2C_SLV0_NACK    = 0,

        INT_PIN_CFG_ACTL                = 7,
        INT_PIN_CFG_OPEN                = 6,
        INT_PIN_CFG_LATCH_INT_EN        = 5,
        INT_PIN_CFG_INT_ANYRD_2CLEAR    = 4,
        INT_PIN_CFG_ACTL_FSYNC          = 3,
        INT_PIN_CFG_FSYNC_INT_MODE_EN   = 2,
        INT_PIN_CFG_BYPASS_EN           = 1,

        INT_ENABLE_WOM_EN               = 6,
        INT_ENABLE_FIFO_OVERFLOW_EN     = 4,
        INT_ENABLE_FSYNC_INT_EN         = 3,
        INT_ENABLE_RAW_RDY_EN           = 0,

        INT_STATUS_WOM_INT              = 6,
        INT_STATUS_FIFO_OVERFLOW_INT    = 4,
        INT_STATUS_FSYNC_INT            = 3,
        INT_STATUS_RAW_DATA_RDY_INT     = 0,

        ACCEL_XOUT_H                    = 0,
        ACCEL_XOUT_L                    = 0,
        ACCEL_YOUT_H                    = 0,
        ACCEL_YOUT_L                    = 0,
        ACCEL_ZOUT_H                    = 0,
        ACCEL_ZOUT_L                    = 0,
        TEMP_OUT_H                      = 0,
        TEMP_OUT_L                      = 0,
        GYRO_XOUT_H                     = 0,
        GYRO_XOUT_L                     = 0,
        GYRO_YOUT_H                     = 0,
        GYRO_YOUT_L                     = 0,
        GYRO_ZOUT_H                     = 0,
        GYRO_ZOUT_L                     = 0,

        EXT_SENS_DATA_00                = 0,
        EXT_SENS_DATA_01                = 0,
        EXT_SENS_DATA_02                = 0,
        EXT_SENS_DATA_03                = 0,
        EXT_SENS_DATA_04                = 0,
        EXT_SENS_DATA_05                = 0,
        EXT_SENS_DATA_06                = 0,
        EXT_SENS_DATA_07                = 0,
        EXT_SENS_DATA_08                = 0,
        EXT_SENS_DATA_09                = 0,
        EXT_SENS_DATA_10                = 0,
        EXT_SENS_DATA_11                = 0,
        EXT_SENS_DATA_12                = 0,
        EXT_SENS_DATA_13                = 0,
        EXT_SENS_DATA_14                = 0,
        EXT_SENS_DATA_15                = 0,
        EXT_SENS_DATA_16                = 0,
        EXT_SENS_DATA_17                = 0,
        EXT_SENS_DATA_18                = 0,
        EXT_SENS_DATA_19                = 0,
        EXT_SENS_DATA_20                = 0,
        EXT_SENS_DATA_21                = 0,
        EXT_SENS_DATA_22                = 0,
        EXT_SENS_DATA_23                = 0,

        I2C_SLV0_DO                     = 0,
        I2C_SLV1_DO                     = 0,
        I2C_SLV2_DO                     = 0,
        I2C_SLV3_DO                     = 0,

        I2C_MST_DELAY_CTRL_DELAY_ES_SHADOW  = 7,
        I2C_MST_DELAY_CTRL_I2C_SLV4_DLY_EN  = 4,
        I2C_MST_DELAY_CTRL_I2C_SLV3_DLY_EN  = 3,
        I2C_MST_DELAY_CTRL_I2C_SLV2_DLY_EN  = 2,
        I2C_MST_DELAY_CTRL_I2C_SLV1_DLY_EN  = 1,
        I2C_MST_DELAY_CTRL_I2C_SLV0_DLY_EN  = 0,

        SIGNAL_PATH_RESET_GYRO_RST      = 2,
        SIGNAL_PATH_RESET_ACCEL_RST     = 1,
        SIGNAL_PATH_RESET_TEMP_RST      = 0,

        ACCEL_INTEL_CTRL_MOT_ACCEL_INTEL_EN     = 7,
        ACCEL_INTEL_CTRL_MOT_ACCEL_INTEL_MODE   = 6,

        USER_CTRL_FIFO_EN               = 6,
        USER_CTRL_I2C_MST_EN            = 5,
        USER_CTRL_I2C_IF_DIS            = 4,
        USER_CTRL_FIFO_RST              = 2,
        USER_CTRL_I2C_MST_RST           = 1,
        USER_CTRL_SIG_COND_RST          = 0,

        PWR_MGMT_1_H_RESET              = 7,
        PWR_MGMT_1_SLEEP                = 6,
        PWR_MGMT_1_CYCLE                = 5,
        PWR_MGMT_1_GYRO_STANDBY         = 4,
        PWR_MGMT_1_PD_PTAT              = 3,
        PWR_MGMT_1_CLKSEL               = 0,

        PWR_MGMT_2_DISABLE_XA           = 5,
        PWR_MGMT_2_DISABLE_YA           = 4,
        PWR_MGMT_2_DISABLE_ZA           = 3,
        PWR_MGMT_2_DISABLE_XG           = 2,
        PWR_MGMT_2_DISABLE_YG           = 1,
        PWR_MGMT_2_DISABLE_ZG           = 0,

        FIFO_COUNTH_FIFO_CNT            = 0,
        FIFO_COUNTL_FIFO_CNT            = 0,

        FIFO_R_W                        = 0,

        WHO_AM_I                        = 0,

        XA_OFFSET_H                     = 0,
        XA_OFFSET_L                     = 1,
        YA_OFFSET_H                     = 0,
        YA_OFFSET_L                     = 1,
        ZA_OFFSET_H                     = 0,
        ZA_OFFSET_L                     = 1,
    };
    //     //////////////////////     BIT MASK     //////////////////////     //
    enum class BitMask : uint8_t {
        SELF_TEST_X_GYRO                = 0xFF,
        SELF_TEST_Y_GYRO                = 0xFF,
        SELF_TEST_Z_GYRO                = 0xFF,

        SELF_TEST_X_ACCEL               = 0xFF,
        SELF_TEST_Y_ACCEL               = 0xFF,
        SELF_TEST_Z_ACCEL               = 0xFF,

        XG_OFFSET_H                     = 0xFF,
        XG_OFFSET_L                     = 0xFF,
        YG_OFFSET_H                     = 0xFF,
        YG_OFFSET_L                     = 0xFF,
        ZG_OFFSET_H                     = 0xFF,
        ZG_OFFSET_L                     = 0xFF,

        SMPLRT_DIV                      = 0xFF,

        CONFIG_FIFO_MODE                = 0x01,
        CONFIG_EXT_SYNC_SET             = 0x07,
        CONFIG_DLPF_CFG                 = 0x07,

        GYRO_CONFIG_XGYRO_CTEN          = 0x01,
        GYRO_CONFIG_YGYRO_CTEN          = 0x01,
        GYRO_CONFIG_ZGYRO_CTEN          = 0x01,
        GYRO_CONFIG_GYRO_FS_SEL         = 0x03,
        GYRO_CONFIG_FCHOICE_B           = 0x03,

        ACCEL_CONFIG_AX_ST_EN           = 0x01,
        ACCEL_CONFIG_AY_ST_EN           = 0x01,
        ACCEL_CONFIG_AZ_ST_EN           = 0x01,
        ACCEL_CONFIG_ACCEL_FS_SEL       = 0x03,

        ACCEL_CONFIG2_ACCEL_FCHOICE_B   = 0x01,
        ACCEL_CONFIG2_A_DLPFCFG         = 0x03,

        LP_ACCEL_ODR_LPOSC_CLKSEL       = 0x0F,

        WOM_THR                         = 0xFF,

        FIFO_EN_TEMP_OUT                = 0x01,
        FIFO_EN_GYRO_XOUT               = 0x01,
        FIFO_EN_GYRO_YOUT               = 0x01,
        FIFO_EN_GYRO_ZOUT               = 0x01,
        FIFO_EN_ACCEL                   = 0x01,
        FIFO_EN_SLV_2                   = 0x01,
        FIFO_EN_SLV_1                   = 0x01,
        FIFO_EN_SLV_0                   = 0x01,

        I2C_MST_CTRL_MULT_MST_EN        = 0x01,
        I2C_MST_CTRL_WAIT_FOR_ES        = 0x01,
        I2C_MST_CTRL_SLV_3_FIFO_EN      = 0x01,
        I2C_MST_CTRL_I2C_MST_P_NSR      = 0x01,
        I2C_MST_CTRL_I2C_MST_CLK        = 0x0F,

        I2C_SLV0_ADDR_I2C_SLV0_RNW      = 0x01,
        I2C_SLV0_ADDR_I2C_ID_0          = 0x7F,

        I2C_SLV0_REG                    = 0xFF,

        I2C_SLV0_CTRL_I2C_SLV0_EN       = 0x01,
        I2C_SLV0_CTRL_I2C_SLV0_BYTE_SW  = 0x01,
        I2C_SLV0_CTRL_I2C_SLV0_REG_DIS  = 0x01,
        I2C_SLV0_CTRL_I2C_SLV0_GRP      = 0x01,
        I2C_SLV0_CTRL_I2C_SLV0_LENG     = 0x0F,

        I2C_SLV1_ADDR_I2C_SLV1_RNW      = 0x01,
        I2C_SLV1_ADDR_I2C_ID_1          = 0x7F,

        I2C_SLV1_REG                    = 0xFF,

        I2C_SLV1_CTRL_I2C_SLV1_EN       = 0x01,
        I2C_SLV1_CTRL_I2C_SLV1_BYTE_SW  = 0x01,
        I2C_SLV1_CTRL_I2C_SLV1_REG_DIS  = 0x01,
        I2C_SLV1_CTRL_I2C_SLV1_GRP      = 0x01,
        I2C_SLV1_CTRL_I2C_SLV1_LENG     = 0x0F,

        I2C_SLV2_ADDR_I2C_SLV2_RNW      = 0x01,
        I2C_SLV2_ADDR_I2C_ID_2          = 0x7F,

        I2C_SLV2_REG                    = 0xFF,

        I2C_SLV2_CTRL_I2C_SLV2_EN       = 0x01,
        I2C_SLV2_CTRL_I2C_SLV2_BYTE_SW  = 0x01,
        I2C_SLV2_CTRL_I2C_SLV2_REG_DIS  = 0x01,
        I2C_SLV2_CTRL_I2C_SLV2_GRP      = 0x01,
        I2C_SLV2_CTRL_I2C_SLV2_LENG     = 0x0F,

        I2C_SLV3_ADDR_I2C_SLV3_RNW      = 0x01,
        I2C_SLV3_ADDR_I2C_ID_3          = 0x7F,

        I2C_SLV3_REG                    = 0xFF,

        I2C_SLV3_CTRL_I2C_SLV3_EN       = 0x01,
        I2C_SLV3_CTRL_I2C_SLV3_BYTE_SW  = 0x01,
        I2C_SLV3_CTRL_I2C_SLV3_REG_DIS  = 0x01,
        I2C_SLV3_CTRL_I2C_SLV3_GRP      = 0x01,
        I2C_SLV3_CTRL_I2C_SLV3_LENG     = 0x0F,

        I2C_SLV4_ADDR_I2C_SLV4_RNW      = 0x01,
        I2C_SLV4_ADDR_I2C_ID_4          = 0x7F,

        I2C_SLV4_REG                    = 0xFF,

        I2C_SLV4_DO                     = 0xFF,

        I2C_SLV4_CTRL_I2C_SLV4_EN       = 0x01,
        I2C_SLV4_CTRL_SLV4_DONE_INT_EN  = 0x01,
        I2C_SLV4_CTRL_I2C_SLV4_REG_DIS  = 0x01,
        I2C_SLV4_CTRL_I2C_MST_DLY       = 0x1F,

        I2C_SLV4_DI                     = 0xFF,

        I2C_MST_STATUS_PASS_THROUGH     = 0x01,
        I2C_MST_STATUS_I2C_SLV4_DONE    = 0x01,
        I2C_MST_STATUS_I2C_LOST_ARB     = 0x01,
        I2C_MST_STATUS_I2C_SLV4_NACK    = 0x01,
        I2C_MST_STATUS_I2C_SLV3_NACK    = 0x01,
        I2C_MST_STATUS_I2C_SLV2_NACK    = 0x01,
        I2C_MST_STATUS_I2C_SLV1_NACK    = 0x01,
        I2C_MST_STATUS_I2C_SLV0_NACK    = 0x01,

        INT_PIN_CFG_ACTL                = 0x01,
        INT_PIN_CFG_OPEN                = 0x01,
        INT_PIN_CFG_LATCH_INT_EN        = 0x01,
        INT_PIN_CFG_INT_ANYRD_2CLEAR    = 0x01,
        INT_PIN_CFG_ACTL_FSYNC          = 0x01,
        INT_PIN_CFG_FSYNC_INT_MODE_EN   = 0x01,
        INT_PIN_CFG_BYPASS_EN           = 0x01,

        INT_ENABLE_WOM_EN               = 0x01,
        INT_ENABLE_FIFO_OVERFLOW_EN     = 0x01,
        INT_ENABLE_FSYNC_INT_EN         = 0x01,
        INT_ENABLE_RAW_RDY_EN           = 0x01,

        INT_STATUS_WOM_INT              = 0x01,
        INT_STATUS_FIFO_OVERFLOW_INT    = 0x01,
        INT_STATUS_FSYNC_INT            = 0x01,
        INT_STATUS_RAW_DATA_RDY_INT     = 0x01,

        ACCEL_XOUT_H                    = 0xFF,
        ACCEL_XOUT_L                    = 0xFF,
        ACCEL_YOUT_H                    = 0xFF,
        ACCEL_YOUT_L                    = 0xFF,
        ACCEL_ZOUT_H                    = 0xFF,
        ACCEL_ZOUT_L                    = 0xFF,
        TEMP_OUT_H                      = 0xFF,
        TEMP_OUT_L                      = 0xFF,
        GYRO_XOUT_H                     = 0xFF,
        GYRO_XOUT_L                     = 0xFF,
        GYRO_YOUT_H                     = 0xFF,
        GYRO_YOUT_L                     = 0xFF,
        GYRO_ZOUT_H                     = 0xFF,
        GYRO_ZOUT_L                     = 0xFF,

        EXT_SENS_DATA_00                = 0xFF,
        EXT_SENS_DATA_01                = 0xFF,
        EXT_SENS_DATA_02                = 0xFF,
        EXT_SENS_DATA_03                = 0xFF,
        EXT_SENS_DATA_04                = 0xFF,
        EXT_SENS_DATA_05                = 0xFF,
        EXT_SENS_DATA_06                = 0xFF,
        EXT_SENS_DATA_07                = 0xFF,
        EXT_SENS_DATA_08                = 0xFF,
        EXT_SENS_DATA_09                = 0xFF,
        EXT_SENS_DATA_10                = 0xFF,
        EXT_SENS_DATA_11                = 0xFF,
        EXT_SENS_DATA_12                = 0xFF,
        EXT_SENS_DATA_13                = 0xFF,
        EXT_SENS_DATA_14                = 0xFF,
        EXT_SENS_DATA_15                = 0xFF,
        EXT_SENS_DATA_16                = 0xFF,
        EXT_SENS_DATA_17                = 0xFF,
        EXT_SENS_DATA_18                = 0xFF,
        EXT_SENS_DATA_19                = 0xFF,
        EXT_SENS_DATA_20                = 0xFF,
        EXT_SENS_DATA_21                = 0xFF,
        EXT_SENS_DATA_22                = 0xFF,
        EXT_SENS_DATA_23                = 0xFF,

        I2C_SLV0_DO                     = 0xFF,
        I2C_SLV1_DO                     = 0xFF,
        I2C_SLV2_DO                     = 0xFF,
        I2C_SLV3_DO                     = 0xFF,

        I2C_MST_DELAY_CTRL_DELAY_ES_SHADOW  = 0x01,
        I2C_MST_DELAY_CTRL_I2C_SLV4_DLY_EN  = 0x01,
        I2C_MST_DELAY_CTRL_I2C_SLV3_DLY_EN  = 0x01,
        I2C_MST_DELAY_CTRL_I2C_SLV2_DLY_EN  = 0x01,
        I2C_MST_DELAY_CTRL_I2C_SLV1_DLY_EN  = 0x01,
        I2C_MST_DELAY_CTRL_I2C_SLV0_DLY_EN  = 0x01,

        SIGNAL_PATH_RESET_GYRO_RST      = 0x01,
        SIGNAL_PATH_RESET_ACCEL_RST     = 0x01,
        SIGNAL_PATH_RESET_TEMP_RST      = 0x01,

        ACCEL_INTEL_CTRL_MOT_ACCEL_INTEL_EN     = 0x01,
        ACCEL_INTEL_CTRL_MOT_ACCEL_INTEL_MODE   = 0x01,

        USER_CTRL_FIFO_EN               = 0x01,
        USER_CTRL_I2C_MST_EN            = 0x01,
        USER_CTRL_I2C_IF_DIS            = 0x01,
        USER_CTRL_FIFO_RST              = 0x01,
        USER_CTRL_I2C_MST_RST           = 0x01,
        USER_CTRL_SIG_COND_RST          = 0x01,

        PWR_MGMT_1_H_RESET              = 0x01,
        PWR_MGMT_1_SLEEP                = 0x01,
        PWR_MGMT_1_CYCLE                = 0x01,
        PWR_MGMT_1_GYRO_STANDBY         = 0x01,
        PWR_MGMT_1_PD_PTAT              = 0x01,
        PWR_MGMT_1_CLKSEL               = 0x07,

        PWR_MGMT_2_DISABLE_XA           = 0x01,
        PWR_MGMT_2_DISABLE_YA           = 0x01,
        PWR_MGMT_2_DISABLE_ZA           = 0x01,
        PWR_MGMT_2_DISABLE_XG           = 0x01,
        PWR_MGMT_2_DISABLE_YG           = 0x01,
        PWR_MGMT_2_DISABLE_ZG           = 0x01,

        FIFO_COUNTH_FIFO_CNT            = 0x1F,
        FIFO_COUNTL_FIFO_CNT            = 0xFF,

        FIFO_R_W                        = 0xFF,

        WHO_AM_I                        = 0xFF,

        XA_OFFSET_H                     = 0xFF,
        XA_OFFSET_L                     = 0x7F,

        YA_OFFSET_H                     = 0xFF,
        YA_OFFSET_L                     = 0x7F,

        ZA_OFFSET_H                     = 0xFF,
        ZA_OFFSET_L                     = 0x7F,
    };
    //     /////////////////     REGISTER ADDRESSES     /////////////////     //
    enum class Register : uint8_t {
        SELF_TEST_X_GYRO                = 0x00,
        SELF_TEST_Y_GYRO                = 0x01,
        SELF_TEST_Z_GYRO                = 0x02,

        SELF_TEST_X_ACCEL               = 0x0D,
        SELF_TEST_Y_ACCEL               = 0x0E,
        SELF_TEST_Z_ACCEL               = 0x0F,

        XG_OFFSET_H                     = 0x13,
        XG_OFFSET_L                     = 0x14,
        YG_OFFSET_H                     = 0x15,
        YG_OFFSET_L                     = 0x16,
        ZG_OFFSET_H                     = 0x17,
        ZG_OFFSET_L                     = 0x18,
        SMPLRT_DIV                      = 0x19,
        CONFIG                          = 0x1A,
        GYRO_CONFIG                     = 0x1B,
        ACCEL_CONFIG                    = 0x1C,
        ACCEL_CONFIG2                   = 0x1D,
        LP_ACCEL_ODR                    = 0x1E,
        WOM_THR                         = 0x1F,

        FIFO_EN                         = 0x23,
        I2C_MST_CTRL                    = 0x24,
        I2C_SLV0_ADDR                   = 0x25,
        I2C_SLV0_REG                    = 0x26,
        I2C_SLV0_CTRL                   = 0x27,
        I2C_SLV1_ADDR                   = 0x28,
        I2C_SLV1_REG                    = 0x29,
        I2C_SLV1_CTRL                   = 0x2A,
        I2C_SLV2_ADDR                   = 0x2B,
        I2C_SLV2_REG                    = 0x2C,
        I2C_SLV2_CTRL                   = 0x2D,
        I2C_SLV3_ADDR                   = 0x2E,
        I2C_SLV3_REG                    = 0x2F,
        I2C_SLV3_CTRL                   = 0x30,
        I2C_SLV4_ADDR                   = 0x31,
        I2C_SLV4_REG                    = 0x32,
        I2C_SLV4_DO                     = 0x33,
        I2C_SLV4_CTRL                   = 0x34,

        I2C_SLV4_DI                     = 0x35,
        I2C_MST_STATUS                  = 0x36,
        INT_PIN_CFG                     = 0x37,
        INT_ENABLE                      = 0x38,

        INT_STATUS                      = 0x3A,
        ACCEL_XOUT_H                    = 0x3B,
        ACCEL_XOUT_L                    = 0x3C,
        ACCEL_YOUT_H                    = 0x3D,
        ACCEL_YOUT_L                    = 0x3E,
        ACCEL_ZOUT_H                    = 0x3F,
        ACCEL_ZOUT_L                    = 0x40,
        TEMP_OUT_H                      = 0x41,
        TEMP_OUT_L                      = 0x42,
        GYRO_XOUT_H                     = 0x43,
        GYRO_XOUT_L                     = 0x44,
        GYRO_YOUT_H                     = 0x45,
        GYRO_YOUT_L                     = 0x46,
        GYRO_ZOUT_H                     = 0x47,
        GYRO_ZOUT_L                     = 0x48,
        EXT_SENS_DATA_00                = 0x49,
        EXT_SENS_DATA_01                = 0x4A,
        EXT_SENS_DATA_02                = 0x4B,
        EXT_SENS_DATA_03                = 0x4C,
        EXT_SENS_DATA_04                = 0x4D,
        EXT_SENS_DATA_05                = 0x4E,
        EXT_SENS_DATA_06                = 0x4F,
        EXT_SENS_DATA_07                = 0x50,
        EXT_SENS_DATA_08                = 0x51,
        EXT_SENS_DATA_09                = 0x52,
        EXT_SENS_DATA_10                = 0x53,
        EXT_SENS_DATA_11                = 0x54,
        EXT_SENS_DATA_12                = 0x55,
        EXT_SENS_DATA_13                = 0x56,
        EXT_SENS_DATA_14                = 0x57,
        EXT_SENS_DATA_15                = 0x58,
        EXT_SENS_DATA_16                = 0x59,
        EXT_SENS_DATA_17                = 0x5A,
        EXT_SENS_DATA_18                = 0x5B,
        EXT_SENS_DATA_19                = 0x5C,
        EXT_SENS_DATA_20                = 0x5D,
        EXT_SENS_DATA_21                = 0x5E,
        EXT_SENS_DATA_22                = 0x5F,
        EXT_SENS_DATA_23                = 0x60,

        I2C_SLV0_DO                     = 0x63,
        I2C_SLV1_DO                     = 0x64,
        I2C_SLV2_DO                     = 0x65,
        I2C_SLV3_DO                     = 0x66,
        I2C_MST_DELAY_CTRL              = 0x67,
        SIGNAL_PATH_RESET               = 0x68,
        ACCEL_INTEL_CTRL                = 0x69,
        USER_CTRL                       = 0x6A,
        PWR_MGMT_1                      = 0x6B,
        PWR_MGMT_2                      = 0x6C,

        FIFO_COUNTH                     = 0x72,
        FIFO_COUNTL                     = 0x73,
        FIFO_R_W                        = 0x74,
        WHO_AM_I                        = 0x75,

        XA_OFFSET_H                     = 0x77,
        XA_OFFSET_L                     = 0x78,

        YA_OFFSET_H                     = 0x7A,
        YA_OFFSET_L                     = 0x7B,

        ZA_OFFSET_H                     = 0x7D,
        ZA_OFFSET_L                     = 0x7E,
    };

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Operators overloading ---------------------------------------------------

public:
    // NONE

private:
    //     ////////////////////    BIT POSITIONS     ////////////////////     //
    inlined friend uint8_t operator<<(uint8_t a, Mpu::Bit b);
    inlined friend uint8_t operator>>(uint8_t a, Mpu::Bit b);

    //     //////////////////////     BIT MASK     //////////////////////     //
    inlined friend uint8_t operator&(uint8_t a, Mpu::BitMask b);
    inlined friend uint8_t &operator&=(uint8_t &a, Mpu::BitMask b);

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Constructors ------------------------------------------------------------

public:
    Mpu(
            PartId partId_p = PartId::MPU9250
    );
    // TODO: Implement destructor function
    ~Mpu(
            void
    );

    // -------------------------------------------------------------------------
    // Methods - Class own methods ---------------------------------------------

public:
    //     //////////////     ACCELEROMETER MANAGEMENT     //////////////     //
    bool configAccel(
            Axis axis_p = Axis::ALL,
            AccelFullScale fullScale_p = AccelFullScale::FS_2G,
            AccelDlpf dlpf_p = AccelDlpf::DLPF_OFF
    );
    bool setAccelAxis(
            Axis axis_p = Axis::ALL
    );
    bool setAccelDlpf(
            AccelDlpf dlpf_p
    );
    bool setAccelFullScale(
            AccelFullScale fullScale_p
    );
    bool setAccelLowPowerMode(
            uint8_t sampleRate_p
    );

    bool getAccelData(
            int16_t *buff_p,
            Axis axis_p = Axis::ALL
    );

    bool getAccelOffset(
            int16_t *buff_p,
            Axis axis_p = Axis::ALL
    );
    bool setAccelOffset(
            int16_t *buff_p,
            Axis axis_p = Axis::ALL
    );

    //     /////////////////     COMPASS MANAGEMENT     /////////////////     //
    bool setCompassSampleRate(
            uint8_t sampleRate_p
    );
    bool setupCompass(
            void
    );

    //     ///////////////////    FIFO MANAGEMENT     ///////////////////     //
    bool configFifo(
            uint8_t sensors_p
    );

    //     ////////////////     GYROSCOPE MANAGEMENT     ////////////////     //
    bool configGyro(
            Axis axis_p = Axis::ALL,
            GyroFullScale fullScale_p = GyroFullScale::FS_2000_DPS,
            GyroDlpf dlpf_p = GyroDlpf::DLPF_OFF
    );
    bool setGyroAxis(
            Axis axis_p = Axis::ALL
    );
    bool setGyroDlpf(
            GyroDlpf dlpf_p
    );
    bool setGyroFullScale(
            GyroFullScale fullScale_p
    );

    bool getGyroData(
            int16_t *buff_p,
            Axis axis_p = Axis::ALL
    );

    bool getGyroOffset(
            int16_t *buff_p,
            Axis axis_p = Axis::ALL
    );
    bool setGyroOffset(
            int16_t *buff_p,
            Axis axis_p = Axis::ALL
    );

    //     ////////////////     INTERRUPT MANAGEMENT     ////////////////     //
    bool configIntPin(
            LogicLevel pinLevel_p = LogicLevel::LOW,
            DriverMode pinDriver_p = DriverMode::PUSH_PULL,
            bool pinLatch_p = false,
            bool pinAutoReset_p = false
    );
    bool disableInterrupts(
            InterruptFlags deactivationMask_p
    );
    bool enableInterrupts(
            InterruptFlags activationMask_p
    );
    bool generalIrqHandler(
            void
    );

    bool setIntFifoOverflowCallback(
            void (*callbackFunction_p)(void)
    );
    bool setIntFsyncPinCallback(
            void (*callbackFunction_p)(void)
    );
    bool setIntRawDataReadyCallback(
            void (*callbackFunction_p)(void)
    );
    bool setIntWakeUpOnMotionCallback(
            void (*callbackFunction_p)(void)
    );

    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    Error getLastError(
            void
    );
    bool init(
            Bus *twiBusHandler_p,
            bool useTwiAlternateAddress_p = false
    );
    bool init(
            Bus *spiBusHandler_p,
            Gpio *csPin_p
    );
    bool reset(
            void
    );
    bool setSampleRate(
            uint16_t sampleRate_p
    );

    //     ///////////////     TEMPERATURE MANAGEMENT     ///////////////     //
    bool getTemperature(
            float *tempValue_p,
            ThermometricScale tempScale_p = ThermometricScale::CELSIUS
    );
    // bool getTemperature(
    //         Int32Q16_t *tempQ16_p,
    //         ThermometricScale tempScale_p = ThermometricScale::CELSIUS
    // );

private:
    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    void _clearData(
            void
    );
    bool _init(
            void
    );
    bool _isInitialized(
            void
    );
    bool _reset(
            void
    );

    //     /////////////////////     INTERRUPTS     /////////////////////     //
    bool _updateInterruptStatus(
            void
    );

    //     ////////////////    REGISTER MANIPULATION     ////////////////     //
    bool _read(
            Register regAddr_p,
            uint8_t *dataBuffer_p,
            uint16_t bytesToRead_p
    );
    bool _write(
            Register regAddr_p,
            uint8_t *dataBuffer_p,
            uint16_t bytesToWrite_p
    );

protected:
    // NONE

    // -------------------------------------------------------------------------
    // Properties --------------------------------------------------------------

public:
    // NONE

private:
    //     ////////////////    PERIPHERAL BUS HANDLER     ////////////////     //
    Bus         *_busHandler;
    bool        _useSpiBus              : 1;
    uint8_t     _twiAddress             : 7;
    Gpio        *_csPin;

    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    bool        _initialized            : 1;
    Error       _lastError;
    PartId      _partId;


    bool        _fifoRollOverDisabled   : 1;
    ExternalSync _externalSync;
    uint16_t    _sampleRate             : 10;

    //     /////////////     ACCELEROMETER RELATED DATA     /////////////     //
    bool        _st_chip_cfg_accel_half         : 1;
    bool        _st_chip_cfg_lp_accel_mode      : 1;


    AccelDlpf       _accelDlpf;
    Axis            _accelAxisEnabled;
    bool            _accelFchoiceB      : 1;
    AccelFullScale  _accelFullScale;

    //     ///////////////     GYROSCOPE RELATED DATA     ///////////////     //
    GyroDlpf        _gyroDlpf;
    Axis            _gyroAxisEnabled;
    uint8_t         _gyroFchoiceB       : 2;
    GyroFullScale   _gyroFullScale;

    //     ////////////////     COMPASS RELATED DATA     ////////////////     //
    uint8_t     _compassSampleRate      : 7;

    //     ///////////////     INTERRUPT RELATED DATA     ///////////////     //
    LogicLevel  _intPinActiveLevel;
    DriverMode  _intPinDriverMode;
    bool        _intPinLatchEnabled;
    bool        _intPinAutoResetEnabled;
    LogicLevel  _fsyncIntPinActiveLevel;
    bool        _i2cRoutedToAuxBus;
    bool        _fsyncPinTriggersInterrupt;
    bool        _intFifoOverflowEnabled;
    bool        _intFsyncPinEnabled;
    bool        _intRawDataReadyEnabled;
    bool        _intWakeUpOnMotionEnabled;
    bool        _intFifoOverflowStatus;
    bool        _intFsyncPinStatus;
    bool        _intRawDataReadyStatus;
    bool        _intWakeUpOnMotionStatus;
    void (*_intFifoOverflowCallback)(void);
    void (*_intFsyncPinCallback)(void);
    void (*_intRawDataReadyCallback)(void);
    void (*_intWakeUpOnMotionCallback)(void);

protected:
    // NONE

}; // class Mpu

// =============================================================================
// Mpu - Class overloading operators
// =============================================================================

inlined uint8_t operator<<(uint8_t a, Mpu::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator>>(uint8_t a, Mpu::Bit b)
{
    return static_cast<uint8_t>(static_cast<uint8_t>(a) >> static_cast<uint8_t>(b));
}

inlined uint8_t operator&(uint8_t a, Mpu::BitMask b)
{
    return static_cast<uint8_t>(a & static_cast<uint8_t>(b));
}

inlined uint8_t &operator&=(uint8_t &a, Mpu::BitMask b)
{
    return a = static_cast<uint8_t>(a & static_cast<uint8_t>(b));
}

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
// Mpu - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// General public functions declarations
// =============================================================================

// Initialization
// FIXME: Implement this functions
int32_t new_mpu_init_slave(void);
int32_t new_mpu_set_bypass(uint8_t bypass_on);

// Configuration APIs
int32_t new_mpu_lp_motion_interrupt(uint16_t thresh, uint8_t time, uint16_t lpa_freq);
int32_t new_mpu_set_int_level(uint8_t active_low);
int32_t new_mpu_set_int_latched(uint8_t enable);
int32_t new_mpu_set_dmp_state(uint8_t enable);
int32_t new_mpu_get_dmp_state(uint8_t *enabled);
int32_t new_mpu_get_compass_fsr(uint16_t *fsr);
int32_t new_mpu_get_gyro_sens(float *sens);
int32_t new_mpu_get_compass_sample_rate(uint16_t *rate);
int32_t new_mpu_get_fifo_config(uint8_t *sensors);
int32_t new_mpu_get_power_state(uint8_t *power_on);
int32_t new_mpu_read_6500_accel_bias(int32_t *accel_bias);
int32_t new_mpu_set_gyro_bias_reg(int32_t *gyro_bias);
int32_t new_mpu_set_accel_bias_6500_reg(cint32_t *accel_bias);
int32_t new_mpu_read_6050_accel_bias(int32_t *accel_bias);
int32_t new_mpu_set_accel_bias_6050_reg(cint32_t *accel_bias);

// Data getter/setter APIs
int32_t new_mpu_get_compass_reg(int16_t *data, uint32_t *timestamp);
int32_t new_mpu_read_fifo(int16_t *gyro, int16_t *accel, uint32_t *timestamp, uint8_t *sensors, uint8_t *more);
int32_t new_mpu_read_fifo_stream(uint16_t length, uint8_t *data, uint8_t *more);
int32_t new_mpu_reset_fifo(void);
int32_t new_mpu_write_mem(uint16_t mem_addr, uint16_t length, uint8_t *data);
int32_t new_mpu_read_mem(uint16_t mem_addr, uint16_t length, uint8_t *data);
int32_t new_mpu_load_firmware(uint16_t length, const uint8_t *firmware, uint16_t start_addr, uint16_t sample_rate);
int32_t new_mpu_reg_dump(void);
int32_t new_mpu_run_self_test(int32_t *gyro, int32_t *accel);
int32_t new_mpu_run_6500_self_test(int32_t *gyro, int32_t *accel, uint8_t debug);
int32_t new_mpu_register_tap_cb(void (*func)(uint8_t, uint8_t));

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

// =============================================================================
// External default objects
// =============================================================================

// NONE

#endif // __FUNSAPE_LIB_MPU_HPP

// =============================================================================
// END OF FILE
// =============================================================================
