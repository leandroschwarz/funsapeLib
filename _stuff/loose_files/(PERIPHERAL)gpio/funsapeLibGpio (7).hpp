





/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
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
	Gpio(
			GPIO_TypeDef *gpioPort_p,
			cuint16_t gpioPin_p
	);
	~Gpio(
			void
	);

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

public:
	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	Error getLastError(
			void
	);
	bool init(
			GPIO_TypeDef *gpioPort_p,
			cuint16_t gpioPin_p
	);

	//     /////////////////    PIN RELATED METHODS     /////////////////     //
	bool clr(
			void
	);
	bool cpl(
			void
	);
	bool set(
			void
	);

private:
	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	void _clearData(
			void
	);
	void _deinitialize(
			void
	);
	bool _initialize(
			GPIO_TypeDef *gpioPort_p,
			cuint16_t gpioPin_p
	);
	bool _isInitialized(
			void
	);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties --------------------------------------------------------------

public:
	// NONE

private:
	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool			_initialized				: 1;
	Error			_lastError;

	//     /////////////     HAL DATA STRUCT VARIABLES     //////////////     //
	uint16_t		_pin;
	GPIO_TypeDef	*_port;

protected:
	// NONE

}; // class Gpio
