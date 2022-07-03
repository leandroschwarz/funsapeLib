



class Gpio
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

public:
	// NONE

private:
	// NONE

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
	Gpio(
			void
	);
	// TODO: Implement destructor function
	~Gpio(
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
	//     /////////////////     CONTROL AND STATUS     /////////////////     //
#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_GPIO_MODULE_ENABLED)
	// ARM implementation
	bool init(
			GPIO_TypeDef *gpioPort_p,
			uint16_t gpioPin_p
	);
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// AVR implementation
	// TODO: AVR implementation
	bool init(
			uint8_t *gpioPort_p,
			uint16_t gpioPin_p
	);
#else
	// Other implementation
	bool init(
			uint8_t *gpioPort_p,
			uint16_t gpioPin_p
	);
#endif
	Error getLastError(void);

	//     ////////////////////     PIN HANDLING     ////////////////////     //
	bool set(void);
	bool clr(void);
	bool cpl(void);

protected:
// NONE

// -----------------------------------------------------------------------------
// Properties ------------------------------------------------------------------

public:
// NONE

private:
	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	Error _lastError;
	bool _initialize					: 1;

	//     ////////////////    PORT AND PIN HANDLING     ////////////////     //
#if defined(_FUNSAPE_PLATFORM_ARM_STM32) && defined(HAL_GPIO_MODULE_ENABLED)
	// ARM implementation
	GPIO_TypeDef *_gpioPort;
	uint16_t _gpioPin;
#elif defined(_FUNSAPE_PLATFORM_AVR)
	// AVR implementation
	// TODO: AVR implementation
	uint8_t *_gpioPort;
	uint16_t _gpioPin;
#else
	// Other implementation
	uint8_t *_gpioPort;
	uint16_t _gpioPin;
#endif

protected:
// NONE

protected:
// NONE

}; // class Gpio

// =============================================================================
// Gpio - Class overloading operators
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
// Gpio - Class inline function definitions
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

#endif // __FUNSAPE_LIB_GPIO_HPP
