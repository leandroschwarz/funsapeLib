
enum class FifoEnableFlags {
	NONE								= 0,
	SLAVE_0								= 1,
	SLAVE_1								= 2,
	SLAVE_2								= 4,
	ACCEL								= 8,
	GYRO_Z								= 16,
	GYRO_Y								= 32,
	GYRO_X								= 64,
	TEMP								= 128,
	SLAVE_3								= 256,
	GYRO								= ENABLE_GYRO_Z | ENABLE_GYRO_Y | ENABLE_GYRO_X,
	GYRO_ACCEL							= ENABLE_GYRO | ENABLE_ACCEL,
	GYRO_ACCEL_TEMP						= ENABLE_GYRO | ENABLE_ACCEL | ENABLE_TEMP,
	ALL									= 511,
};

enum class InterruptEnableFlags {
	NONE								= 0,
	RAW_DATA_READY						= 1,
	FSYNC_INT							= 8,
	FIFO_OVERFLOW						= 16,
	WAKE_ON_MOTION						= 64,
	ALL									= 89,
};

Mpu9250(void);
~Mpu9250(void);
bool init(Handler *twiHandler_p, bool useAlternateAddress_p);
bool init(Handler *spiHandler_p, void (*actDevFuncCallback_p)(void), void (*deactDevFuncCallback_p)(void));
bool restart(void);

// -----------------------------------------------------------------------------
// DEVICE CONFIGURATION
// -----------------------------------------------------------------------------

bool configAccel(AccelConfig config_p, AccelFullScale scale_p, Axis axis_p);
bool configGyro(GyroConfig config_p, GyroFullScale scale_p, Axis axis_p);
// bool reset(ResetFlags reset_p);
bool performSelfTest(Axis accelAxis_p, Axis gyroAxis_p);
bool configFifo(FifoEnableFlags enableFlags_p, bool allowOverwrite_p);
bool configWakeOnMotion(uint8_t threshold_p);
bool configInt(InterruptEnableFlags interruptFlags_p);
bool getIntStatus(InterruptEnableFlags *interruptFlags_p);




// bool setIntDataReady(bool enabled_p);

// -----------------------------------------------------------------------------
// RAW DATA MANAGEMENT
// -----------------------------------------------------------------------------

bool getAccelData(int16_t *accelBuff_p, Axis axis_p);
// bool getAccelOffset(int16_t *accelBuff_p, Axis axis_p);
// bool setAccelOffset(int16_t *accelBuff_p, Axis axis_p);
bool getGyroData(int16_t *gyroBuff_p, Axis axis_p);
// bool getGyroOffset(int16_t *gyroBuff_p, Axis axis_p);
// bool setGyroOffset(int16_t *gyroBuff_p, Axis axis_p);
// bool getTempData(int16_t *temp_p);
// void getMagData(int16_t *magBuff);

// =============================================================================
// Class own methods - Private
// =============================================================================

// bool _initialize(void);
// bool _deinitialize(void);
// void _clearData(void);
// bool _read(uint8_t regAddr_p, uint8_t *dataBuffer_p, uint16_t bytesToRead_p);
// bool _write(uint8_t regAddr_p, uint8_t *dataBuffer_p, uint16_t bytesToWrite_p);


bool configI2cMaster(void);
