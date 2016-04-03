/*
* lidar.c
*
* Created: 12.11.2015 14:40:21
*  Author: Mathias Parys
*/

#include "lidar.h"
#include "avr/interrupt.h"



/*! \brief Initialize the LIDAR-Lite
 *
 *  This function inizialize the TWI-communication and set the default calibration value.
 */
void lidar_init(){
	twi_init();
	lidar_setDistanceCalibration(CALIBRATION_BOOT);
}

/*! \brief Get value of distance measure
 *
 *  This function start a new distance measure.
 *  \return Distance measure
 */
uint16_t lidar_getValue(){
	uint16_t convert;
	uint8_t data[2];
	uint8_t *dp = &data[0];
	uint8_t errorW = 100;
	uint8_t errorR = 100;
	cli();
	while (errorW != 0)
	{
		errorW = twi_writeReg(LIDARLite_ADDRESS, REGISTER_MEASURE, MEASURE_VALUE_WITH_DC);
		if (errorW == TWI_CONNECTION_ERROR)
		{
			return TWI_CONNECTION_ERROR;
		}
		_delay_ms(TWI_PREVENT_OVERPOLLING_DELAY);
	}
	while (errorR != 0)
	{
		errorR = twi_readReg(LIDARLite_ADDRESS, REGISTER_VALUE, dp, 2);
		_delay_ms(TWI_PREVENT_OVERPOLLING_DELAY);
		if (errorR == TWI_CONNECTION_ERROR)
		{
			return TWI_CONNECTION_ERROR;
		}
	}
	sei();
	int16_t temp;
	temp = data[0];
	temp = (temp <<8);
	convert = (temp + data[1]);
	if (convert == 0)
	{
		return MAX_VALUE;
	}
	return convert;
}

/*! \brief Get average of new distance measures
 *
 *	This function makes a average of a given count of distance measures
 *
 *	\param count Quantity of Values
 *  \return Distance measure average
 */
uint16_t lidar_getValueAVG(uint16_t count){
	uint16_t average = 0;
	for (uint8_t i =0; i < count; i++)
	{
		average = average + lidar_getValue();
		//_delay_us(50);
	}
	average = average / count;
	return average;
}

/*! \brief Get value of distance measure without DC-Correction
 *
 *  This function start a new distance measure without DC-Correction.
 *  \return Distance measure
 */
uint16_t lidar_getValueWithoutDCCorrection(){
	int16_t convert;
	uint8_t data[2];
	uint8_t *dp = &data[0];
	uint8_t errorW = 1;
	uint8_t errorR = 1;
	cli();
	while (errorW != 0)
	{
		errorW = twi_writeReg(LIDARLite_ADDRESS, REGISTER_MEASURE, MEASURE_VALUE_WITHOUT_DC);
		if (errorW == TWI_CONNECTION_ERROR)
		{
			return TWI_CONNECTION_ERROR;
		}
		//_delay_ms(TWI_PREVENT_OVERPOLLING_DELAY);
	}
	while (errorR != 0)
	{
		errorR = twi_readReg(LIDARLite_ADDRESS, REGISTER_VALUE, dp, 2);
		if (errorR == TWI_CONNECTION_ERROR)
		{
			return TWI_CONNECTION_ERROR;
		}
		//_delay_ms(TWI_PREVENT_OVERPOLLING_DELAY);
	}
	sei();
	
	int16_t temp;
	temp = data[0];
	temp = (temp <<8);
	convert = (temp + data[1]);
	return convert;
}

/*! \brief Get average of new distance measures without DC-Correction
 *
 *	This function makes a average of a given count of distance measures without DC-Correction.
 *
 *	\param count Quantity of Values
 *  \return Distance measure average
 */
uint16_t lidar_getValueAVGWithoutDCCorrection(uint8_t count){
	uint16_t average = 0;
	for (uint8_t i =0; i < count; i++)
	{
		average += lidar_getValueWithoutDCCorrection();
	}
	average = average / count;
	return average;
}


/*! \brief Enable the Velocity measure
 *
 *  This function enables the Velocity measure.
 *
 *  Velocity Scaling
 *  Measurement | Velocity        | velocityScalingValue
 *  Period (ms) | Scaling (m/sec) |
 *  :-----------| :---------------| :-------------------
 *  100         | 0.10 m/s        | 0
 *  10          | 1.00 m/s        | 1
 *
 */

void lidar_setVelocityEnable(uint8_t scale){
	uint8_t regval;
	if (scale == 0)
	{
		regval = 0xA0;
	}
	else
	{
		regval = 0x80;
	}
	uint8_t errorW = 100;
	cli();
	while (errorW != 0)
	{
		errorW = twi_writeReg(LIDARLite_ADDRESS, MEASURE_VALUE_WITH_DC, regval);
		_delay_ms(TWI_PREVENT_OVERPOLLING_DELAY);
	}
	sei();
}

/*! \brief Disable the Velocity measure
 *
 *  This function disables the Velocity measure.
 *  
 */
void lidar_setVelocityDisable(){
	uint8_t errorW = 1;
	cli();
	while (errorW != 0)
	{
		errorW = twi_writeReg(LIDARLite_ADDRESS, REGISTER_MEASURE, 0x00);
		_delay_ms(TWI_PREVENT_OVERPOLLING_DELAY);
	}
	sei();
}

/*! \brief Get value of velocity measure
 *
 *  This function gives a new velocity measure.
 *	\return Velocity measure
 */
int8_t lidar_getVelocity()
{
	uint8_t data;
	uint8_t *dp = &data;
	uint8_t errorW = 1;
	uint8_t errorR = 1;
	cli();
	while (errorW != 0)
	{
		errorW = twi_writeReg(LIDARLite_ADDRESS, REGISTER_MEASURE, MEASURE_VALUE_WITH_DC);
		if (errorW == TWI_CONNECTION_ERROR)
		{
			return TWI_CONNECTION_ERROR;
		}
		_delay_ms(TWI_PREVENT_OVERPOLLING_DELAY);
	}
	while (errorR != 0)
	{
		errorR = twi_readReg(LIDARLite_ADDRESS, REGISTER_MEASURE_VELOCITY, dp, 1);
		if (errorR == TWI_CONNECTION_ERROR)
		{
			return TWI_CONNECTION_ERROR;
		}
		_delay_ms(TWI_PREVENT_OVERPOLLING_DELAY);
	}
	sei();
	return data;
}

/*! \brief Get average of new velocity measures
*
*	This function makes a average of a given count of velocity measures
*
*	\param count Quantity of Values
*  \return Velocity measure average
 */
int8_t lidar_getVelocityAVG(uint8_t count){
	int8_t average = 0;
	int8_t temp =0;
	for (uint8_t i =0; i < count; i++)
	{
		temp = lidar_getVelocity();
		if (temp < 0)
		{
			temp = (temp *(-1));
			average = average - temp;
		}
		else
		{
			average = average + temp;
		}
	}
	average = average / count;
	return average;
}

/*! \brief Set distance calibration
 *
 *  This function set the value for the offset calibration.
 */
void lidar_setDistanceCalibration(int8_t value){
	if (value > 127)
	{
		value = 127;
	}
	else if (value < -128)
	{
		value = -128;
	}
	
	uint8_t errorW = 1;
	cli();
	while (errorW != 0)
	{
		errorW = twi_writeReg(LIDARLite_ADDRESS, CALIBRATION_REGISTER, value);
		_delay_ms(TWI_PREVENT_OVERPOLLING_DELAY);
	}
	sei();
}

/*! \brief Get distance calibration
 *
 *  This function return the value of the offset calibration.
 *	\return Offset value
 */
int8_t lidar_getDistanceCalibration(){
	uint8_t data;
	uint8_t *dp = &data;
	uint8_t errorR = 1;
	cli();
	while (errorR != 0)
	{
		errorR = twi_readReg(LIDARLite_ADDRESS, CALIBRATION_REGISTER, dp, 1);
		if (errorR == TWI_CONNECTION_ERROR)
		{
			return TWI_CONNECTION_ERROR;
		}
		//_delay_ms(TWI_PREVENT_OVERPOLLING_DELAY);
	}
	sei();
	return data;
}

/*! \brief Write in Register
 *
 *  Write the Value in the Register. Check the LIDAR-Lite documentation for more informations of the register.
 *	\param reg Register
 *	\param value Value
 */
int8_t lidar_writeRegister(uint8_t reg, uint8_t value){
	uint8_t errorW = 1;
	cli();
	while (errorW != 0)
	{
		errorW = twi_writeReg(LIDARLite_ADDRESS, reg, value);
		if (errorW == TWI_CONNECTION_ERROR)
		{
			return TWI_CONNECTION_ERROR;
		}
		//_delay_ms(TWI_PREVENT_OVERPOLLING_DELAY);
	}
	sei();
	return WRITE_REGISTER_OK;
}

/*! \brief Read in Register
 *
 *  Read the Value in the Register. Check the LIDAR-Lite documentation for more informations of the register.
 *	\param reg Register
 *	\param len Length in Byte
 */
int8_t lidar_readRegister(uint8_t reg, uint8_t len){
	uint8_t data;
	uint8_t *dp = &data;
	uint8_t errorR = 1;
	cli();
	while (errorR != 0)
	{
		errorR = twi_readReg(LIDARLite_ADDRESS, reg, dp, len);
		if (errorR == TWI_CONNECTION_ERROR)
		{
			return TWI_CONNECTION_ERROR;
		}
		//_delay_ms(TWI_PREVENT_OVERPOLLING_DELAY);
	}
	sei();
	return data;
}