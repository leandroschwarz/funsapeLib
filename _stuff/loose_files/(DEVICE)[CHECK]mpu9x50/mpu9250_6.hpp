
class Mpu9250
{
	// New data types
public:

	// Constructors
public:

	// Methods
public:

protected:
	void updateSampleRateDivider(void);
	void updateConfig(void);
	void updateAccelConfig2(void);
	void updateLowPowerAccelODR(void);
	void updateWakeUpMotionThreshold(void);
	void updateFifoEnable(void);
	void updateI2cMasterControl(void);
	void updateI2cSlave0Address(void);
	void updateI2cSlave0Register(void);
	void updateI2cSlave0Control(void);
	void updateI2cSlave1Address(void);
	void updateI2cSlave1Register(void);
	void updateI2cSlave1Control(void);
	void updateI2cSlave2Address(void);
	void updateI2cSlave2Register(void);
	void updateI2cSlave2Control(void);
	void updateI2cSlave3Address(void);
	void updateI2cSlave3Register(void);
	void updateI2cSlave3Control(void);
	void updateI2cSlave4Address(void);
	void updateI2cSlave4Register(void);
	void updateI2cSlave4Control(void);
	void updateI2cSlave4DataOut(void);
	void readI2cSlave4DataIn(void);
	void readI2cMasterStatus(void);
	void updateIntPinConfig(void);
	void updateIntEnable(void);
	void readExtSensData(uint8_t first, uint8_t total);
	void updateI2cSlave0DataOut(void);
	void updateI2cSlave1DataOut(void);
	void updateI2cSlave2DataOut(void);
	void updateI2cSlave3DataOut(void);
	void updateI2cMasterControlDelay(void);
	void updateSignalPathReset(void);
	void updateMotionDetectControl(void);
	void updateUserControl(void);
	void updatePowerManagementControl2(void);
	void readFifoCount(void);
	void readFifoReadWrite(void);
	void updateFifoReadWrite(void);

// Data members
protected:
	// Register in order
	/*
		uint8_t					sampleRateDivider_;
		bool_t					fifoStopWhenFull_					: 1;
		externalSync_e			externalSync_						: 3;
		accelSamplingConfig_e	accelSamplingConfig_				: 4;
		accelSamplingLowPower_e	accelSamplingLowPower_				: 4;
		uint8_t					accelWakeUpMotionThreshold_;
		bool_t					fifoTempRecordEnabled_				: 1;
		bool_t					fifoGyroXRecordEnabled_				: 1;
		bool_t					fifoGyroYRecordEnabled_				: 1;
		bool_t					fifoGyroZRecordEnabled_				: 1;
		bool_t					fifoAccelXYXRecordEnabled_			: 1;
		bool_t					fifoSlave2RecordEnabled_			: 1;
		bool_t					fifoSlave1RecordEnabled_			: 1;
		bool_t					fifoSlave0RecordEnabled_			: 1;
		bool_t					i2cMultiMasterEnabled_				: 1;
		bool_t					i2cDataReadyIntWaitExternalSensors_	: 1;
		bool_t					fifoSlave3RecordEnabled_			: 1;
		bool_t					i2cSendStopBetweenSlaves_			: 1;
		i2cMasterClock_e		i2cMasterClock_						: 4;
		bool_t					i2cSlave0ReadMode_					: 1;
		uint8_t					i2cSlave0Address_					: 7;
		uint8_t					i2cSlave0Register_;
		bool_t					i2cSlave0Enabled_					: 1;
		bool_t					i2cSlave0WordBytesSwap_				: 1;
		bool_t					i2cSlave0DoNotSendRegister_			: 1;
		bool_t					i2cSlave0GroupEndingEven_			: 1;
		uint8_t					i2cSlave0DataLenght_				: 4;
		bool_t					i2cSlave1ReadMode_					: 1;
		uint8_t					i2cSlave1Address_					: 7;
		uint8_t					i2cSlave1Register_;
		bool_t					i2cSlave1Enabled_					: 1;
		bool_t					i2cSlave1WordBytesSwap_				: 1;
		bool_t					i2cSlave1DoNotSendRegister_			: 1;
		bool_t					i2cSlave1GroupEndingEven_			: 1;
		uint8_t					i2cSlave1DataLenght_				: 4;
		bool_t					i2cSlave2ReadMode_					: 1;
		uint8_t					i2cSlave2Address_					: 7;
		uint8_t					i2cSlave2Register_;
		bool_t					i2cSlave2Enabled_					: 1;
		bool_t					i2cSlave2WordBytesSwap_				: 1;
		bool_t					i2cSlave2DoNotSendRegister_			: 1;
		bool_t					i2cSlave2GroupEndingEven_			: 1;
		uint8_t					i2cSlave2DataLenght_				: 4;
		bool_t					i2cSlave3ReadMode_					: 1;
		uint8_t					i2cSlave3Address_					: 7;
		uint8_t					i2cSlave3Register_;
		bool_t					i2cSlave3Enabled_					: 1;
		bool_t					i2cSlave3WordBytesSwap_				: 1;
		bool_t					i2cSlave3DoNotSendRegister_			: 1;
		bool_t					i2cSlave3GroupEndingEven_			: 1;
		uint8_t					i2cSlave3DataLenght_				: 4;
		bool_t					i2cSlave4ReadMode_					: 1;
		uint8_t					i2cSlave4Address_					: 7;
		uint8_t					i2cSlave4Register_;
		uint8_t					i2cSlave4DataOut_;
		bool_t					i2cSlave4SingleTransferEnable_		: 1;
		bool_t					i2cSlave4IntTransferReadyEnabled_	: 1;
		bool_t					i2cSlave4DoNotSendRegister_			: 1;
		uint8_t					i2cSlave4MasterDelay_				: 5;
		uint8_t					i2cSlave4DataIn_;
		bool_t					i2cStatusPassThroughFlag_			: 1;
		bool_t					i2cStatusSlave4TransferDoneFlag_	: 1;
		bool_t					i2cStatusArbitrationLost_			: 1;
		bool_t					i2cStatusSlave4NackReceivedFlag_	: 1;
		bool_t					i2cStatusSlave3NackReceivedFlag_	: 1;
		bool_t					i2cStatusSlave2NackReceivedFlag_	: 1;
		bool_t					i2cStatusSlave1NackReceivedFlag_	: 1;
		bool_t					i2cStatusSlave0NackReceivedFlag_	: 1;
		logicLevel_t			intPinActivationLevel_				: 1;
		outputDriver_t			intPinOutputDriver_					: 1;
		bool_t					intPinLatched_						: 1;
		bool_t					intPinClearWhenRead_				: 1;
		logicLevel_t			fsyncPinActivationLevel_			: 1;
		bool_t					fsyncPinTriggerInterrupt_			: 1;
		bool_t					i2cBypassModeEnabled_				: 1;
		bool_t					intWakeOnMotionEnabled_				: 1;
		bool_t					intFifoOverflowEnabled_				: 1;
		bool_t					intFsyncEnabled_					: 1;
		bool_t					intI2cDataReadyEnabled_				: 1;
		uint8_t					extSensData_[24];
		uint8_t					i2cSlave0DataOut_;
		uint8_t					i2cSlave1DataOut_;
		uint8_t					i2cSlave2DataOut_;
		uint8_t					i2cSlave3DataOut_;
		bool_t					i2cExtDataShadowingEnabled_			: 1;
		bool_t					i2cSlave4DelayEnabled_				: 1;
		bool_t					i2cSlave3DelayEnabled_				: 1;
		bool_t					i2cSlave2DelayEnabled_				: 1;
		bool_t					i2cSlave1DelayEnabled_				: 1;
		bool_t					i2cSlave0DelayEnabled_				: 1;
		bool_t					gyroSignalPathReset_				: 1;
		bool_t					accelSignalPathReset_				: 1;
		bool_t					tempSignalPathReset_				: 1;
	//	MOT_DETECT_CTRL			ACCEL_INTEL_EN
	//	MOT_DETECT_CTRL			ACCEL_INTEL_MODE
		bool_t					fifoEnabled_						: 1;
		bool_t					i2cMasterEnabled_					: 1;
		bool_t					useSpiInterface_					: 1;
	//	USER_CTRL				FIFO_RST
	//	USER_CTRL				I2C_MST_RST
	//	USER_CTRL				SIG_COND_RST
		bool_t					accelXDisable_						: 1;
		bool_t					accelYDisable_						: 1;
		bool_t					accelZDisable_						: 1;
		bool_t					gyroXDisable_						: 1;
		bool_t					gyroYDisable_						: 1;
		bool_t					gyroZDisable_						: 1;
		uint16_t				fifoCount_							: 13;
		uint8_t					fifoData_;		// FIFO_R_W
	*/





	/*
	protected:

	// Set initial input parameters
	enum Ascale
	    {
	      AFS_2G = 0,
	      AFS_4G,
	      AFS_8G,
	      AFS_16G
	    };

	    enum Gscale {
	      GFS_250DPS = 0,
	      GFS_500DPS,
	      GFS_1000DPS,
	      GFS_2000DPS
	    };

	    enum Mscale {
	      MFS_14BITS = 0, // 0.6 mG per LSB
	      MFS_16BITS      // 0.15 mG per LSB
	    };

	    enum M_MODE {
	      M_8HZ = 0x02,  // 8 Hz update
	      M_100HZ = 0x06 // 100 Hz continuous magnetometer
	    };

	    // TODO: Add setter methods for this hard coded stuff
	    // Specify sensor full scale
	    uint8_t Gscale = GFS_250DPS;
	    uint8_t Ascale = AFS_2G;
	    // Choose either 14-bit or 16-bit magnetometer resolution
	    uint8_t Mscale = MFS_16BITS;

	    // 2 for 8 Hz, 6 for 100 Hz continuous magnetometer data read
	    uint8_t Mmode = M_8HZ;

	    // SPI chip select pin
	    int8_t _csPin;

	    uint8_t writeByteWire(uint8_t, uint8_t, uint8_t);
	    uint8_t writeByteSPI(uint8_t, uint8_t);
	    uint8_t readByteSPI(uint8_t subAddress);
	    uint8_t readByteWire(uint8_t address, uint8_t subAddress);
	    bool magInit();
	    void kickHardware();
	    void select();
	    void deselect();
	// TODO: Remove this next line
	public:
	    uint8_t ak8963WhoAmI_SPI();

	  public:
	    float pitch, yaw, roll;
	    float temperature;   // Stores the real internal chip temperature in Celsius
	    int16_t tempCount;   // Temperature raw count output
	    uint32_t delt_t = 0; // Used to control display output rate

	    uint32_t count = 0, sumCount = 0; // used to control display output rate
	    float deltat = 0.0f, sum = 0.0f;  // integration interval for both filter schemes
	    uint32_t lastUpdate = 0, firstUpdate = 0; // used to calculate integration interval
	    uint32_t Now = 0;        // used to calculate integration interval

	    int16_t gyroCount[3];   // Stores the 16-bit signed gyro sensor output
	    int16_t magCount[3];    // Stores the 16-bit signed magnetometer sensor output
	    // Scale resolutions per LSB for the sensors
	    float aRes, gRes, mRes;
	    // Variables to hold latest sensor data values
	    float ax, ay, az, gx, gy, gz, mx, my, mz;
	    // Factory mag calibration and mag bias
	    float factoryMagCalibration[3] = {0, 0, 0}, factoryMagBias[3] = {0, 0, 0};
	    // Bias corrections for gyro, accelerometer, and magnetometer
	    float gyroBias[3]  = {0, 0, 0},
	          accelBias[3] = {0, 0, 0},
	          magBias[3]   = {0, 0, 0},
	          magScale[3]  = {0, 0, 0};
	    float selfTest[6];
	    // Stores the 16-bit signed accelerometer sensor output
	    int16_t accelCount[3];

	    // Public method declarations
	   void getMres();
	    void getGres();
	    void getAres();
	    void readAccelData(int16_t *);
	    void readGyroData(int16_t *);
	    void readMagData(int16_t *);
	    int16_t readTempData();
	    void updateTime();
	    void initAK8963(float *);
	    void initMPU9250();
	    void calibrateMPU9250(float * gyroBias, float * accelBias);
	    void MPU9250SelfTest(float * destination);
	    void magCalMPU9250(float * dest1, float * dest2);
	    uint8_t writeByte(uint8_t, uint8_t, uint8_t);
	    uint8_t readByte(uint8_t, uint8_t);
	    uint8_t readBytes(uint8_t, uint8_t, uint8_t, uint8_t *);
	    // TODO: make SPI/Wire private
	    uint8_t readBytesSPI(uint8_t, uint8_t, uint8_t *);
	    uint8_t readBytesWire(uint8_t, uint8_t, uint8_t, uint8_t *);
	    bool isInI2cMode() { return _csPin == -1; }
	    bool begin();
	*/
};  // class Mpu9250

// -----------------------------------------------------------------------------
// Mpu9250 class - Inline functions definitions --------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// General inline functions declarations ---------------------------------------

/* NONE */




/*
#define SERIAL_DEBUG true

// See also MPU-9250 Register Map and Descriptions, Revision 4.0,
// RM-MPU-9250A-00, Rev. 1.4, 9/9/2013 for registers not listed in above
// document; the MPU9250 and MPU9150 are virtually identical but the latter has
// a different register map

//Magnetometer Registers
#define AK8963_ADDRESS   0x0C
#define WHO_AM_I_AK8963  0x49 // (AKA WIA) should return 0x48
#define INFO             0x01
#define AK8963_ST1       0x02  // data ready status bit 0
#define AK8963_XOUT_L    0x03  // data
#define AK8963_XOUT_H    0x04
#define AK8963_YOUT_L    0x05
#define AK8963_YOUT_H    0x06
#define AK8963_ZOUT_L    0x07
#define AK8963_ZOUT_H    0x08
#define AK8963_ST2       0x09  // Data overflow bit 3 and data read error status bit 2
#define AK8963_CNTL      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_ASTC      0x0C  // Self test control
#define AK8963_I2CDIS    0x0F  // I2C disable
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value

#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02

// #define X_FINE_GAIN      0x03 // [7:0] fine gain
// #define Y_FINE_GAIN      0x04
// #define Z_FINE_GAIN      0x05
// #define XA_OFFSET_H      0x06 // User-defined trim values for accelerometer
// #define XA_OFFSET_L_TC   0x07
// #define YA_OFFSET_H      0x08
// #define YA_OFFSET_L_TC   0x09
// #define ZA_OFFSET_H      0x0A
// #define ZA_OFFSET_L_TC   0x0B

#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F

#define SELF_TEST_A       0x10

#define XG_OFFSET_H       0x13  // User-defined trim values for gyroscope
#define XG_OFFSET_L       0x14
#define YG_OFFSET_H       0x15
#define YG_OFFSET_L       0x16
#define ZG_OFFSET_H       0x17
#define ZG_OFFSET_L       0x18
#define SMPLRT_DIV        0x19
#define CONFIG            0x1A
#define GYRO_CONFIG       0x1B
#define ACCEL_CONFIG      0x1C
#define ACCEL_CONFIG2     0x1D
#define LP_ACCEL_ODR      0x1E
#define WOM_THR           0x1F

// Duration counter threshold for motion interrupt generation, 1 kHz rate,
// LSB = 1 ms
#define MOT_DUR           0x20
// Zero-motion detection threshold bits [7:0]
#define ZMOT_THR          0x21
// Duration counter threshold for zero motion interrupt generation, 16 Hz rate,
// LSB = 64 ms
#define ZRMOT_DUR         0x22

#define FIFO_EN            0x23
#define I2C_MST_CTRL       0x24
#define I2C_SLV0_ADDR      0x25
#define I2C_SLV0_REG       0x26
#define I2C_SLV0_CTRL      0x27
#define I2C_SLV1_ADDR      0x28
#define I2C_SLV1_REG       0x29
#define I2C_SLV1_CTRL      0x2A
#define I2C_SLV2_ADDR      0x2B
#define I2C_SLV2_REG       0x2C
#define I2C_SLV2_CTRL      0x2D
#define I2C_SLV3_ADDR      0x2E
#define I2C_SLV3_REG       0x2F
#define I2C_SLV3_CTRL      0x30
#define I2C_SLV4_ADDR      0x31
#define I2C_SLV4_REG       0x32
#define I2C_SLV4_DO        0x33
#define I2C_SLV4_CTRL      0x34
#define I2C_SLV4_DI        0x35
#define I2C_MST_STATUS     0x36
#define INT_PIN_CFG        0x37
#define INT_ENABLE         0x38
#define DMP_INT_STATUS     0x39  // Check DMP interrupt
#define INT_STATUS         0x3A
#define ACCEL_XOUT_H       0x3B
#define ACCEL_XOUT_L       0x3C
#define ACCEL_YOUT_H       0x3D
#define ACCEL_YOUT_L       0x3E
#define ACCEL_ZOUT_H       0x3F
#define ACCEL_ZOUT_L       0x40
#define TEMP_OUT_H         0x41
#define TEMP_OUT_L         0x42
#define GYRO_XOUT_H        0x43
#define GYRO_XOUT_L        0x44
#define GYRO_YOUT_H        0x45
#define GYRO_YOUT_L        0x46
#define GYRO_ZOUT_H        0x47
#define GYRO_ZOUT_L        0x48
#define EXT_SENS_DATA_00   0x49
#define EXT_SENS_DATA_01   0x4A
#define EXT_SENS_DATA_02   0x4B
#define EXT_SENS_DATA_03   0x4C
#define EXT_SENS_DATA_04   0x4D
#define EXT_SENS_DATA_05   0x4E
#define EXT_SENS_DATA_06   0x4F
#define EXT_SENS_DATA_07   0x50
#define EXT_SENS_DATA_08   0x51
#define EXT_SENS_DATA_09   0x52
#define EXT_SENS_DATA_10   0x53
#define EXT_SENS_DATA_11   0x54
#define EXT_SENS_DATA_12   0x55
#define EXT_SENS_DATA_13   0x56
#define EXT_SENS_DATA_14   0x57
#define EXT_SENS_DATA_15   0x58
#define EXT_SENS_DATA_16   0x59
#define EXT_SENS_DATA_17   0x5A
#define EXT_SENS_DATA_18   0x5B
#define EXT_SENS_DATA_19   0x5C
#define EXT_SENS_DATA_20   0x5D
#define EXT_SENS_DATA_21   0x5E
#define EXT_SENS_DATA_22   0x5F
#define EXT_SENS_DATA_23   0x60
#define MOT_DETECT_STATUS  0x61
#define I2C_SLV0_DO        0x63
#define I2C_SLV1_DO        0x64
#define I2C_SLV2_DO        0x65
#define I2C_SLV3_DO        0x66
#define I2C_MST_DELAY_CTRL 0x67
#define SIGNAL_PATH_RESET  0x68
#define MOT_DETECT_CTRL    0x69
#define USER_CTRL          0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define PWR_MGMT_1         0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2         0x6C
#define DMP_BANK           0x6D  // Activates a specific bank in the DMP
#define DMP_RW_PNT         0x6E  // Set read/write pointer to a specific start address in specified DMP bank
#define DMP_REG            0x6F  // Register in DMP from which to read or to which to write
#define DMP_REG_1          0x70
#define DMP_REG_2          0x71
#define FIFO_COUNTH        0x72
#define FIFO_COUNTL        0x73
#define FIFO_R_W           0x74
#define WHO_AM_I_MPU9250   0x75 // Should return 0x71
#define XA_OFFSET_H        0x77
#define XA_OFFSET_L        0x78
#define YA_OFFSET_H        0x7A
#define YA_OFFSET_L        0x7B
#define ZA_OFFSET_H        0x7D
#define ZA_OFFSET_L        0x7E

// Using the MPU-9250 breakout board, ADO is set to 0
// Seven-bit device address is 110100 for ADO = 0 and 110101 for ADO = 1
#define ADO 0
#if ADO
#define MPU9250_ADDRESS 0x69  // Device address when ADO = 1
#else
#define MPU9250_ADDRESS 0x68  // Device address when ADO = 0
#define AK8963_ADDRESS  0x0C   // Address of magnetometer
#endif // AD0

#define READ_FLAG 0x80
#define NOT_SPI -1
#define SPI_DATA_RATE 1000000 // 1MHz is the max speed of the MPU-9250
//#define SPI_DATA_RATE 1000000 // 1MHz is the max speed of the MPU-9250
#define SPI_MODE SPI_MODE3

class MPU9250
{
  protected:
    // Set initial input parameters
    enum Ascale
    {
      AFS_2G = 0,
      AFS_4G,
      AFS_8G,
      AFS_16G
    };

    enum Gscale {
      GFS_250DPS = 0,
      GFS_500DPS,
      GFS_1000DPS,
      GFS_2000DPS
    };

    enum Mscale {
      MFS_14BITS = 0, // 0.6 mG per LSB
      MFS_16BITS      // 0.15 mG per LSB
    };

    enum M_MODE {
      M_8HZ = 0x02,  // 8 Hz update
      M_100HZ = 0x06 // 100 Hz continuous magnetometer
    };

    // TODO: Add setter methods for this hard coded stuff
    // Specify sensor full scale
    uint8_t Gscale = GFS_250DPS;
    uint8_t Ascale = AFS_2G;
    // Choose either 14-bit or 16-bit magnetometer resolution
    uint8_t Mscale = MFS_16BITS;

    // 2 for 8 Hz, 6 for 100 Hz continuous magnetometer data read
    uint8_t Mmode = M_8HZ;

    // SPI chip select pin
    int8_t _csPin;

    uint8_t writeByteWire(uint8_t, uint8_t, uint8_t);
    uint8_t writeByteSPI(uint8_t, uint8_t);
    uint8_t readByteSPI(uint8_t subAddress);
    uint8_t readByteWire(uint8_t address, uint8_t subAddress);
    bool magInit();
    void kickHardware();
    void select();
    void deselect();
// TODO: Remove this next line
public:
    uint8_t ak8963WhoAmI_SPI();

  public:
    float pitch, yaw, roll;
    float temperature;   // Stores the real internal chip temperature in Celsius
    int16_t tempCount;   // Temperature raw count output
    uint32_t delt_t = 0; // Used to control display output rate

    uint32_t count = 0, sumCount = 0; // used to control display output rate
    float deltat = 0.0f, sum = 0.0f;  // integration interval for both filter schemes
    uint32_t lastUpdate = 0, firstUpdate = 0; // used to calculate integration interval
    uint32_t Now = 0;        // used to calculate integration interval

    int16_t gyroCount[3];   // Stores the 16-bit signed gyro sensor output
    int16_t magCount[3];    // Stores the 16-bit signed magnetometer sensor output
    // Scale resolutions per LSB for the sensors
    float aRes, gRes, mRes;
    // Variables to hold latest sensor data values
    float ax, ay, az, gx, gy, gz, mx, my, mz;
    // Factory mag calibration and mag bias
    float factoryMagCalibration[3] = {0, 0, 0}, factoryMagBias[3] = {0, 0, 0};
    // Bias corrections for gyro, accelerometer, and magnetometer
    float gyroBias[3]  = {0, 0, 0},
          accelBias[3] = {0, 0, 0},
          magBias[3]   = {0, 0, 0},
          magScale[3]  = {0, 0, 0};
    float selfTest[6];
    // Stores the 16-bit signed accelerometer sensor output
    int16_t accelCount[3];

    // Public method declarations
    MPU9250(int8_t csPin=NOT_SPI);
    void getMres();
    void getGres();
    void getAres();
    void readAccelData(int16_t *);
    void readGyroData(int16_t *);
    void readMagData(int16_t *);
    int16_t readTempData();
    void updateTime();
    void initAK8963(float *);
    void initMPU9250();
    void calibrateMPU9250(float * gyroBias, float * accelBias);
    void MPU9250SelfTest(float * destination);
    void magCalMPU9250(float * dest1, float * dest2);
    uint8_t writeByte(uint8_t, uint8_t, uint8_t);
    uint8_t readByte(uint8_t, uint8_t);
    uint8_t readBytes(uint8_t, uint8_t, uint8_t, uint8_t *);
    // TODO: make SPI/Wire private
    uint8_t readBytesSPI(uint8_t, uint8_t, uint8_t *);
    uint8_t readBytesWire(uint8_t, uint8_t, uint8_t, uint8_t *);
    bool isInI2cMode() { return _csPin == -1; }
    bool begin();
};  // class MPU9250

*/

#endif // __MPU9250_HPP

