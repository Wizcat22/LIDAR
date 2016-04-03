/**********************************************************************************************//**
 * @file	main.c
 *
 * @brief	main class.
 **************************************************************************************************/

#ifndef F_CPU

/**********************************************************************************************//**
 * @def	F_CPU
 *
 * @brief	A macro that defines CPU frequency.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

#define F_CPU 16000000L     /* Cristal with 16 MHz */
#endif

#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "motor.h"
#include "adc.h"
#include "serial.h"
#include "lidar.h"
#include "servo.h"

/**********************************************************************************************//**
 * @fn	void send_data(char mpos, char sdeg, uint16_t val)
 *
 * @brief	Sends data in the right format.
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @param	mpos	The motor position.
 * @param	sdeg	The servo position.
 * @param	val 	The distance value.
 **************************************************************************************************/

void send_data(char mpos, char sdeg, uint16_t val){
	serial_write_string("mpos: ");
	serial_write_int(mpos);
	serial_write_string(" sdeg: ");
	serial_write_int(sdeg);
	serial_write_string(" val: ");
	serial_write_int(val);
	serial_write_string("\r\n");
}

/**********************************************************************************************//**
 * @fn	int main(void)
 *
 * @brief	Main entry-point for this application.
 *          At first everything gets initialised and the motor gets calibrated.
 *          Then the function waits for user input.
 *          Every command begins with # followed by a number or ? \n
 *          The actions are : \n
 *          #?: Show possible commands \n
 *          #1: Return distance value of the current position \n
 *          #2: Activate radar mode (2D, cancel with #) \n
 *          #3 mmm sss: Set position of motor and servo \n
 *          #4: Calibrate motor \n
 *          #5: Activate radar mode (3D, motor first, cancel with #) \n
 *          #6 x: Set number of values per average \n
 *          #7: Onetime measure of velocity in 1m/s \n
 *          #8 : Onetime measure of velocity in 0.1m/s \n
 *	    #9 x: Set Offset of measurement \n
 *          
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @return	Exit-code for the process - 0 for success, else an error code.
 **************************************************************************************************/

int main(void)
{
	wdt_enable(WDTO_2S);
	uint16_t avg = 10;
	int16_t v = 0;
	char stop = 0;
	
	
	serial_init();
	serial_write_string("SERIAL READY!\r\n");
	wdt_reset();
	lidar_init();
	serial_write_string("TWI READY!\r\n");
	wdt_reset();
	adc_init();
	serial_write_string("ADC READY!\r\n");
	wdt_reset();
	motor_init();
	serial_write_string("STEPPER READY!\r\n");
	wdt_reset();
	servo_init();
	serial_write_string("SERVO READY!\r\n");
	wdt_reset();
	serial_write_string("Calibrating...\r\n");
	motor_calibrate();
	wdt_reset();
	serial_write_string("############################ \r\n");
	serial_write_string("LIDAR Ready! \r\n");
	serial_write_string("Type #? for more information.\r\n");
	wdt_reset();
	

	
	char a;
	char b;

	while (1)
	{
		wdt_reset();
		stop = 0;
		_delay_ms(100);
		a = serial_read_char();
		b = serial_read_char();
		
		//servo_toPosition(0);
		if(a == '#')
		{
			switch(b){
				//////////////////////////////////////////////////////////////////////////
				case '?':
				wdt_reset();
				serial_write_string("Possible actions: \r\n");
				serial_write_string("#? : Help \r\n");
				serial_write_string("#1 : Onetime measure of distance\r\n");
				serial_write_string("#2 : Radar mode 2D\r\n");
				serial_write_string("#3 mmm sss : Set Position of Motor and Servo \r\n");
				serial_write_string("#4 : Calibration \r\n");
				serial_write_string("#5 : Radar mode 3D (M first)\r\n");
				serial_write_string("#6 : Set number of values per measurement (default = 10)\r\n");
				serial_write_string("#7 : Onetime measure of velocity in 1m/s\r\n");
				serial_write_string("#8 : Onetime measure of velocity in 10cm/s\r\n");
				serial_write_string("#9 x: Set Offset of measurement\r\n");

				break;
				//////////////////////////////////////////////////////////////////////////
				case '1':
				wdt_reset();
				serial_write_string("Onetime Measure: \r\n");
				send_data(motor_get_position(),servo_get_position(),lidar_getValueAVG(avg));

				break;
				//////////////////////////////////////////////////////////////////////////
				case '2':
				wdt_reset();
				serial_write_string("Radar mode 2D activated! \r\n");
				while (1)
				{
					wdt_reset();
					if (stop == 1)
					{
						break;
					}
					if (motor_get_position() == 1)
					{
						for(int i = 0; i<=MOTOR_MAX_STEPS/2;i++ ){
							wdt_reset();
							if(serial_read_char() == '#'){
								motor_calibrate();
								stop = 1 ;
								break;
							}
							motor_toPosition(i);
							send_data(motor_get_position(),servo_get_position(),lidar_getValueAVG(avg));
						}
					}
					else
					{
						for(int i = MOTOR_MAX_STEPS/2; i>0;i-- ){
							wdt_reset();
							if(serial_read_char() == '#'){
								motor_calibrate();
								stop = 1 ;
								break;
							}
							motor_toPosition(i);
							send_data(motor_get_position(),servo_get_position(),lidar_getValueAVG(avg));
						}
					}
				}
				
				break;
				//////////////////////////////////////////////////////////////////////////
				case '3':
				{
					wdt_reset();
					uint16_t m ;
					uint16_t s ;
					serial_read_char();
					m = serial_read_int();
					s = serial_read_int();
					motor_toPosition(m);
					servo_toPosition(s);
					
				}
				
				break;
				//////////////////////////////////////////////////////////////////////////
				case '4':
				motor_calibrate();
				serial_write_string("Calibrated \r\n");
				break;
				//////////////////////////////////////////////////////////////////////////
				case '5': serial_write_string("Radar mode 3D (M first) activated! \r\n");
				while (1)
				{
					wdt_reset();
					if (stop == 1)
					{
						break;
					}
					for (char k = 0; k<= SERVO_MAX_DEG/2 ; k++)
					{
						wdt_reset();
						if (stop == 1)
						{
							break;
						}
						servo_toPosition(k);

							for(int i = 0; i<=MOTOR_MAX_STEPS/2;i++ ){
								wdt_reset();
								if(serial_read_char() == '#'){
									motor_calibrate();
									stop = 1 ;
									break;
								}
								motor_toPosition(i);
								send_data(motor_get_position(),servo_get_position(),lidar_getValueAVG(avg));
							}
							k++;
							servo_toPosition(k);
						
							for(int i = MOTOR_MAX_STEPS/2; i>=0;i-- ){
								wdt_reset();
								if (stop == 1){break;}
									
								if(serial_read_char() == '#'){
									motor_calibrate();
									stop = 1 ;
									break;
								}
								motor_toPosition(i);
								send_data(motor_get_position(),servo_get_position(),lidar_getValueAVG(avg));
							}
							
						
						
					}
				}
				
				break;
				//////////////////////////////////////////////////////////////////////////
				case '6': serial_write_string("#6 : Set number of values per measurement\r\n");

				wdt_reset();
				serial_read_char();
				avg = serial_read_int();
				serial_write_string("Number of Values = ");
				serial_write_int(avg);
				serial_write_string("\r\n");

				
				break;
				//////////////////////////////////////////////////////////////////////////
				case '7': serial_write_string("#7 : Onetime measure of velocity in 1m/s\r\n");

				wdt_reset();
				lidar_setVelocityEnable(1);
				v = lidar_getVelocityAVG(avg);
				lidar_setVelocityDisable();
				serial_write_string("Velocity = ");
				//konvertierung von signed auf unsigned
				if (v<0)
				{
					v = v*(-1);
					serial_write_string("-");
				}
				serial_write_int(v);
				serial_write_string("\r\n");

				

				
				break;
				//////////////////////////////////////////////////////////////////////////
				case '8': serial_write_string("#8 : Onetime measure of velocity in 10cm/s\r\n");

				wdt_reset();
				lidar_setVelocityEnable(0);
				v = lidar_getVelocityAVG(avg);
				lidar_setVelocityDisable();
				serial_write_string("Velocity = ");
				//converting signed to unsigned
				if (v<0)
				{
					v = v*(-1);
					serial_write_string("-");
				}
				serial_write_int(v);
				serial_write_string("\r\n");

				
				break;
				//////////////////////////////////////////////////////////////////////////
				case '9': serial_write_string("#9 x: Set Offset of measurement\r\n");

				wdt_reset();
				serial_read_char();
				int16_t offset = serial_read_int();
				lidar_setDistanceCalibration(offset);
				serial_write_string("Offset = ");
				serial_write_int(lidar_getDistanceCalibration());
				serial_write_string("\r\n");
				
				break;
				//////////////////////////////////////////////////////////////////////////
				default: serial_write_string("Unknown code!\r\n");
			}
		}
	}
}


