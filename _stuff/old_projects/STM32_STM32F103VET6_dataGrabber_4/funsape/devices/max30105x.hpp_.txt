
#define MAX30105_ADDRESS				0x57	// 7-bit I2C Address

#define TWI_SPEED_STANDARD				100000
#define TWI_SPEED_FAST					400000
#define TWI_BUFFER_LENGTH				32		// Size of the I2C buffer

class MAX30105
{
public:
	uint32_t getRed(void);				// Returns immediate red value
	uint32_t getIR(void);				// Returns immediate IR value
	uint32_t getGreen(void);			// Returns immediate green value
	bool safeCheck(
			uint8_t maxTimeToCheck
	);									// Given a max amount of time, check for new data

	// Configuration
	void setPulseAmplitudeGreen(uint8_t value);
	void setPulseAmplitudeProximity(uint8_t value);
	void setProximityThreshold(uint8_t threshMSB);

	// Multi-led configuration mode (page 22)
	void enableSlot(
			uint8_t slotNumber,
			uint8_t device
	);									// Given slot number, assign a device to slot
	void disableSlots(void);

	// FIFO Reading
	uint16_t check(void);				// Checks for new data and fills FIFO
	uint8_t available(void);			// Tells caller how many new samples are available (head - tail)
	void nextSample(void);				// Advances the tail of the sense array
	uint32_t getFIFORed(void);			// Returns the FIFO sample pointed to by tail
	uint32_t getFIFOIR(void);			// Returns the FIFO sample pointed to by tail
	uint32_t getFIFOGreen(void);		// Returns the FIFO sample pointed to by tail

	// Proximity Mode Interrupt Threshold
	void setPROXINTTHRESH(uint8_t val);

	// Setup the IC with user selectable settings
	void setup(
			uint8_t powerLevel = 0x1F,
			uint8_t sampleAverage = 4,
			uint8_t ledMode = 3,
			int sampleRate = 400,
			int pulseWidth = 411,
			int adcRange = 4096
	);

private:
	TwoWire *_i2cPort;					// The generic connection to user's chosen I2C hardware
	uint8_t _i2caddr;

	// activeLEDs is the number of channels turned on, and can be 1 to 3. 2 is common for Red+IR.
	uint8_t activeLEDs;					// Gets set during setup. Allows check() to calculate how many bytes to read from FIFO
	uint8_t revisionID;
	void bitMask(uint8_t reg, uint8_t mask, uint8_t thing);
#define STORAGE_SIZE					4		// Each long is 4 bytes so limit this to fit on your micro
	typedef struct Record {
		uint32_t red[STORAGE_SIZE];
		uint32_t IR[STORAGE_SIZE];
		uint32_t green[STORAGE_SIZE];
		uint8_t head;
		uint8_t tail;
	} sense_struct;						//This is our circular buffer of readings from the sensor
	sense_struct sense;
};
