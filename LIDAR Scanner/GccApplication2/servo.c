/**********************************************************************************************//**
 * @file	servo.c
 *
 * @brief	servo class.
 **************************************************************************************************/

#include "servo.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <assert.h>
/** @brief	Position in degrees (0°-180°) */
char servo_position  = 0;

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

void servo_timer_init(){
	
	DDRB |= (1<<PWMS);
	PORTB = (1<<PWMS);

	TCCR1A = (1<<COM1A1) | (1<<WGM11);
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11) | (1<<CS10) ; // FAST PWM , TOP = ICR1, Prescaler 64 -> 50Hz = 16Mhz/(64*(1+ICR1)) -> ICR1 = 4999

	ICR1 = 4999;	
}

/**********************************************************************************************//**
 * @fn	void servo_init()
 *
 * @brief	Initialises the Timer for the servo signal and moves servo to POS_MAX.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

void servo_init(){
	servo_timer_init();
	OCR1A = POS_MAX;
	
}

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

void servo_toPosition(char deg){
	assert(deg <= 90);
	char s = SERVO_MAX_DEG-deg;
	if (s>=0 && s<=SERVO_MAX_DEG)
	{
			OCR1A = POS_MIN + ((POS_MAX-POS_MIN)/(SERVO_MAX_DEG/10) * (s))/10;
			servo_position = deg;
	}
}

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

char servo_get_position(){
	return servo_position;
}