/**********************************************************************************************//**
 * @file	servo.h
 *
 * @brief	Declares the servo class.
 **************************************************************************************************/

//16Mhz/64 => 4us
//0° = 240 = PULSE of 960us
//90° = 455 = PULSE of 1820us
//180° = 670 = PULSE of 2680us
//OCR1A = POS_MIN + ((POS_MAX-POS_MIN)/(SERVO_MAX_DEG/10) * (deg))/10
//SERVO GROSS min 240 max ?780?
//SERVO KLEIN min 135 max 625

#ifndef SERVO_H_
#define SERVO_H_

#ifndef F_CPU

/**********************************************************************************************//**
 * @def	F_CPU();
 *
 * @brief	A macro that defines CPU frequency.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define F_CPU 16000000L     /* Cristal with 16 MHz */
#endif

/**********************************************************************************************//**
 * @def	PWMS();
 *
 * @brief	A macro that defines PWM-Signal pin.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define PWMS PINB1

/**********************************************************************************************//**
 * @def	POS_MIN();
 *
 * @brief	A macro that defines timer value for position minimum.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define POS_MIN  150//OCR1A Value for 0°

/**********************************************************************************************//**
 * @def	POS_MAX();
 *
 * @brief	A macro that defines timer value for position maximum.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define POS_MAX 575 //OCR1A Value for 180°

/**********************************************************************************************//**
 * @def	SERVO_MAX_DEG();
 *
 * @brief	A macro that defines servo maximum degrees.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define SERVO_MAX_DEG 180

/**********************************************************************************************//**
 * @fn	void servo_timer_init()
 *
 * @brief	Servo timer initialise.
 *          Configures timer1 for fast pwm mode with ICR1 as timer maximum.
 *          The prescaler is set to 64. ICR1 is set to 4999 which results in a 50Hz period (50Hz = 16Mhz/(64*(1+ICR1)) -> ICR1 = 4999).
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

void servo_timer_init();

/**********************************************************************************************//**
 * @fn	void servo_init()
 *
 * @brief	Initialises the Timer for the servo signal and moves servo to POS_MAX.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

void servo_init();

/**********************************************************************************************//**
 * @fn	void servo_toPosition(char deg)
 *
 * @brief	Sets the position of the servo.
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @param	deg	The degrees.
 **************************************************************************************************/

void servo_toPosition(char deg);

/**********************************************************************************************//**
 * @fn	char servo_get_position()
 *
 * @brief	Returns actual servo position.
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @return	Servo position in degrees.
 **************************************************************************************************/

char servo_get_position();

#endif /* SERVO_H_ */