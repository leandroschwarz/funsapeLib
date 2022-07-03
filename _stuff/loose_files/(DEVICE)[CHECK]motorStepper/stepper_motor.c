/* -----------------------------------------------------------------------------
 * File:			stepper_motor.c
 * Module:			Stepper motor interface
 * Authors:			Fábio Cabral Pacheco
 * 					Leandro Schwarz
 * Version:			1.0
 * Last edition:	2015-03-10
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "stepper_motor.h"
#if __STEPPER_MOTOR_H != 10
	#error Error 101 - Version mismatch on header and source code files (stepper_motor).
#endif

/* -----------------------------------------------------------------------------
 * Resets the motor configuration.
 * -------------------------------------------------------------------------- */

stepperResult_t stepperResetConfiguration(volatile stepperConfiguration_t * motor)
{
	motor->stepperDDR = NULL;
	motor->stepperPORT = NULL;
	motor->stepperLsb = PB0;
	motor->position = 0;
	motor->mode = STEPPER_WAVE;
	motor->direction = STEPPER_CLOCKWISE;
	motor->portConfigured = FALSE;
	motor->directionConfigured = FALSE;
	motor->modeConfigured = FALSE;
	motor->initialized = FALSE;

	return STEPPER_OK;
}

/* -----------------------------------------------------------------------------
 * Configures the port I/O.
 * -------------------------------------------------------------------------- */

stepperResult_t stepperSetPort(volatile stepperConfiguration_t * motor, volatile uint8 * ddr, volatile uint8 * port, uint8 lsb)
{
	motor->stepperDDR = ddr;
	motor->stepperPORT = port;
	motor->stepperLsb = lsb;
	motor->portConfigured = TRUE;

	return STEPPER_OK;
}

/* -----------------------------------------------------------------------------
 * Configures the movement direction.
 * -------------------------------------------------------------------------- */

stepperResult_t stepperSetDirection(volatile stepperConfiguration_t * motor, stepperDirection_t direction)
{
	motor->direction = direction;
	motor->directionConfigured = TRUE;

	return STEPPER_OK;
}

/* -----------------------------------------------------------------------------
 * Gets the movement direction.
 * -------------------------------------------------------------------------- */

stepperDirection_t stepperGetDirection(volatile stepperConfiguration_t * motor)
{
	return motor->direction;
}

/* -----------------------------------------------------------------------------
 * Configures the movement mode.
 * -------------------------------------------------------------------------- */

stepperResult_t stepperSetMode(volatile stepperConfiguration_t * motor, stepperMode_t mode)
{
	motor->mode = mode;
	motor->modeConfigured = TRUE;

	return STEPPER_OK;
}

/* -----------------------------------------------------------------------------
 * Gets the movement mode.
 * -------------------------------------------------------------------------- */

stepperMode_t stepperGetMode(volatile stepperConfiguration_t * motor)
{
	return motor->mode;
}

/* -----------------------------------------------------------------------------
 * Performs the stepper initialization.
 * -------------------------------------------------------------------------- */

stepperResult_t stepperInit(volatile stepperConfiguration_t * motor)
{
	if(motor->portConfigured == FALSE)
		return STEPPER_PORTS_NOT_CONFIGURED;

	*(motor->stepperDDR) |= (0x0F << motor->stepperLsb);
	*(motor->stepperPORT) &= ~(0x0F << motor->stepperLsb);
	motor->initialized = TRUE;

	return STEPPER_OK;
}

/* -----------------------------------------------------------------------------
 * Moves the motor.
 * -------------------------------------------------------------------------- */

stepperResult_t stepperMove(volatile stepperConfiguration_t * motor)
{
	uint8 aux = 0;

	if(motor->initialized == FALSE)
		return STEPPER_NOT_INITIALIZED;

	if(motor->portConfigured == FALSE)
		return STEPPER_PORTS_NOT_CONFIGURED;

	if(motor->directionConfigured == FALSE)
		return STEPPER_DIRECTION_NOT_CONFIGURED;

	if(motor->modeConfigured == FALSE)
		return STEPPER_MODE_NOT_CONFIGURED;

	// Calculate new motor position
	if(motor->direction == STEPPER_CLOCKWISE)
		motor->position = (motor->position == 7)?0:(motor->position + 1);
	else
		motor->position = (motor->position == 0)?7:(motor->position - 1);
		// Move motor to new position
	aux = *(motor->stepperPORT) & ~(0x0F << motor->stepperLsb);
	if(motor->mode == STEPPER_FULL_STEP){
		switch(motor->position){
			case 0: aux |= (0b00001001 << motor->stepperLsb); break;
			case 1: aux |= (0b00001100 << motor->stepperLsb); break;
			case 2: aux |= (0b00001100 << motor->stepperLsb); break;
			case 3: aux |= (0b00000110 << motor->stepperLsb); break;
			case 4: aux |= (0b00000110 << motor->stepperLsb); break;
			case 5: aux |= (0b00000011 << motor->stepperLsb); break;
			case 6: aux |= (0b00000011 << motor->stepperLsb); break;
			case 7: aux |= (0b00001001 << motor->stepperLsb); break;
		}
	}else if(motor->mode == STEPPER_HALF_STEP){
		switch(motor->position){
			case 0: aux |= (0b00001001 << motor->stepperLsb); break;
			case 1: aux |= (0b00001000 << motor->stepperLsb); break;
			case 2: aux |= (0b00001100 << motor->stepperLsb); break;
			case 3: aux |= (0b00000100 << motor->stepperLsb); break;
			case 4: aux |= (0b00000110 << motor->stepperLsb); break;
			case 5: aux |= (0b00000010 << motor->stepperLsb); break;
			case 6: aux |= (0b00000011 << motor->stepperLsb); break;
			case 7: aux |= (0b00000001 << motor->stepperLsb); break;
		}
	}else if(motor->mode == STEPPER_WAVE){
		switch(motor->position){
			case 0: aux |= (0b00001000 << motor->stepperLsb); break;
			case 1: aux |= (0b00001000 << motor->stepperLsb); break;
			case 2: aux |= (0b00000100 << motor->stepperLsb); break;
			case 3: aux |= (0b00000100 << motor->stepperLsb); break;
			case 4: aux |= (0b00000010 << motor->stepperLsb); break;
			case 5: aux |= (0b00000010 << motor->stepperLsb); break;
			case 6: aux |= (0b00000001 << motor->stepperLsb); break;
			case 7: aux |= (0b00000001 << motor->stepperLsb); break;
		}
	}
	*(motor->stepperPORT) = aux;

	return STEPPER_OK;
}
