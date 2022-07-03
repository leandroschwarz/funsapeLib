/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibMpu9250.hpp
 * Module:          MPU9250 Interface Module for FunSAPE++ Embedded Library
 *                  project
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_MPU9250_HPP
#define __FUNSAPE_LIB_MPU9250_HPP               __BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Project configuration file --------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#   error   [funsapeLibMpu9250.hpp] Error 10 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#   error   [funsapeLibMpu9250.hpp] Error 11 - Build mismatch between (funsapeLibMpu9250.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - Basic header for all files -----------------------------------

#include "../funsapeLibI2c.hpp"
#include "../funsapeLibSpi.hpp"

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
// =============================================================================

// NONE

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// Single variables => shortening the name of single data types ----------------

// NONE

// -----------------------------------------------------------------------------
// Single variables => enumerations understood as logic values -----------------

// NONE

// -----------------------------------------------------------------------------
// Single variables => small enumerations that can be understood as a single variable

// NONE

// -----------------------------------------------------------------------------
// Enumerations => large enumerations or enumerations with expansible number of elements

typedef enum mpu9250AccelFullScale_e {
    MPU9250_AFS_2G                          = 0,
    MPU9250_AFS_4G                          = 1,
    MPU9250_AFS_8G                          = 2,
    MPU9250_AFS_16G                         = 3,
} mpu9250AccelFullScale_e;

typedef enum mpu9250AccelSampleRate_e {
    MPU9250_ACCEL_SAMPLE_RATE_4_KHZ         = 256,
    MPU9250_ACCEL_SAMPLE_RATE_1_KHZ         = 0,
    MPU9250_ACCEL_SAMPLE_RATE_500_HZ        = 1,
    MPU9250_ACCEL_SAMPLE_RATE_250_HZ        = 3,
    MPU9250_ACCEL_SAMPLE_RATE_200_HZ        = 4,
    MPU9250_ACCEL_SAMPLE_RATE_125_HZ        = 7,
    MPU9250_ACCEL_SAMPLE_RATE_100_HZ        = 9,
    MPU9250_ACCEL_SAMPLE_RATE_50_HZ         = 19,
    MPU9250_ACCEL_SAMPLE_RATE_40_HZ         = 24,
    MPU9250_ACCEL_SAMPLE_RATE_25_HZ         = 39,
    MPU9250_ACCEL_SAMPLE_RATE_20_HZ         = 49,
    MPU9250_ACCEL_SAMPLE_RATE_12_5_HZ       = 79,
    MPU9250_ACCEL_SAMPLE_RATE_10_HZ         = 99,
    MPU9250_ACCEL_SAMPLE_RATE_8_HZ          = 124,
    MPU9250_ACCEL_SAMPLE_RATE_5_HZ          = 199,
    MPU9250_ACCEL_SAMPLE_RATE_4_HZ          = 249,
    MPU9250_ACCEL_SAMPLE_RATE_3_9_HZ        = 255,
} mpu9250AccelSampleRate_e;

typedef enum mpu9250AccelDigitalFilter_e {
    MPU9250_ACCEL_DLPF_218_HZ               = 1,
    MPU9250_ACCEL_DLPF_99_HZ                = 2,
    MPU9250_ACCEL_DLPF_45_HZ                = 3,
    MPU9250_ACCEL_DLPF_21_HZ                = 4,
    MPU9250_ACCEL_DLPF_10_HZ                = 5,
    MPU9250_ACCEL_DLPF_5_HZ                 = 6,
    MPU9250_ACCEL_DLPF_420_HZ               = 7,
    MPU9250_ACCEL_DLPF_1046_HZ              = 8,
} mpu9250AccelDigitalFilter_e;

typedef enum mpu9250GyroFullScale_e {
    MPU9250_GFS_250DPS                      = 0,
    MPU9250_GFS_500DPS                      = 1,
    MPU9250_GFS_1000DPS                     = 2,
    MPU9250_GFS_2000DPS                     = 3,
} mpu9250GyroFullScale_e;

typedef enum mpu9250GyroDigitalFilter_e {
    MPU9250_GYRO_DLPF_250_HZ                = 0,
    MPU9250_GYRO_DLPF_184_HZ                = 1,
    MPU9250_GYRO_DLPF_92_HZ                 = 2,
    MPU9250_GYRO_DLPF_41_HZ                 = 3,
    MPU9250_GYRO_DLPF_20_HZ                 = 4,
    MPU9250_GYRO_DLPF_10_HZ                 = 5,
    MPU9250_GYRO_DLPF_5_HZ                  = 6,
    MPU9250_GYRO_DLPF_3600_HZ               = 7,
    MPU9250_GYRO_DLPF_8800_HZ               = 8,
} mpu9250GyroDigitalFilter_e;

typedef enum mpu9250GyroSampleRate_e {
    MPU9250_GYRO_SAMPLE_RATE_32_KHZ         = 512,
    MPU9250_GYRO_SAMPLE_RATE_8_KHZ          = 256,
    MPU9250_GYRO_SAMPLE_RATE_4_KHZ          = 257,
    MPU9250_GYRO_SAMPLE_RATE_2_KHZ          = 259,
    MPU9250_GYRO_SAMPLE_RATE_1_6_KHZ        = 260,
    MPU9250_GYRO_SAMPLE_RATE_1_KHZ          = 0,
    MPU9250_GYRO_SAMPLE_RATE_800_HZ         = 265,
    MPU9250_GYRO_SAMPLE_RATE_500_HZ         = 1,
    MPU9250_GYRO_SAMPLE_RATE_400_HZ         = 275,
    MPU9250_GYRO_SAMPLE_RATE_320_HZ         = 281,
    MPU9250_GYRO_SAMPLE_RATE_250_HZ         = 3,
    MPU9250_GYRO_SAMPLE_RATE_200_HZ         = 4,
    MPU9250_GYRO_SAMPLE_RATE_160_HZ         = 305,
    MPU9250_GYRO_SAMPLE_RATE_125_HZ         = 7,
    MPU9250_GYRO_SAMPLE_RATE_100_HZ         = 9,
    MPU9250_GYRO_SAMPLE_RATE_80_HZ          = 355,
    MPU9250_GYRO_SAMPLE_RATE_64_HZ          = 380,
    MPU9250_GYRO_SAMPLE_RATE_50_HZ          = 19,
    MPU9250_GYRO_SAMPLE_RATE_40_HZ          = 24,
    MPU9250_GYRO_SAMPLE_RATE_32_HZ          = 505,
    MPU9250_GYRO_SAMPLE_RATE_31_25_HZ       = 511,
    MPU9250_GYRO_SAMPLE_RATE_25_HZ          = 39,
    MPU9250_GYRO_SAMPLE_RATE_20_HZ          = 49,
    MPU9250_GYRO_SAMPLE_RATE_12_5_HZ        = 79,
    MPU9250_GYRO_SAMPLE_RATE_10_HZ          = 99,
    MPU9250_GYRO_SAMPLE_RATE_8_HZ           = 124,
    MPU9250_GYRO_SAMPLE_RATE_5_HZ           = 199,
    MPU9250_GYRO_SAMPLE_RATE_4_HZ           = 249,
    MPU9250_GYRO_SAMPLE_RATE_3_9_HZ         = 255,
} mpu9250GyroSampleRate_e;

// -----------------------------------------------------------------------------
// New Data Type - Enumerations => parametric table that doesn't maintain value equivalence with datasheet's values

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Flags => enumerations that can be combined in the form of flags

typedef enum mpu9250ResetPath_f {
    MPU9250_RESET_GYRO      = 1,
    MPU9250_RESET_ACCEL     = 2,
    MPU9250_RESET_TEMP      = 4,
    MPU9250_RESET_FIFO      = 8,
    MPU9250_RESET_I2C_MST   = 16,
    MPU9250_RESET_SIG_COND  = 32,
    MPU9250_RESET_ALL       = 63
} mpu9250ResetPath_f;

// -----------------------------------------------------------------------------
// New Data Type - Structures, Unions and Bit Fields ---------------------------

// NONE

// -----------------------------------------------------------------------------
// New Data Type - Classes -----------------------------------------------------

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// Mpu9250 - Class declaration
// =============================================================================

class Mpu9250
{
    // -------------------------------------------------------------------------
    // New data types
    // -------------------------------------------------------------------------

public:
    // typedef enum mpu9250I2cMasterClock_e {
    //  MPU9250_I2C_MST_CLK_258_KHz = 8,
    //  MPU9250_I2C_MST_CLK_267_KHz = 7,
    //  MPU9250_I2C_MST_CLK_276_KHz = 6,
    //  MPU9250_I2C_MST_CLK_286_KHz = 5,
    //  MPU9250_I2C_MST_CLK_296_KHz = 4,
    //  MPU9250_I2C_MST_CLK_308_KHz = 3,
    //  MPU9250_I2C_MST_CLK_320_KHz = 2,
    //  MPU9250_I2C_MST_CLK_333_KHz = 1,
    //  MPU9250_I2C_MST_CLK_348_KHz = 0,
    //  MPU9250_I2C_MST_CLK_364_KHz = 15,
    //  MPU9250_I2C_MST_CLK_381_KHz = 14,
    //  MPU9250_I2C_MST_CLK_400_KHz = 13,
    //  MPU9250_I2C_MST_CLK_421_KHz = 12,
    //  MPU9250_I2C_MST_CLK_444_KHz = 11,
    //  MPU9250_I2C_MST_CLK_471_KHz = 10,
    //  MPU9250_I2C_MST_CLK_500_KHz = 9
    // } mpu9250I2cMasterClock_e;

    // typedef enum mpu9250ExternalSync_e {
    //  MPU9250_EXT_SYNC_DISABLED   = 0,
    //  MPU9250_EXT_SYNC_TEMP,
    //  MPU9250_EXT_SYNC_GYRO_X,
    //  MPU9250_EXT_SYNC_GYRO_Y,
    //  MPU9250_EXT_SYNC_GYRO_Z,
    //  MPU9250_EXT_SYNC_ACCEL_X,
    //  MPU9250_EXT_SYNC_ACCEL_Y,
    //  MPU9250_EXT_SYNC_ACCEL_Z
    // } mpu9250ExternalSync_e;

    // typedef enum mpu9250AccelSamplingLowPower_e {
    //  MPU9250_ACCEL_SAMP_LOW_POWER_0P24_HZ        = 0,
    //  MPU9250_ACCEL_SAMP_LOW_POWER_0P49_HZ        = 1,
    //  MPU9250_ACCEL_SAMP_LOW_POWER_0P98_HZ        = 2,
    //  MPU9250_ACCEL_SAMP_LOW_POWER_1P95_HZ        = 3,
    //  MPU9250_ACCEL_SAMP_LOW_POWER_3P91_HZ        = 4,
    //  MPU9250_ACCEL_SAMP_LOW_POWER_7P81_HZ        = 5,
    //  MPU9250_ACCEL_SAMP_LOW_POWER_15P63_HZ       = 6,
    //  MPU9250_ACCEL_SAMP_LOW_POWER_31P25_HZ       = 7,
    //  MPU9250_ACCEL_SAMP_LOW_POWER_62P50_HZ       = 8,
    //  MPU9250_ACCEL_SAMP_LOW_POWER_125_HZ         = 9,
    //  MPU9250_ACCEL_SAMP_LOW_POWER_250_HZ         = 10,
    //  MPU9250_ACCEL_SAMP_LOW_POWER_500_HZ         = 11,
    // } mpu9250AccelSamplingLowPower_e;

    // typedef enum mpu9250PowerMode_e {
    //  MPU9250_POWER_MODE_UNDEFINED            = 0,
    //  MPU9250_POWER_MODE_SLEEP                = 1,
    //  MPU9250_POWER_MODE_STANDBY              = 2,
    //  MPU9250_POWER_MODE_LOW_POWER_ACCEL      = 3,
    //  MPU9250_POWER_MODE_LOW_NOISE_ACCEL      = 4,
    //  MPU9250_POWER_MODE_GYRO                 = 5,
    //  MPU9250_POWER_MODE_MAG                  = 6,
    //  MPU9250_POWER_MODE_ACCEL_GYRO           = 7,
    //  MPU9250_POWER_MODE_ACCEL_MAG            = 8,
    //  MPU9250_POWER_MODE_ACCEL_GYRO_MAG       = 9,
    // } mpu9250PowerMode_e;

    // typedef enum mpu9250ClockSelection_e {
    //  MPU9250_CLOCK_20_MHZ_OSCILLATOR     = 0,
    //  MPU9250_CLOCK_AUTO_SELECT_BEST      = 1,
    //  MPU9250_CLOCK_DISABLED              = 2,
    // } mpu9250ClockSelection_e;

    // -------------------------------------------------------------------------
    // Operators overloading
    // -------------------------------------------------------------------------

    // NONE

    // -------------------------------------------------------------------------
    // Constructors
    // -------------------------------------------------------------------------

public:
    Mpu9250(void);
    ~Mpu9250(void);                     // TODO: Implement function

    // -------------------------------------------------------------------------
    // Methods
    // -------------------------------------------------------------------------

    // Methods - Public
public:
    //     ///////////////////     INITIALIZATION     ///////////////////     //
    bool initialize(
        I2c     *i2cHndlr,
        bool useAlternateAddress = false
    );
    bool initialize(
        Spi     *spiHndlr,
        void    (*actDevFuncCallback)(void),
        void    (*deactDevFuncCallback)(void)
    );
    bool restart(void);
    bool reset(
        mpu9250ResetPath_f resetPaths
    );

    //     ////////////////////     CHECK STATUS     ////////////////////     //
    error_e getLastError(void);

    //     ////////////////     DEVICE CONFIGURATION     ////////////////     //
    bool configAccel(axis_t axis,
        mpu9250AccelFullScale_e scale,
        mpu9250AccelSampleRate_e sampleRate,
        mpu9250AccelDigitalFilter_e filter);
    bool configGyro(axis_t axis,
        mpu9250GyroFullScale_e scale,
        mpu9250GyroSampleRate_e sampleRate,
        mpu9250GyroDigitalFilter_e filter);
    bool performSelfTest(axis_t accelAxis,
        axis_t gyroAxis);                   // TODO: Implement function

    //     /////////////////    RAW DATA MANAGEMENT     /////////////////     //
    bool getAccelData(int16_t *accelBuff,
        axis_t axis = AXIS_ALL);
    bool getAccelOffset(int16_t *accelBuff,
        axis_t axis = AXIS_ALL);
    bool setAccelOffset(int16_t *accelBuff,
        axis_t axis = AXIS_ALL);
    bool getGyroData(int16_t *gyroBuff,
        axis_t axis = AXIS_ALL);
    bool getGyroOffset(int16_t *gyroBuff,
        axis_t axis = AXIS_ALL);
    bool setGyroOffset(int16_t *gyroBuff,
        axis_t axis = AXIS_ALL);
    bool getTempData(int16_t *temp);

    //     /////////////////     FIFO CONFIGURATION     /////////////////     //
    bool configFifo(void);                              //************** To Do
    bool configIntPin(logicLevel_t pinLevel = LOGIC_LOW,
        outputDriver_t pinDriver = DRIVER_PUSH_PULL,
        bool pinLatch = false,
        bool pinAutoReset = false);
    bool setIntDataReady(bool enabled);

    //     ////////////////     OTHER TEST FUNCTIONS     ////////////////     //
//  void readAll(uint8_t *buffer, uint8_t *size);
//  void readMag(int16_t * magBuff);

    // Methods - Private
private:
    //     //////////////////////    INTERFACE     //////////////////////     //
    bool _deinitialize(void);           // TODO: Implement function
    bool _initialize(void);
    bool _getWhoAmI(void);

    //     ////////////////////    CONFIGURATION     ////////////////////     //
    bool _updateSampleRateDivider(void);                        //************** To Do
    bool _updateAccelConfig(void);                              //************** To Do
    bool _updateAccelConfig2(void);                             //************** To Do
    bool _updatePowerManagementControl2(void);                  //************** To Do
    bool _updateConfig(void);                                   //************** To Do
    bool _updateGyroConfig(void);                               //************** To Do
    bool _getAccelSelfTest(void);                               //************** To Do
    bool _getGyroSelfTest(void);                                //************** To Do
    bool _updateUserControl(void);                              //************** To Do

    //     /////////////////    RAW DATA MANAGEMENT     /////////////////     //
    bool _getAccelData(void);                                   //************** To Do
    bool _getAccelOffset(void);                                 //************** To Do
    bool _getGyroData(void);                                    //************** To Do
    bool _getGyroOffset(void);                                  //************** To Do
    bool _getTempData(void);                                    //************** To Do
    bool _updateAccelOffset(void);                              //************** To Do
    bool _updateGyroOffset(void);                               //************** To Do

    //     ////////////////////////     FIFO     ////////////////////////     //
    bool _updateFifoEnable(void);                               //************** To Do
    bool _getFifoCount(void);                                   //************** To Do
    bool _getFifoReadWrite(void);                               //************** To Do
    bool _updateFifoReadWrite(void);                            //************** To Do

    //     ///////////////////     MOTION CONTROL     ///////////////////     //
    bool _updateWakeUpMotionThreshold(void);                    //************** To Do
    bool _updateMotionDetectControl(void);                      //************** To Do

    //     /////////////////////////    I2C     /////////////////////////     //
    bool _updateI2cMasterControl(void);                         //************** To Do
    bool _updateI2cSlave0Address(void);                         //************** To Do
    bool _updateI2cSlave0Control(void);                         //************** To Do
    bool _updateI2cSlave0Register(void);                        //************** To Do
    bool _updateI2cSlave1Address(void);                         //************** To Do
    bool _updateI2cSlave1Control(void);                         //************** To Do
    bool _updateI2cSlave1Register(void);                        //************** To Do
    bool _updateI2cSlave2Address(void);                         //************** To Do
    bool _updateI2cSlave2Control(void);                         //************** To Do
    bool _updateI2cSlave2Register(void);                        //************** To Do
    bool _updateI2cSlave3Address(void);                         //************** To Do
    bool _updateI2cSlave3Control(void);                         //************** To Do
    bool _updateI2cSlave3Register(void);                        //************** To Do
    bool _updateI2cSlave4Address(void);                         //************** To Do
    bool _updateI2cSlave4Control(void);                         //************** To Do
    bool _updateI2cSlave4Register(void);                        //************** To Do
    bool _updateI2cSlave4DataOut(void);                         //************** To Do
    bool _getI2cSlave4DataIn(void);                             //************** To Do
    bool _getI2cMasterStatus(void);                             //************** To Do
    bool _updateI2cMasterControlDelay(void);                    //************** To Do
    bool _updateI2cSlave0DataOut(void);                         //************** To Do
    bool _updateI2cSlave1DataOut(void);                         //************** To Do
    bool _updateI2cSlave2DataOut(void);                         //************** To Do
    bool _updateI2cSlave3DataOut(void);                         //************** To Do

    //     //////////////////////    INTERRUPT     //////////////////////     //
    bool _updateIntPinConfig(void);                             //************** To Do
    bool _updateIntEnable(void);                                //************** To Do
    bool _getInterruptStatus(void);                             //************** To Do

    //     ///////////////////    EXTERNAL SENSOR     ///////////////////     //
    bool _readExtSensData(uint8_t first, uint8_t total);        //************** To Do

    //     ////////////////////////    RESET     ////////////////////////     //
    bool _updateSignalPathReset(void);                          //************** To Do
    bool _updatePowerManagementControl1(void);                  //************** To Do

    //     //////////////////////    LOW POWER     //////////////////////     //
    bool _updateLowPowerAccelODR(void);                         //************** To Do

    //     ///////////////    LOW LEVEL COMMUNICATION     ///////////////     //
    bool inlined _readFromDevice(uint8_t reg,
        uint8_t *buffer,
        uint8_t size);              //************** To Do
    bool inlined _writeAtDevice(uint8_t reg,
        uint8_t *buffer,
        uint8_t size);              //************** To Do

    // -------------------------------------------------------------------------
    // Properties
    // -------------------------------------------------------------------------

    // Data members - Private
    // *INDENT-OFF*
private:
    //     //////////////////////    INTERFACE     //////////////////////     //
    error_e     _lastError;
    bool        _initialized                    : 1;
    I2c         *_i2cHandler;
    uint8_t     _i2c7bitAddress                 : 7;
    bool        _useSpi                         : 1;
    bool        _commPortSet                    : 1;
    Spi         *_spiHandler;
    void    (* _activateDevice)(void);
    void    (* _deactivateDevice)(void);
    bool        _resetGyro                      : 1;
    bool        _resetAccel                     : 1;
    bool        _resetTemp                      : 1;
    bool        _resetFifo                      : 1;
    bool        _resetI2c                       : 1;
    bool        _resetSignal                    : 1;

    //     ////////////////     SENSOR CONFIGURATION     ////////////////     //
    bool        _accelDisableX                  : 1;
    bool        _accelDisableY                  : 1;
    bool        _accelDisableZ                  : 1;
    uint8_t     _accelDlpfCfg                   : 3;
    bool        _accelFChoiceB                  : 1;
    uint8_t     _accelFullScale                 : 2;
    uint8_t     _sampleRateDivider;
    bool        _gyroDisableX                   : 1;
    bool        _gyroDisableY                   : 1;
    bool        _gyroDisableZ                   : 1;
    uint8_t     _gyroDlpfCfg                    : 3;
    uint8_t     _gyroFChoiceB                   : 2;
    uint8_t     _gyroFullScale                  : 2;
    bool        _accelXSelfTestEnabled          : 1;    /// To do
    bool        _accelYSelfTestEnabled          : 1;    /// To do
    bool        _accelZSelfTestEnabled          : 1;    /// To do
    uint8_t     _accelSelfTest[3];                      /// To do
    bool        _gyroXSelfTestEnabled           : 1;    /// To do
    bool        _gyroYSelfTestEnabled           : 1;    /// To do
    bool        _gyroZSelfTestEnabled           : 1;    /// To do
    uint8_t     _gyroSelfTest[3];                       /// To do

    //     /////////////////    RAW DATA MANAGEMENT     /////////////////     //
    int16_t     _accelData[3];
    uint16_t    _accelOffset[3];
    int16_t     _gyroData[3];
    uint16_t    _gyroOffset[3];
    int16_t     _tempData;

    //     /////////////////     FIFO CONFIGURATION     /////////////////     //
    bool        _fifoMode                       : 1;
    bool        _fifoEnable                     : 1;
    uint16_t    _fifoLenght;
    bool        _fifoTempStore                  : 1;
    bool        _fifoGyroXStore                 : 1;
    bool        _fifoGyroYStore                 : 1;
    bool        _fifoGyroZStore                 : 1;
    bool        _fifoAccelStore                 : 1;
    bool        _fifoSlave3Store                : 1;
    bool        _fifoSlave2Store                : 1;
    bool        _fifoSlave1Store                : 1;
    bool        _fifoSlave0Store                : 1;

    //     /////////////    I2C MASTER BUS DATA MEMBERS     /////////////     //
    bool        _i2cMultiMaster                 : 1;
    bool        _i2cWaitForExtSensor            : 1;
    bool        _i2cStopBetweenTransmissions    : 1;
    bool        _i2cMasterClock                 : 4;
    bool        _i2cSlave0ReadWrite             : 1;
    uint8_t     _i2cSlave0Address               : 7;
    bool        _i2cSlave1ReadWrite             : 1;
    uint8_t     _i2cSlave1Address               : 7;
    bool        _i2cSlave2ReadWrite             : 1;
    uint8_t     _i2cSlave2Address               : 7;
    bool        _i2cSlave3ReadWrite             : 1;
    uint8_t     _i2cSlave3Address               : 7;
    bool        _i2cSlave4ReadWrite             : 1;
    uint8_t     _i2cSlave4Address               : 7;
    uint8_t     _i2cSlave0Register;
    uint8_t     _i2cSlave1Register;
    uint8_t     _i2cSlave2Register;
    uint8_t     _i2cSlave3Register;
    uint8_t     _i2cSlave4Register;
    uint8_t     _i2cSlave0DataOut;
    uint8_t     _i2cSlave1DataOut;
    uint8_t     _i2cSlave2DataOut;
    uint8_t     _i2cSlave3DataOut;
    uint8_t     _i2cSlave4DataOut;
    bool        _i2cSlave0Enable                : 1;
    bool        _i2cSlave0SwapBytes             : 1;
    bool        _i2cSlave0RegisterDisable       : 1;
    bool        _i2cSlave0GroupingSkipFirst     : 1;
    uint8_t     _i2cSlave0DataLength            : 4;
    bool        _i2cSlave1Enable                : 1;
    bool        _i2cSlave1SwapBytes             : 1;
    bool        _i2cSlave1RegisterDisable       : 1;
    bool        _i2cSlave1GroupingSkipFirst     : 1;
    uint8_t     _i2cSlave1DataLength            : 4;
    bool        _i2cSlave2Enable                : 1;
    bool        _i2cSlave2SwapBytes             : 1;
    bool        _i2cSlave2RegisterDisable       : 1;
    bool        _i2cSlave2GroupingSkipFirst     : 1;
    uint8_t     _i2cSlave2DataLength            : 4;
    bool        _i2cSlave3Enable                : 1;
    bool        _i2cSlave3SwapBytes             : 1;
    bool        _i2cSlave3RegisterDisable       : 1;
    bool        _i2cSlave3GroupingSkipFirst     : 1;
    uint8_t     _i2cSlave3DataLength            : 4;
    bool        _i2cSlave4Enable                : 1;
    bool        _i2cSlave4IntEnable             : 1;
    bool        _i2cSlave4RegisterDisable       : 1;
    uint8_t     _i2cSlave4Delay                 : 5;
    bool        _i2cBypassEnable                : 1;
    bool        _i2cExtSensorShadowDelayEnable  : 1;
    bool        _i2cSlave4DelayEnable           : 1;
    bool        _i2cSlave3DelayEnable           : 1;
    bool        _i2cSlave2DelayEnable           : 1;
    bool        _i2cSlave1DelayEnable           : 1;
    bool        _i2cSlave0DelayEnable           : 1;
    bool        _i2cMasterEnable                : 1;

    //     /////////////////////     INTERRUPTS     /////////////////////     //
    bool        _intPinActiveLevel              : 1;
    bool        _intPinOpenDrain                : 1;
    bool        _intPinLatchEnable              : 1;
    bool        _intPinAutoClear                : 1;
    bool        _intPinFsyncActiveLevel         : 1;
    bool        _intFsyncModeEnable             : 1;
    bool        _intWakeOnMotionEnable          : 1;
    bool        _intFifoOverfowEnable           : 1;
    bool        _intFsyncEnable                 : 1;
    bool        _intRawDataReadyEnable          : 1;
    bool        _intWakeOnMotionFlag            : 1;
    bool        _intFifoOverflowFlag            : 1;
    bool        _intFsyncFlag                   : 1;
    bool        _intI2cDataReadyFlag            : 1;

    //     //////////////////     MOTION DETECTION     //////////////////     //
    uint8_t     _wakeOnMotionThreshold;
    bool        _wakeOnMotionMode               : 1;
    bool        _wakeOnMotionEnable             : 1;

    //     //////////////////     POWER MANAGEMENT     //////////////////     //
    bool        _cycleEnable                    : 1;
    bool        _sleepEnable                    : 1;
    bool        _gyroStandby                    : 1;
    bool        _ptatDisable                    : 1;
    uint8_t     _clockSelection                 : 3;
    uint8_t     _lowPowerOscClock               : 4;

    //     /////////////    EXTERNAL SENSOR DATA MEMBER     /////////////     //
    uint8_t     _extenalSensorData[24];
    uint8_t     _extSyncSet                     : 3;
    // *INDENT-ON*
}; // class Mpu9250

// =============================================================================
// Mpu9250 - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// Mpu9250 - Class inline function definitions
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




// =============================================================================
// Public functions declarations
// =============================================================================

// NONE

#endif // __FUNSAPE_LIB_MPU9250_HPP
