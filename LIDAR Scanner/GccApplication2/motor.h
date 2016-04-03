/**********************************************************************************************//**
 * @file	motor.h
 *
 * @brief	Declares the motor class.
 **************************************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_

#ifndef F_CPU

/**********************************************************************************************//**
 * @def	F_CPU();
 *
 * @brief	A macro that defines CPU.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define F_CPU 16000000L     /* Cristal with 16 MHz */
#endif

/**********************************************************************************************//**
 * @def	PWMA();
 *
 * @brief	A macro that defines the PWM-Pin A.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define PWMA	PIND5 //Power for Coil A

/**********************************************************************************************//**
 * @def	PWMB();
 *
 * @brief	A macro that defines the PWM-Pin B.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define PWMB	PIND6 //Power for Coil B

/**********************************************************************************************//**
 * @def	MOTA1();
 *
 * @brief	A macro that defines motor-pin A1.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define MOTA1	PIND3 //Signal Coil A 1

/**********************************************************************************************//**
 * @def	MOTA2();
 *
 * @brief	A macro that defines motor-pin A2.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define MOTA2	PIND2 //Signal Coil A 2

/**********************************************************************************************//**
 * @def	MOTB1();
 *
 * @brief	A macro that defines motor-pin B1.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define MOTB1	PINB0 //Signal Coil B 1

/**********************************************************************************************//**
 * @def	MOTB2();
 *
 * @brief	A macro that defines motor-pin B2.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define MOTB2	PIND7 //Signal Coil B 2

/**********************************************************************************************//**
 * @def	STBY();
 *
 * @brief	A macro that defines standby-pin.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define STBY	PIND4 //Standby Pin

/**********************************************************************************************//**
 * @def	T_STEP();
 *
 * @brief	A macro that defines the minimum time between steps in ms.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define T_STEP	10	  //Time between steps

/**********************************************************************************************//**
 * @def	CW();
 *
 * @brief	A macro that defines clockwise rotation.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define CW 1		  //Clockwise

/**********************************************************************************************//**
 * @def	CCW();
 *
 * @brief	A macro that defines counter clockwise rotation.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define CCW -1		  //Counter Clockwise

/**********************************************************************************************//**
 * @def	MOTOR_MAX_STEPS();
 *
 * @brief	A macro that defines motor maximum steps per rotation.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define MOTOR_MAX_STEPS 200 //Steps per rotation

/**********************************************************************************************//**
 * @def	POWER();
 *
 * @brief	A macro that defines the maximum power output in %.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define POWER	60	  //Maximum stepper Power in %

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

void _sleep_ms(int ms);

/**********************************************************************************************//**
 * @fn	void motor_timer_init(void)
 *
 * @brief	Initialises the timer0 for the power control. 
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

void motor_timer_init(void);	  //Initializes Timer 0 for PWM

/**********************************************************************************************//**
 * @fn	void motor_init(void)
 *
 * @brief	Initialises all used pins as output and calls motor_timer_init()
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

void motor_init(void);	  //Initializes the stepper

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

void motor_turn(char steps,char dir ); //Moves Stepper for "steps" steps in "dir" direction 

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

void motor_toPosition(char p); //Rotates Stepper to desired position

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

void motor_set_speed(char t); //Setter function for the Speed parameter

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

void motor_set_power(char power); //Setter function for Stepper Power; "power" in %

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

void motor_set_position(char pos); //Setter function for stepper position

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

char motor_get_position(void);

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

void motor_calibrate(void);



#endif /* MOTOR_H_ */