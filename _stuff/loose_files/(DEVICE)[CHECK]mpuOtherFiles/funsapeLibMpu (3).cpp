/* =============================================================================
 * Project:         FunSAPE++ Embedded Library
 * File name:       funsapeLibMpu.cpp
 * Module:          MPU module interface for FunSAPE++ Embedded Library
 *                      project
 * Authors:         __AUTHORS_TO_REPLACE__
 * Build:           __BUILD_TO_REPLACE__
 * Last edition:    __DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibMpu.hpp"
#ifndef __FUNSAPE_LIB_MPU_HPP
#   error   [funsapeLibMpu.cpp] Error 16 - Header file (funsapeLibMpu.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_MPU_HPP != __BUILD_TO_REPLACE__
#   error   [funsapeLibMpu.cpp] Error 17 - Build mismatch between source (funsapeLibMpu.cpp) and header (funsapeLibMpu.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

cint8_t  mpuCompassSampleRateMax        = 100;
cint8_t  mpuPartId                      = 0x71;
cint32_t mpuTemperatureOffset           = 0;
cint32_t mpuTemperatureSensitivity      = 321;
cuint8_t mpuTwiAddressAlternate         = 0x69;
cuint8_t mpuTwiAddressStandard          = 0x68;

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

// NONE

// =============================================================================
// Class constructors
// =============================================================================

Mpu::Mpu(PartId partId_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::Mpu(PartId)", DEBUG_CODE_MPU);

    // Resets data members
    this->_clearData();
    this->_busHandler                   = nullptr;
    this->_initialized                  = false;
    this->_useSpiBus                    = false;

    // Update data members
    switch (partId_p) {
    case PartId::MPU6050:
    case PartId::MPU6500:
    case PartId::MPU9150:
    case PartId::MPU9250:
        this->_partId = partId_p;
        break;
    default:
        // Return error
        this->_lastError = Error::UNKNOWN;
        debugMessage(Error::UNKNOWN);
        return;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

// TODO: Implement destructor function
Mpu::~Mpu(void)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::~Mpu(void)", DEBUG_CODE_MPU);

    // Returns successfully
    return;
}

// =============================================================================
// Class own methods - Public
// =============================================================================

//     ////////////////     ACCELEROMETER MANAGEMENT     ////////////////     //

bool Mpu::configAccel(Axis axis_p, AccelFullScale fullScale_p, AccelDlpf dlpf_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::configAccel(Axis, AccelFullScale, AccelDlpf)", DEBUG_CODE_MPU);

    // Initialization and error checking is performed inside called functions

    // Set axes
    if (!this->setAccelAxis(axis_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Set full scale
    if (!this->setAccelFullScale(fullScale_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Set digital filter
    if (!this->setAccelDlpf(dlpf_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setAccelAxis(Axis axis_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setAccelAxis(Axis)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff = 0;
    bool auxBoolX = false;
    bool auxBoolY = false;
    bool auxBoolZ = false;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process argument
    auxBoolX = ((bool)(axis_p & Axis::X));
    auxBoolY = ((bool)(axis_p & Axis::Y));
    auxBoolZ = ((bool)(axis_p & Axis::Z));

    // Process transmission buffer
    auxBuff = 0;
    setMaskOffset(auxBuff, !(auxBoolX), Bit::PWR_MGMT_2_DISABLE_XA);
    setMaskOffset(auxBuff, !(auxBoolY), Bit::PWR_MGMT_2_DISABLE_YA);
    setMaskOffset(auxBuff, !(auxBoolZ), Bit::PWR_MGMT_2_DISABLE_ZA);
    setMaskOffset(auxBuff, !((bool)(this->_gyroAxisEnabled & Axis::X)), Bit::PWR_MGMT_2_DISABLE_XA);
    setMaskOffset(auxBuff, !((bool)(this->_gyroAxisEnabled & Axis::Y)), Bit::PWR_MGMT_2_DISABLE_YA);
    setMaskOffset(auxBuff, !((bool)(this->_gyroAxisEnabled & Axis::Z)), Bit::PWR_MGMT_2_DISABLE_ZA);

    // Sends data to device
    if (!this->_write(Register::PWR_MGMT_2, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_accelAxisEnabled = axis_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setAccelDlpf(AccelDlpf dlpf_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setAccelDlpf(AccelDlpf)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff = 0;
    bool dlpfDisabled = false;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process argument
    switch (dlpf_p) {
    case AccelDlpf::DLPF_OFF:       // Filter is disabled
        dlpfDisabled = true;
        break;
    case AccelDlpf::DLPF_5_HZ:      // Filter is enabled
    case AccelDlpf::DLPF_10_HZ:
    case AccelDlpf::DLPF_21_HZ:
    case AccelDlpf::DLPF_44_HZ:
    case AccelDlpf::DLPF_99_HZ:
    case AccelDlpf::DLPF_218_HZ:
    case AccelDlpf::DLPF_420_HZ:
        dlpfDisabled = false;
        break;
    }

    // Process transmission buffer
    auxBuff = 0;
    setMaskOffset(auxBuff, dlpfDisabled, Bit::ACCEL_CONFIG2_ACCEL_FCHOICE_B);
    setMaskOffset(auxBuff, (uint8_t)(dlpf_p), Bit::ACCEL_CONFIG2_A_DLPFCFG);

    // Send data to device
    if (!this->_write(Register::ACCEL_CONFIG2, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_accelDlpf = dlpf_p;
    this->_accelFchoiceB = dlpfDisabled;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setAccelFullScale(AccelFullScale fullScale_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setAccelFullScale(AccelFullScale)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process transmission buffer
    auxBuff = 0;
    setMaskOffset(auxBuff, (uint8_t)(fullScale_p), Bit::ACCEL_CONFIG_ACCEL_FS_SEL);

    // Sends data to device
    if (!this->_write(Register::ACCEL_CONFIG, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_accelFullScale = fullScale_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setAccelLowPowerMode(uint8_t sampleRate_p)
{
    // FIXME: Implement -> bool Mpu::setAccelLowPowerMode(uint8_t sampleRate_p)
    // Mark passage for debugging purpose
    debugMark("Mpu::setAccelLowPowerMode(uint8_t)", DEBUG_CODE_MPU);

    // Returns error
    this->_lastError = Error::UNDER_DEVELOPMENT;
    debugMessage(Error::UNDER_DEVELOPMENT);
    return false;
}

bool Mpu::getAccelData(int16_t *buff_p, Axis axis_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::getAccelData(int16_t *, Axis)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff[6];
    int16_t auxInt16 = 0;
    uint8_t index = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if (!isPointerValid(buff_p)) {
        // Returns error
        this->_lastError = Error::HANDLER_POINTER_NULL;
        debugMessage(Error::HANDLER_POINTER_NULL);
        return false;
    }
    if (axis_p == Axis::NONE) {
        // Returns error
        this->_lastError = Error::INVALID_AXIS;
        debugMessage(Error::INVALID_AXIS);
        return false;
    }

    // Reads data from device
    if (!this->_read(Register::ACCEL_XOUT_H, auxBuff, 6)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process data
    index = 0;
    if ((axis_p & Axis::X) == Axis::X) {
        auxInt16 = (int16_t)auxBuff[0];
        auxInt16 <<= 8;
        auxInt16 |= (int16_t)auxBuff[1];
        buff_p[index++] = auxInt16;
    }
    if ((axis_p & Axis::Y) == Axis::Y) {
        auxInt16 = (int16_t)auxBuff[2];
        auxInt16 <<= 8;
        auxInt16 |= (int16_t)auxBuff[3];
        buff_p[index++] = auxInt16;
    }
    if ((axis_p & Axis::Z) == Axis::Z) {
        auxInt16 = (int16_t)auxBuff[4];
        auxInt16 <<= 8;
        auxInt16 |= (int16_t)auxBuff[5];
        buff_p[index++] = auxInt16;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::getAccelOffset(int16_t *buff_p, Axis axis_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::getAccelOffset(int16_t *, Axis)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff[8];
    int16_t auxInt16 = 0;
    uint8_t index = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if (!isPointerValid(buff_p)) {
        // Returns error
        this->_lastError = Error::HANDLER_POINTER_NULL;
        debugMessage(Error::HANDLER_POINTER_NULL);
        return false;
    }
    if (axis_p == Axis::NONE) {
        // Returns error
        this->_lastError = Error::INVALID_AXIS;
        debugMessage(Error::INVALID_AXIS);
        return false;
    }

    // Reads data from device
    if (!this->_read(Register::XA_OFFSET_H, auxBuff, 8)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process data
    index = 0;
    if ((axis_p & Axis::X) == Axis::X) {
        auxInt16 = (int16_t)auxBuff[0];
        auxInt16 <<= 8;
        auxInt16 |= (int16_t)auxBuff[1];
        auxInt16 >>= 1;
        buff_p[index++] = auxInt16;
    }
    // auxBuff[2] is unused
    if ((axis_p & Axis::Y) == Axis::Y) {
        auxInt16 = (int16_t)auxBuff[3];
        auxInt16 <<= 8;
        auxInt16 |= (int16_t)auxBuff[4];
        auxInt16 >>= 1;
        buff_p[index++] = auxInt16;
    }
    // auxBuff[5] is unused
    if ((axis_p & Axis::Z) == Axis::Z) {
        auxInt16 = (int16_t)auxBuff[6];
        auxInt16 <<= 8;
        auxInt16 |= (int16_t)auxBuff[7];
        auxInt16 >>= 1;
        buff_p[index++] = auxInt16;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setAccelOffset(int16_t *buff_p, Axis axis_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setAccelOffset(int16_t *, Axis)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff[2];
    int16_t auxInt16 = 0;
    uint8_t index = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if (!isPointerValid(buff_p)) {
        // Returns error
        this->_lastError = Error::HANDLER_POINTER_NULL;
        debugMessage(Error::HANDLER_POINTER_NULL);
        return false;
    }
    if (axis_p == Axis::NONE) {
        // Returns error
        this->_lastError = Error::INVALID_AXIS;
        debugMessage(Error::INVALID_AXIS);
        return false;
    }

    // Process arguments
    index = 0;
    if ((axis_p & Axis::X) == Axis::X) {
        // Process transmission buffer
        auxInt16 = buff_p[index++];
        setMaskOffset(auxBuff[1], (auxInt16 & BitMask::XA_OFFSET_L), Bit::XA_OFFSET_L);
        auxInt16 >>= 7;
        setMaskOffset(auxBuff[0], (auxInt16 & BitMask::XA_OFFSET_H), Bit::XA_OFFSET_H);
        // Sends data to device
        if (!this->_write(Register::XA_OFFSET_H, auxBuff, 2)) {
            // Returns error
            debugMessage(this->_lastError);
            return false;
        }
    }
    if ((axis_p & Axis::Y) == Axis::Y) {
        // Process transmission buffer
        auxInt16 = buff_p[index++];
        setMaskOffset(auxBuff[1], (auxInt16 & BitMask::YA_OFFSET_L), Bit::YA_OFFSET_L);
        auxInt16 >>= 7;
        setMaskOffset(auxBuff[0], (auxInt16 & BitMask::YA_OFFSET_H), Bit::YA_OFFSET_H);
        // Sends data to device
        if (!this->_write(Register::YA_OFFSET_H, auxBuff, 2)) {
            // Returns error
            debugMessage(this->_lastError);
            return false;
        }
    }
    if ((axis_p & Axis::Z) == Axis::Z) {
        // Process transmission buffer
        auxInt16 = buff_p[index++];
        setMaskOffset(auxBuff[1], (auxInt16 & BitMask::ZA_OFFSET_L), Bit::ZA_OFFSET_L);
        auxInt16 >>= 7;
        setMaskOffset(auxBuff[0], (auxInt16 & BitMask::ZA_OFFSET_H), Bit::ZA_OFFSET_H);
        // Sends data to device
        if (!this->_write(Register::ZA_OFFSET_H, auxBuff, 2)) {
            // Returns error
            debugMessage(this->_lastError);
            return false;
        }
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     ///////////////////     COMPASS MANAGEMENT     ///////////////////     //

bool Mpu::setCompassSampleRate(uint8_t sampleRate_p)
{
    // FIXME: Implement -> bool Mpu::setCompassSampleRate(uint8_t sampleRate_p)
    // Mark passage for debugging purpose
    debugMark("Mpu::setCompassSampleRate(uint8_t)", DEBUG_CODE_MPU);

    // Returns error
    this->_lastError = Error::UNDER_DEVELOPMENT;
    debugMessage(Error::UNDER_DEVELOPMENT);
    return false;
}

bool Mpu::setupCompass(void)
{
    // FIXME: Implement -> bool Mpu::setupCompass(void)
    // Mark passage for debugging purpose
    debugMark("Mpu::setupCompass(void)", DEBUG_CODE_MPU);

    // Returns error
    this->_lastError = Error::UNDER_DEVELOPMENT;
    debugMessage(Error::UNDER_DEVELOPMENT);
    return false;
}

//     /////////////////////    FIFO MANAGEMENT     /////////////////////     //

bool Mpu::configFifo(uint8_t sensors_p)
{
    // FIXME: Implement -> bool Mpu::configFifo(uint8_t sensors_p)
    // Mark passage for debugging purpose
    debugMark("Mpu::configFifo(uint8_t)", DEBUG_CODE_MPU);

    // Returns error
    this->_lastError = Error::UNDER_DEVELOPMENT;
    debugMessage(Error::UNDER_DEVELOPMENT);
    return false;
}

//     //////////////////     GYROSCOPE MANAGEMENT     //////////////////     //

bool Mpu::configGyro(Axis axis_p, GyroFullScale fullScale_p, GyroDlpf dlpf_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::configGyro(Axis, GyroFullScale, GyroDlpf)", DEBUG_CODE_MPU);

    // Initialization and error checking is performed inside called functions

    // Set axes
    if (!this->setGyroAxis(axis_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Set full scale
    if (!this->setGyroFullScale(fullScale_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Set digital filter
    if (!this->setGyroDlpf(dlpf_p)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setGyroAxis(Axis axis_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setGyroAxis(Axis)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff = 0;
    bool auxBoolX = false;
    bool auxBoolY = false;
    bool auxBoolZ = false;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process argument
    auxBoolX = ((bool)(axis_p & Axis::X));
    auxBoolY = ((bool)(axis_p & Axis::Y));
    auxBoolZ = ((bool)(axis_p & Axis::Z));

    // Process transmission buffer
    auxBuff = 0;
    setMaskOffset(auxBuff, !((bool)(this->_accelAxisEnabled & Axis::X)), Bit::PWR_MGMT_2_DISABLE_XA);
    setMaskOffset(auxBuff, !((bool)(this->_accelAxisEnabled & Axis::Y)), Bit::PWR_MGMT_2_DISABLE_YA);
    setMaskOffset(auxBuff, !((bool)(this->_accelAxisEnabled & Axis::Z)), Bit::PWR_MGMT_2_DISABLE_ZA);
    setMaskOffset(auxBuff, !(auxBoolX), Bit::PWR_MGMT_2_DISABLE_XG);
    setMaskOffset(auxBuff, !(auxBoolY), Bit::PWR_MGMT_2_DISABLE_YG);
    setMaskOffset(auxBuff, !(auxBoolZ), Bit::PWR_MGMT_2_DISABLE_ZG);

    // Sends data to device
    if (!this->_write(Register::PWR_MGMT_2, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_gyroAxisEnabled = axis_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setGyroDlpf(GyroDlpf dlpf_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setGyroDlpf(GyroDlpf)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff[2] = {0, 0};
    uint8_t fChoiceB = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process argument
    switch (dlpf_p) {
    case GyroDlpf::DLPF_OFF:        // Filter is disabled
        fChoiceB = 3;
        break;
    case GyroDlpf::DLPF_188_HZ: // Filter is enabled
    case GyroDlpf::DLPF_98_HZ:
    case GyroDlpf::DLPF_42_HZ:
    case GyroDlpf::DLPF_20_HZ:
    case GyroDlpf::DLPF_10_HZ:
    case GyroDlpf::DLPF_5_HZ:
        fChoiceB = 0;
        break;
    }

    // Process transmission buffer
    auxBuff[0] = 0;
    auxBuff[1] = 0;
    setMaskOffset(auxBuff[0], this->_fifoRollOverDisabled, Bit::CONFIG_FIFO_MODE);
    setMaskOffset(auxBuff[0], (uint8_t)(this->_externalSync), Bit::CONFIG_EXT_SYNC_SET);
    setMaskOffset(auxBuff[0], (uint8_t)(dlpf_p), Bit::CONFIG_DLPF_CFG);
    setMaskOffset(auxBuff[1], fChoiceB, Bit::GYRO_CONFIG_FCHOICE_B);
    setMaskOffset(auxBuff[1], (uint8_t)(this->_gyroFullScale), Bit::GYRO_CONFIG_GYRO_FS_SEL);

    // Send data to device
    if (!this->_write(Register::CONFIG, auxBuff, 2)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_gyroDlpf = dlpf_p;
    this->_gyroFchoiceB = fChoiceB;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setGyroFullScale(GyroFullScale fullScale_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setGyroFullScale(GyroFullScale)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process transmission buffer
    auxBuff = 0;
    setMaskOffset(auxBuff, (uint8_t)(fullScale_p), Bit::GYRO_CONFIG_GYRO_FS_SEL);
    setMaskOffset(auxBuff, this->_gyroFchoiceB, Bit::GYRO_CONFIG_FCHOICE_B);

    // Sends data to device
    if (!this->_write(Register::GYRO_CONFIG, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_gyroFullScale = fullScale_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::getGyroData(int16_t *buff_p, Axis axis_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::getGyroData(int16_t *, Axis)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff[6];
    int16_t auxInt16 = 0;
    uint8_t index = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if (!isPointerValid(buff_p)) {
        // Returns error
        this->_lastError = Error::HANDLER_POINTER_NULL;
        debugMessage(Error::HANDLER_POINTER_NULL);
        return false;
    }
    if (axis_p == Axis::NONE) {
        // Returns error
        this->_lastError = Error::INVALID_AXIS;
        debugMessage(Error::INVALID_AXIS);
        return false;
    }

    // Reads data from device
    if (!this->_read(Register::GYRO_XOUT_H, auxBuff, 6)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process data
    index = 0;
    if ((axis_p & Axis::X) == Axis::X) {
        auxInt16 = (int16_t)auxBuff[0];
        auxInt16 <<= 8;
        auxInt16 |= (int16_t)auxBuff[1];
        buff_p[index++] = auxInt16;
    }
    if ((axis_p & Axis::Y) == Axis::Y) {
        auxInt16 = (int16_t)auxBuff[2];
        auxInt16 <<= 8;
        auxInt16 |= (int16_t)auxBuff[3];
        buff_p[index++] = auxInt16;
    }
    if ((axis_p & Axis::Z) == Axis::Z) {
        auxInt16 = (int16_t)auxBuff[4];
        auxInt16 <<= 8;
        auxInt16 |= (int16_t)auxBuff[5];
        buff_p[index++] = auxInt16;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::getGyroOffset(int16_t *buff_p, Axis axis_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::getGyroOffset(int16_t *, Axis)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff[6];
    int16_t auxInt16 = 0;
    uint8_t index = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if (!isPointerValid(buff_p)) {
        // Returns error
        this->_lastError = Error::HANDLER_POINTER_NULL;
        debugMessage(Error::HANDLER_POINTER_NULL);
        return false;
    }
    if (axis_p == Axis::NONE) {
        // Returns error
        this->_lastError = Error::INVALID_AXIS;
        debugMessage(Error::INVALID_AXIS);
        return false;
    }

    // Reads data from device
    if (!this->_read(Register::XG_OFFSET_H, auxBuff, 6)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process data
    index = 0;
    if ((axis_p & Axis::X) == Axis::X) {
        auxInt16 = (int16_t)auxBuff[0];
        auxInt16 <<= 8;
        auxInt16 |= (int16_t)auxBuff[1];
        buff_p[index++] = auxInt16;
    }
    if ((axis_p & Axis::Y) == Axis::Y) {
        auxInt16 = (int16_t)auxBuff[2];
        auxInt16 <<= 8;
        auxInt16 |= (int16_t)auxBuff[3];
        buff_p[index++] = auxInt16;
    }
    if ((axis_p & Axis::Z) == Axis::Z) {
        auxInt16 = (int16_t)auxBuff[4];
        auxInt16 <<= 8;
        auxInt16 |= (int16_t)auxBuff[5];
        buff_p[index++] = auxInt16;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setGyroOffset(int16_t *buff_p, Axis axis_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setGyroOffset(int16_t *, Axis)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff[2];
    int16_t auxInt16 = 0;
    uint8_t index = 0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if (!isPointerValid(buff_p)) {
        // Returns error
        this->_lastError = Error::HANDLER_POINTER_NULL;
        debugMessage(Error::HANDLER_POINTER_NULL);
        return false;
    }
    if (axis_p == Axis::NONE) {
        // Returns error
        this->_lastError = Error::INVALID_AXIS;
        debugMessage(Error::INVALID_AXIS);
        return false;
    }

    // Process arguments
    index = 0;
    if ((axis_p & Axis::X) == Axis::X) {
        // Process transmission buffer
        auxInt16 = buff_p[index++];
        setMaskOffset(auxBuff[1], (auxInt16 & BitMask::XG_OFFSET_L), Bit::XG_OFFSET_L);
        auxInt16 >>= 8;
        setMaskOffset(auxBuff[0], (auxInt16 & BitMask::XG_OFFSET_H), Bit::XG_OFFSET_H);
        // Sends data to device
        if (!this->_write(Register::XG_OFFSET_H, auxBuff, 2)) {
            // Returns error
            debugMessage(this->_lastError);
            return false;
        }
    }
    if ((axis_p & Axis::Y) == Axis::Y) {
        // Process transmission buffer
        auxInt16 = buff_p[index++];
        setMaskOffset(auxBuff[1], (auxInt16 & BitMask::YG_OFFSET_L), Bit::YG_OFFSET_L);
        auxInt16 >>= 8;
        setMaskOffset(auxBuff[0], (auxInt16 & BitMask::YG_OFFSET_H), Bit::YG_OFFSET_H);
        // Sends data to device
        if (!this->_write(Register::YG_OFFSET_H, auxBuff, 2)) {
            // Returns error
            debugMessage(this->_lastError);
            return false;
        }
    }
    if ((axis_p & Axis::Z) == Axis::Z) {
        // Process transmission buffer
        auxInt16 = buff_p[index++];
        setMaskOffset(auxBuff[1], (auxInt16 & BitMask::ZG_OFFSET_L), Bit::ZG_OFFSET_L);
        auxInt16 >>= 8;
        setMaskOffset(auxBuff[0], (auxInt16 & BitMask::ZG_OFFSET_H), Bit::ZG_OFFSET_H);
        // Sends data to device
        if (!this->_write(Register::ZG_OFFSET_H, auxBuff, 2)) {
            // Returns error
            debugMessage(this->_lastError);
            return false;
        }
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     //////////////////     INTERRUPT MANAGEMENT     //////////////////     //

bool Mpu::configIntPin(LogicLevel pinLevel_p, DriverMode pinDriver_p, bool pinLatch_p, bool pinAutoReset_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::configIntPin(LogicLevel, DriverMode, bool, bool)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process transmission buffer
    auxBuff = 0;
    setMaskOffset(auxBuff, !(bool)(pinLevel_p), Bit::INT_PIN_CFG_ACTL);
    setMaskOffset(auxBuff, (bool)(pinDriver_p), Bit::INT_PIN_CFG_OPEN);
    setMaskOffset(auxBuff, (bool)(pinLatch_p), Bit::INT_PIN_CFG_LATCH_INT_EN);
    setMaskOffset(auxBuff, (bool)(pinAutoReset_p), Bit::INT_PIN_CFG_INT_ANYRD_2CLEAR);
    setMaskOffset(auxBuff, !(bool)(this->_fsyncIntPinActiveLevel), Bit::INT_PIN_CFG_ACTL_FSYNC);
    setMaskOffset(auxBuff, this->_fsyncPinTriggersInterrupt, Bit::INT_PIN_CFG_FSYNC_INT_MODE_EN);
    setMaskOffset(auxBuff, this->_i2cRoutedToAuxBus, Bit::INT_PIN_CFG_BYPASS_EN);

    // Sends data to device
    if (!this->_write(Register::INT_PIN_CFG, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_intPinActiveLevel = pinLevel_p;
    this->_intPinDriverMode = pinDriver_p;
    this->_intPinLatchEnabled = pinLatch_p;
    this->_intPinAutoResetEnabled = pinAutoReset_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::disableInterrupts(InterruptFlags deactivationMask_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::disableInterrupts(InterruptFlags)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff;
    bool intFifoOverflow                = false;
    bool intFsyncPin                    = false;
    bool intRawDataReady                = false;
    bool intWakeUpOnMotion              = false;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Populate the variables with the class members values
    intFifoOverflow = this->_intFifoOverflowEnabled;
    intFsyncPin = this->_intFsyncPinEnabled;
    intRawDataReady = this->_intRawDataReadyEnabled;
    intWakeUpOnMotion = this->_intWakeUpOnMotionEnabled;

    // Process the deactivation mask
    if (deactivationMask_p & InterruptFlags::FIFO_OVERFLOW) {
        intFifoOverflow = false;
    }
    if (deactivationMask_p & InterruptFlags::FSYNC) {
        intFsyncPin = false;
    }
    if (deactivationMask_p & InterruptFlags::RAW_DATA_READY) {
        intRawDataReady = false;
    }
    if (deactivationMask_p & InterruptFlags::WAKE_UP_ON_MOTION) {
        intWakeUpOnMotion = false;
    }

    // Process transmission buffer
    auxBuff = 0;
    setMaskOffset(auxBuff, intWakeUpOnMotion, Bit::INT_ENABLE_WOM_EN);
    setMaskOffset(auxBuff, intFifoOverflow, Bit::INT_ENABLE_FIFO_OVERFLOW_EN);
    setMaskOffset(auxBuff, intFsyncPin, Bit::INT_ENABLE_FSYNC_INT_EN);
    setMaskOffset(auxBuff, intRawDataReady, Bit::INT_ENABLE_RAW_RDY_EN);

    // Sends data to device
    if (!this->_write(Register::INT_ENABLE, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_intFifoOverflowEnabled = intFifoOverflow;
    this->_intFsyncPinEnabled = intFsyncPin;
    this->_intRawDataReadyEnabled = intRawDataReady;
    this->_intWakeUpOnMotionEnabled = intWakeUpOnMotion;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::enableInterrupts(InterruptFlags activationMask_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::enableInterrupts(InterruptFlags)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff;
    bool intFifoOverflow                = false;
    bool intFsyncPin                    = false;
    bool intRawDataReady                = false;
    bool intWakeUpOnMotion              = false;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Populate the variables with the class members values
    intFifoOverflow = this->_intFifoOverflowEnabled;
    intFsyncPin = this->_intFsyncPinEnabled;
    intRawDataReady = this->_intRawDataReadyEnabled;
    intWakeUpOnMotion = this->_intWakeUpOnMotionEnabled;

    // Process the deactivation mask
    if (activationMask_p & InterruptFlags::FIFO_OVERFLOW) {
        intFifoOverflow = true;
    }
    if (activationMask_p & InterruptFlags::FSYNC) {
        intFsyncPin = true;
    }
    if (activationMask_p & InterruptFlags::RAW_DATA_READY) {
        intRawDataReady = true;
    }
    if (activationMask_p & InterruptFlags::WAKE_UP_ON_MOTION) {
        intWakeUpOnMotion = true;
    }

    // Process transmission buffer
    auxBuff = 0;
    setMaskOffset(auxBuff, intWakeUpOnMotion, Bit::INT_ENABLE_WOM_EN);
    setMaskOffset(auxBuff, intFifoOverflow, Bit::INT_ENABLE_FIFO_OVERFLOW_EN);
    setMaskOffset(auxBuff, intFsyncPin, Bit::INT_ENABLE_FSYNC_INT_EN);
    setMaskOffset(auxBuff, intRawDataReady, Bit::INT_ENABLE_RAW_RDY_EN);

    // Sends data to device
    if (!this->_write(Register::INT_ENABLE, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_intFifoOverflowEnabled = intFifoOverflow;
    this->_intFsyncPinEnabled = intFsyncPin;
    this->_intRawDataReadyEnabled = intRawDataReady;
    this->_intWakeUpOnMotionEnabled = intWakeUpOnMotion;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::generalIrqHandler(void)
{
    // Note: Since this function is called asynchronously, no debug mark passage
    // was placed for caution. The same happens with the updateInterruptStatus()
    // function that is called inside this function.

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Read the interrupt status registers
    if (!this->_updateInterruptStatus()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Call callback functions
    if (this->_intFifoOverflowStatus) {
        this->_intFifoOverflowCallback();
        this->_intFifoOverflowStatus = false;
    }
    if (this->_intFsyncPinStatus) {
        this->_intFsyncPinCallback();
        this->_intFsyncPinStatus = false;
    }
    if (this->_intRawDataReadyStatus) {
        this->_intRawDataReadyCallback();
        this->_intRawDataReadyStatus = false;
    }
    if (this->_intWakeUpOnMotionStatus) {
        this->_intWakeUpOnMotionCallback();
        this->_intWakeUpOnMotionStatus = false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setIntFifoOverflowCallback(void (*callbackFunction_p)(void))
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setIntFifoOverflowCallback(void (*)(void))", DEBUG_CODE_MPU);

    // Check function arguments for errors
    if (!isPointerValid(callbackFunction_p)) {
        // Returns error
        this->_lastError = Error::FUNCTION_POINTER_NULL;
        debugMessage(Error::FUNCTION_POINTER_NULL);
        return false;
    }

    // Update data members
    this->_intFifoOverflowCallback = callbackFunction_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setIntFsyncPinCallback(void (*callbackFunction_p)(void))
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setIntFsyncPinCallback(void (*)(void))", DEBUG_CODE_MPU);

    // Check function arguments for errors
    if (!isPointerValid(callbackFunction_p)) {
        // Returns error
        this->_lastError = Error::FUNCTION_POINTER_NULL;
        debugMessage(Error::FUNCTION_POINTER_NULL);
        return false;
    }

    // Update data members
    this->_intFsyncPinCallback = callbackFunction_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setIntRawDataReadyCallback(void (*callbackFunction_p)(void))
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setIntRawDataReadyCallback(void (*)(void))", DEBUG_CODE_MPU);

    // Check function arguments for errors
    if (!isPointerValid(callbackFunction_p)) {
        // Returns error
        this->_lastError = Error::FUNCTION_POINTER_NULL;
        debugMessage(Error::FUNCTION_POINTER_NULL);
        return false;
    }

    // Update data members
    this->_intRawDataReadyCallback = callbackFunction_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setIntWakeUpOnMotionCallback(void (*callbackFunction_p)(void))
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setIntWakeUpOnMotionCallback(void (*)(void))", DEBUG_CODE_MPU);

    // Check function arguments for errors
    if (!isPointerValid(callbackFunction_p)) {
        // Returns error
        this->_lastError = Error::FUNCTION_POINTER_NULL;
        debugMessage(Error::FUNCTION_POINTER_NULL);
        return false;
    }

    // Update data members
    this->_intWakeUpOnMotionCallback = callbackFunction_p;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

Error Mpu::getLastError(void)
{
    // Returns last error
    return this->_lastError;
}

bool Mpu::init(Bus *twiBusHandler_p, bool useTwiAlternateAddress_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::init(Bus *, bool)", DEBUG_CODE_MPU);

    // Resets data members
    this->_clearData();
    this->_busHandler                   = nullptr;
    this->_initialized                  = false;
    this->_useSpiBus                    = false;

    // Check function arguments for errors
    if (!isPointerValid(twiBusHandler_p)) {
        // Returns error
        this->_lastError = Error::HANDLER_POINTER_NULL;
        debugMessage(Error::HANDLER_POINTER_NULL);
        return false;
    } else {
        switch (twiBusHandler_p->getBusType()) {
        case Bus::BusType::TWI:
            break;
        case Bus::BusType::SPI:
            // Returns error
            this->_lastError = Error::ARGUMENT_VALUE_INVALID;
            debugMessage(Error::ARGUMENT_VALUE_INVALID);
            return false;
        default:
            // Returns error
            this->_lastError = Error::HANDLER_UNSUPPORTED;
            debugMessage(Error::HANDLER_UNSUPPORTED);
            return false;
        }
    }

    // Update data members
    this->_busHandler = twiBusHandler_p;
    this->_csPin = nullptr;
    this->_initialized = true;
    this->_twiAddress = (useTwiAlternateAddress_p) ? mpuTwiAddressAlternate : mpuTwiAddressStandard;
    this->_useSpiBus = false;

    // Initializes device
    if (!this->_init()) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::init(Bus *spiBusHandler_p, Gpio *csPin_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::init(Bus *, Gpio *)", DEBUG_CODE_MPU);

    // Resets data members
    this->_clearData();
    this->_busHandler                   = nullptr;
    this->_initialized                  = false;
    this->_useSpiBus                    = false;

    // Check function arguments for errors
    if (!isPointerValid(spiBusHandler_p)) {
        // Returns error
        this->_lastError = Error::HANDLER_POINTER_NULL;
        debugMessage(Error::HANDLER_POINTER_NULL);
        return false;
    } else {
        switch (spiBusHandler_p->getBusType()) {
        case Bus::BusType::SPI:
            break;
        case Bus::BusType::TWI:
            // Returns error
            this->_lastError = Error::ARGUMENT_VALUE_INVALID;
            debugMessage(Error::ARGUMENT_VALUE_INVALID);
            return false;
        default:
            // Returns error
            this->_lastError = Error::HANDLER_UNSUPPORTED;
            debugMessage(Error::HANDLER_UNSUPPORTED);
            return false;
        }
    }
    if (!isPointerValid(csPin_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Update data members
    this->_busHandler = spiBusHandler_p;
    this->_csPin = csPin_p;
    this->_initialized = true;
    this->_twiAddress = 0;
    this->_useSpiBus = true;

    // Initializes device
    if (!this->_init()) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::reset(void)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::reset(void)", DEBUG_CODE_MPU);

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Sends reset signal
    this->_reset();

    // Clears data members
    this->_clearData();

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::setSampleRate(uint16_t sampleRate_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::setSampleRate(uint16_t)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff = 0;
    uint16_t aux16 = 0;
    GyroDlpf dlpf = GyroDlpf::DLPF_OFF;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process argument
    if (this->_st_chip_cfg_lp_accel_mode) {     // Low power mode
        if ((sampleRate_p = 0) || (sampleRate_p > 40)) {
            // Returns error
            this->_lastError = Error::ARGUMENT_VALUE_INVALID;
            debugMessage(Error::ARGUMENT_VALUE_INVALID);
            return false;
        }
    } else {                            // Full power mode
        if ((sampleRate_p < 4) || (sampleRate_p > 1000)) {
            // Returns error
            this->_lastError = Error::ARGUMENT_VALUE_INVALID;
            debugMessage(Error::ARGUMENT_VALUE_INVALID);
            return false;
        }
    }

    // Process transmission buffer
    if (!this->_st_chip_cfg_lp_accel_mode) {        // Full power mode
        aux16 = ((1000 / sampleRate_p) - 1);
        auxBuff = (uint8_t)aux16;
    }

    // Sends data to device
    if (this->_st_chip_cfg_lp_accel_mode) {     // Low power mode
        this->setAccelLowPowerMode(sampleRate_p);
    } else {                                    // Full power mode
        if (!this->_write(Register::SMPLRT_DIV, &auxBuff, 1)) {
            // Returns error
            debugMessage(this->_lastError);
            return false;
        }
        if (!this->_write(Register::SMPLRT_DIV, &auxBuff, 1)) {
            // Returns error
            debugMessage(this->_lastError);
            return false;
        }
    }

    // Update data members
    this->_sampleRate = 1000 / (1 + aux16);

    // Sets compass sample rate
    auxBuff = (uint8_t)getMinimumOf(this->_compassSampleRate, mpuCompassSampleRateMax);

    if (!this->setCompassSampleRate(auxBuff)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Adjusts filter
    aux16 = (this->_sampleRate / 2);
    if (aux16 >= 188) {
        dlpf = GyroDlpf::DLPF_188_HZ;
    } else if (aux16 >= 98) {
        dlpf = GyroDlpf::DLPF_98_HZ;
    } else if (aux16 >= 42) {
        dlpf = GyroDlpf::DLPF_42_HZ;
    } else if (aux16 >= 20) {
        dlpf = GyroDlpf::DLPF_20_HZ;
    } else if (aux16 >= 10) {
        dlpf = GyroDlpf::DLPF_10_HZ;
    } else {
        dlpf = GyroDlpf::DLPF_5_HZ;
    }
    if (!this->setGyroDlpf(dlpf)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     /////////////////     TEMPERATURE MANAGEMENT     /////////////////     //

bool Mpu::getTemperature(float *tempValue_p, ThermometricScale tempScale_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::getTemperature(float *, ThermometricScale)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff[2];
    int32_t auxInt32 = 0;
    float auxFloat = 0.0;

    // Check device's initialization
    if (!this->_isInitialized()) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Check function arguments for errors
    if (!isPointerValid(tempValue_p)) {
        // Returns error
        this->_lastError = Error::ARGUMENT_POINTER_NULL;
        debugMessage(Error::ARGUMENT_POINTER_NULL);
        return false;
    }

    // Reads die temperature register
    if (!this->_read(Register::TEMP_OUT_H, auxBuff, 2)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Process transmission buffer
    auxInt32 = auxBuff[0];
    auxInt32 <<= 8;
    auxInt32 |= auxBuff[1];

    // Calculates temperature (in Q16 format)
    auxInt32 -= mpuTemperatureOffset;
    auxInt32 += (21 * mpuTemperatureSensitivity);
    auxInt32 *= 65536;
    auxInt32 /= mpuTemperatureSensitivity;

    // Converts temperature to float
    auxFloat = (float)auxInt32;
    auxFloat /= 65536.0;

    // Converts to the thermometric scale
    switch (tempScale_p) {
    case ThermometricScale::CELSIUS:
        *tempValue_p = auxFloat;
        break;
    case ThermometricScale::DELISLE:
    case ThermometricScale::FAHRENHEIT:
    case ThermometricScale::KELVIN:
    case ThermometricScale::NEWTON:
    case ThermometricScale::RANKINE:
    case ThermometricScale::REAUMUR:
    case ThermometricScale::ROMER:
        // TODO: Implement the conversion between thermometric scales
        // Returns error
        this->_lastError = Error::UNDER_DEVELOPMENT;
        debugMessage(Error::UNDER_DEVELOPMENT);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::getTemperature(Int32Q16_t *tempQ16_p, ThermometricScale tempScale_p)
{
    // FIXME: Implement -> bool Mpu::getTemperature(Int32Q16_t *tempQ16_p, ThermometricScale tempScale_p)
    // Returns error
    this->_lastError = Error::UNDER_DEVELOPMENT;
    debugMessage(Error::UNDER_DEVELOPMENT);
    return false;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

void Mpu::_clearData(void)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::_clearData(void)", DEBUG_CODE_MPU);

    //     /////////////////     CONTROL AND STATUS     /////////////////     //
    this->_initialized                  = false;
    this->_fifoRollOverDisabled         = false;
    this->_externalSync                 = ExternalSync::OFF;
    this->_sampleRate                   = 0;
    //     /////////////     ACCELEROMETER RELATED DATA     /////////////     //
    this->_st_chip_cfg_accel_half       = false;
    this->_st_chip_cfg_lp_accel_mode    = false;
    this->_accelDlpf                    = AccelDlpf::DLPF_OFF;
    this->_accelAxisEnabled             = Axis::NONE;
    this->_accelFchoiceB                = false;
    this->_accelFullScale               = AccelFullScale::FS_2G;
    //     ///////////////     GYROSCOPE RELATED DATA     ///////////////     //
    this->_gyroDlpf                     = GyroDlpf::DLPF_OFF;
    this->_gyroAxisEnabled              = Axis::NONE;
    this->_gyroFchoiceB                 = 0;
    this->_gyroFullScale                = GyroFullScale::FS_250_DPS;
    //     ////////////////     COMPASS RELATED DATA     ////////////////     //
    this->_compassSampleRate            = 0;
    //     ///////////////     INTERRUPT RELATED DATA     ///////////////     //
    this->_fsyncIntPinActiveLevel       = LogicLevel::LOW;
    this->_fsyncPinTriggersInterrupt    = false;
    this->_i2cRoutedToAuxBus            = false;
    this->_intFifoOverflowEnabled       = false;
    this->_intFifoOverflowStatus        = false;
    this->_intFsyncPinEnabled           = false;
    this->_intFsyncPinStatus            = false;
    this->_intPinActiveLevel            = LogicLevel::LOW;
    this->_intPinAutoResetEnabled       = false;
    this->_intPinDriverMode             = DriverMode::PUSH_PULL;
    this->_intPinLatchEnabled           = false;
    this->_intRawDataReadyEnabled       = false;
    this->_intRawDataReadyStatus        = false;
    this->_intWakeUpOnMotionEnabled     = false;
    this->_intWakeUpOnMotionStatus      = false;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return;
}

bool Mpu::_init(void)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::_init(void)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff;

    // Resets device
    if (!this->_reset()) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    this->_st_chip_cfg_accel_half = false;

    // Retrieve device's part IDs
    if (!this->_read(Register::WHO_AM_I, &auxBuff, 1)) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Check device ID
    auxBuff = auxBuff >> Bit::WHO_AM_I;
    auxBuff &= BitMask::WHO_AM_I;
    if (auxBuff != mpuPartId) {
        // Returns error
        this->_initialized = false;
        this->_lastError = Error::DEVICE_ID_MATCH_FAILED;
        debugMessage(Error::DEVICE_ID_MATCH_FAILED);
        return false;
    }

    // Disables digital filtering for accelerometers
    if (!this->setAccelDlpf(AccelDlpf::DLPF_OFF)) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Sets gyroscope full scale
    if (!this->setGyroFullScale(GyroFullScale::FS_2000_DPS)) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Sets accelerometer full scale
    if (!this->setAccelFullScale(AccelFullScale::FS_2G)) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Sets gyroscope digital filter
    if (!this->setGyroDlpf(GyroDlpf::DLPF_42_HZ)) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Sets gyroscope digital filter
    if (!this->setGyroDlpf(GyroDlpf::DLPF_42_HZ)) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Sets sample rate
    if (!this->setSampleRate(50)) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Disables FIFO
    if (!this->configFifo(0)) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Setups compass
    if (!this->setupCompass()) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Disable all axis
    if (!this->setAccelAxis(Axis::NONE)) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }
    if (!this->setGyroAxis(Axis::NONE)) {
        // Returns error
        this->_initialized = false;
        debugMessage(this->_lastError);
        return false;
    }

    // Update data members
    this->_initialized                  = true;

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::_isInitialized(void)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::_isInitialized(void)", DEBUG_CODE_MPU);

    // Checks if device is initialized
    if (!this->_initialized) {
        // Returns error
        this->_lastError = Error::NOT_INITIALIZED;
        debugMessage(Error::NOT_INITIALIZED);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::_reset(void)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::_reset(void)", DEBUG_CODE_MPU);

    // Local variables
    uint8_t auxBuff;

    // Sends reset signal to device
    auxBuff = (1 << Bit::PWR_MGMT_1_H_RESET);
    if (!this->_write(Register::PWR_MGMT_1, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    delayMs(100);

    // Wakes up chip
    auxBuff = (0 << Bit::PWR_MGMT_1_H_RESET);
    if (!this->_write(Register::PWR_MGMT_1, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     ///////////////////////     INTERRUPTS     ///////////////////////     //

bool Mpu::_updateInterruptStatus(void)
{
    // Local variables
    uint8_t auxBuff;

    // Note: This function is also called by the generalIrqHandler() function.
    // Since the generalIrqHandler() function is called asynchronously,
    // triggered by interrupts, no debug mark passage was placed for caution.

    // Read the interrupt status registers
    if (!this->_read(Register::INT_STATUS, &auxBuff, 1)) {
        // Returns error
        debugMessage(this->_lastError);
        return false;
    }

    // Update class data members
    if (this->_intFifoOverflowEnabled) {
        this->_intFifoOverflowStatus = (bool)(auxBuff & (1 << Bit::INT_STATUS_FIFO_OVERFLOW_INT));
    }
    if (this->_intFsyncPinEnabled) {
        this->_intFsyncPinStatus = (bool)(auxBuff & (1 << Bit::INT_STATUS_FSYNC_INT));
    }
    if (this->_intRawDataReadyEnabled) {
        this->_intRawDataReadyStatus = (bool)(auxBuff & (1 << Bit::INT_STATUS_RAW_DATA_RDY_INT));
    }
    if (this->_intWakeUpOnMotionEnabled) {
        this->_intWakeUpOnMotionStatus = (bool)(auxBuff & (1 << Bit::INT_STATUS_WOM_INT));
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

//     //////////////////    REGISTER MANIPULATION     //////////////////     //

bool Mpu::_read(Register regAddr_p, uint8_t *dataBuffer_p, uint16_t bytesToRead_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::_read(Register, uint8_t *, uint16_t)", DEBUG_CODE_MPU);

    // Local variables
    bool result = false;

    // Selects device
    if (this->_useSpiBus) {
        result = this->_busHandler->setDevice(this->_csPin);
    } else {
        result = this->_busHandler->setDevice(this->_twiAddress);
    }
    if (!result) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Read data from device
    if (!this->_busHandler->readReg((uint8_t)regAddr_p, dataBuffer_p, bytesToRead_p)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

bool Mpu::_write(Register regAddr_p, uint8_t *dataBuffer_p, uint16_t bytesToWrite_p)
{
    // Mark passage for debugging purpose
    debugMark("Mpu::_write(Register, uint8_t *, uint16_t)", DEBUG_CODE_MPU);

    // Local variables
    bool result = false;

    // Selects device
    if (this->_useSpiBus) {
        result = this->_busHandler->setDevice(this->_csPin);
    } else {
        result = this->_busHandler->setDevice(this->_twiAddress);
    }
    if (!result) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Read data from device
    if (!this->_busHandler->writeReg((uint8_t)regAddr_p, dataBuffer_p, bytesToWrite_p)) {
        // Returns error
        this->_lastError = this->_busHandler->getLastError();
        debugMessage(this->_lastError);
        return false;
    }

    // Returns successfully
    this->_lastError = Error::NONE;
    debugMessage(Error::NONE);
    return true;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

// =============================================================================
// END OF FILE
// =============================================================================
