/**********************************************************************************************//**
 * @file	motor.c
 *
 * @brief	motor class.
 **************************************************************************************************/

#include "motor.h"
#include <avr/io.h>
#include "adc.h"
#include <util/delay.h>
#include <assert.h>
#include <avr/wdt.h>

/** @brief	Last done step (0-3). */
char step = 0;
/** @brief	The motor position. */
char motor_position = 0;
/** @brief	The time between steps. */
char speed = T_STEP;

/**********************************************************************************************//**
 * @fn	void _sleep_ms(int ms)
 *
 * @brief	Delay for ms milliseconds. The _delay_ms() function does not allow the use of variables. Thats what this function is used for.
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @param	ms	The milliseconds.
 **************************************************************************************************/

void _sleep_ms(int ms){
	
	for (int i = 0;i<ms;i++)
	{
		_delay_ms(1);
	}
	
}

/**********************************************************************************************//**
 * @fn	void motor_timer_init(void)
 *
 * @brief	Initialises the timer0 for the power control. 
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

void motor_timer_init(void){
	TCCR0A |= (1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);
	TCCR0B |= (1<<CS01);
	
	OCR0A = (255/100) * POWER;
	OCR0B = (255/100) * POWER;
}

/**********************************************************************************************//**
 * @fn	void motor_init(void)
 *
 * @brief	Initialises all used pins as output and calls motor_timer_init()
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

void motor_init(void){
	DDRD |= (1<<PWMA) | (1<<PWMB) | (1<<MOTA1) | (1<<MOTA2)  | (1<<MOTB2) | (1<<STBY);
	DDRB |= (1<<MOTB1);
	PORTD |= (1<<PWMA) | (1<<PWMB) | (1<<STBY);


	motor_timer_init();

}

/**********************************************************************************************//**
 * @fn	void motor_turn(char steps,char dir)
 *
 * @brief	Turn Stepper for some steps in the desired direction.
 *          Based on the last step, this function sets the pins to rotate the stepper by one step.
 * 
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @param	steps	Number of steps.
 * @param	dir  	Direction (CW or CCW).
 **************************************************************************************************/

void motor_turn(char steps,char dir){
	for (char s = 0; s<steps ; s++)
	{
		step = (step + 1) % 4;
		if (dir == CW)
		{
			motor_position = (motor_position+1) % MOTOR_MAX_STEPS;
			switch (step)
			{
				case 0:
				PORTD |= (1<<MOTA1);
				PORTB |= (1<<MOTB1);
				PORTD &=  ~((1<<MOTA2) | (1<<MOTB2));
				break;
				case 1:
				PORTD |= (1<<MOTA1) | (1<<MOTB2);
				PORTD &=  ~(1<<MOTA2);
				PORTB &= ~(1<<MOTB1);
				break;
				case 2:
				PORTD |= (1<<MOTA2)  | (1<<MOTB2);
				PORTD &=  ~(1<<MOTA1);
				PORTB &= ~(1<<MOTB1);
				break;
				case 3:
				PORTD |=  (1<<MOTA2);
				PORTB |= (1<<MOTB1);
				PORTD &=  ~((1<<MOTA1) | (1<<MOTB2));
				break;
			}
		}
		else
		{
			motor_position = (motor_position-1) % MOTOR_MAX_STEPS;
			switch (step)
			{
				case 0:
				PORTD |= (1<<MOTA2);
				PORTB |= (1<<MOTB1);
				PORTD &=  ~((1<<MOTA1) | (1<<MOTB2));
				break;
				case 1:
				PORTD |= (1<<MOTA2) | (1<<MOTB2);
				PORTD &=  ~(1<<MOTA1);
				PORTB &= ~(1<<MOTB1);
				break;
				case 2:
				PORTD |= (1<<MOTA1)  | (1<<MOTB2);
				PORTD &=  ~(1<<MOTA2);
				PORTB &= ~(1<<MOTB1);
				break;
				case 3:
				PORTD |=  (1<<MOTA1);
				PORTB |= (1<<MOTB1);
				PORTD &=  ~((1<<MOTA2) | (1<<MOTB2));
				break;
			}
		}
		
		_sleep_ms(speed);
		
	}
	
}

/**********************************************************************************************//**
 * @fn	void motor_toPosition(char p)
 *
 * @brief	Turns motor to the defined position.
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @param	p	The char to process.
 **************************************************************************************************/

void motor_toPosition(char p){
	p = p % MOTOR_MAX_STEPS;
	if (motor_position > p )
	{
		motor_turn(motor_position-p,CCW);
	}
	else if (motor_position < p)
	{
		motor_turn(p-motor_position,CW);
	}
	else
	{
		return;
	}
}

/**********************************************************************************************//**
 * @fn	void motor_set_speed(char t)
 *
 * @brief	Change time between steps.
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @param	t	The char to process.
 **************************************************************************************************/

void motor_set_speed(char t){
	assert(t<=255);
	
	speed = t;
}

/**********************************************************************************************//**
 * @fn	void motor_set_power(char power)
 *
 * @brief	Change the maximum power output.
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @param	power	The power.
 **************************************************************************************************/

void motor_set_power(char power){
	assert(power >= 0 && power <=100);
	
	OCR0A = (255/100) * power;
	OCR0B = (255/100) * power;
}

/**********************************************************************************************//**
 * @fn	void motor_set_position(char pos)
 *
 * @brief	Set the position of the stepper (only in software).
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @param	pos	The position.
 **************************************************************************************************/

void motor_set_position(char pos){
	motor_position = pos;
}

/**********************************************************************************************//**
 * @fn	char motor_get_position(void)
 *
 * @brief	Returns the actual motor position.
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @return	Motor position in steps.
 **************************************************************************************************/

char motor_get_position(void){
	return motor_position;
}

/**********************************************************************************************//**
 * @fn	void motor_calibrate()
 *
 * @brief	Calibrates motor position.
 *          This function reads the value of the adc pin of the light barrier. 
 *          The motor is in the right position if the value is lower than 40.
 *          If it is higher, it rotates the stepper in CW direction for 100 steps (180°). 
 *          Then it returns to zero and rotates in CCW direction for 100 steps (180°) till the value is below 40.
 *          
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

void motor_calibrate(){
	uint16_t a;
	char p = 0;
	char top = 60;
	int8_t d = CW;
	while (1)
	{
		wdt_reset();
		
		a = adc_read_avg(5);
		//a= adc_read_avg(3);
		
		//serial_write_int(a); //debug infos
		//serial_write_string("\r\n");
		if (a<=top)
		{
			
			//motor_turn(2,d);
			motor_set_position(MOTOR_MAX_STEPS/4);
			return;
		}
		
		if(d==CW)
		{
			motor_turn(1,CW);
			p++;
		}
		else
		{
			motor_turn(1,CCW);
			p++;
		}
		if (p == 100)
		{
			d = d * (-1);
			motor_turn(100,d);
			p = 0;
			top+=5;
		}
		
	}
}