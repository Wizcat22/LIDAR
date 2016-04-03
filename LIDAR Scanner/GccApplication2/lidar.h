/*
 * lidar.h
 *
 * Created: 12.11.2015 14:40:36
 *  Author: Mathias Parys
 */ 


#ifndef LIDAR_H_
#define LIDAR_H_

/*! \def F_CPU
 *
 *  Define the CPU Clock
 */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/*! \def TWI_PREVENT_OVERPOLLING_DELAY
 *
 *  Define the time for a delay to save an overpolling in the TWI-communication
 */
#define TWI_PREVENT_OVERPOLLING_DELAY	1

/*! \def LIDARLite_ADDRESS
 *
 *  Define the default address of the LIDAR-Lite
 */
#define	LIDARLite_ADDRESS	0x62

/*! \def REGISTER_VALUE
 *
 *  Define the LIDAR-Lite register for the distance measure
 */
#define REGISTER_VALUE		0x8F

/*! \def REGISTER_MEASURE
 *
 *  Define the main konfiguration register
 */
#define REGISTER_MEASURE	0x00

/*! \def MEASURE_VALUE_WITH_DC
 *
 *  Define register for measure of distance with DC
 */
#define MEASURE_VALUE_WITH_DC		0x04

/*! \def MEASURE_VALUE_WITHOUT_DC
 *
 *  Define register for measure of distance without DC
 */
#define MEASURE_VALUE_WITHOUT_DC	0x03

/*! \def REGISTER_MEASURE_VELOCITY
 *
 *  Define register for measure of velocity
 */
#define REGISTER_MEASURE_VELOCITY	0x09

/*! \def REGISTER_SIGNAL_MAX_MIN
 *
 *  Define the register for Min-Max configuration of signal correlation
 */
#define REGISTER_SIGNAL_MAX_MIN	0x4b

/*! \def ECHO_SELECT_SECOND_RETURN
 *
 *  Define the register for the second return value
 */
#define ECHO_SELECT_SECOND_RETURN	0x01

/*! \def ECHO_SELECT_RANGE_CRITERIA
 *
 *  Define the register for the configuration of select value
 */
#define ECHO_SELECT_RANGE_CRITERIA	0x02

// Calibration Register
// 8bit signed int
//allows increasing or decreasing the measured value

/*! \def CALIBRATION_REGISTER
 *
 *  Define the register for offset calibration value
 */
#define CALIBRATION_REGISTER	0x13

/*! \def CALIBRATION_BOOT
 *
 *  Define the value for calibration value at start
 */
#define CALIBRATION_BOOT		0x00

/*! \def VELOCITY_ENABLE_OK
 *
 *  Define the success value for enabling velocity measure
 */
#define VELOCITY_ENABLE_OK	0

/*! \def VELOCITY_DISABLE_OK
 *
 *  Define the success value for disabling velocity measure
 */
#define VELOCITY_DISABLE_OK	0

/*! \def CALIBRATION_OK
 *
 *  Define the success value for calibrating
 */
#define CALIBRATION_OK		0

/*! \def WRITE_REGISTER_OK
 *
 *  Define the success value for writing in a register
 */
#define WRITE_REGISTER_OK	0

/*! \def MAX_VALUE
 *
 *  Define the peak of the LIDAR-Lite measure values
 */
#define MAX_VALUE	4000


#include <util/delay.h>
#include "twi.h"


void lidar_init();
uint16_t lidar_getValue();
uint16_t lidar_getValueAVG(uint16_t count);
uint16_t lidar_getValueWithoutDCCorrection();
uint16_t lidar_getValueAVGWithoutDCCorrection(uint8_t count);
void lidar_setVelocityEnable(uint8_t scale);
void lidar_setVelocityDisable();
int8_t lidar_getVelocity();
int8_t lidar_getVelocityAVG(uint8_t count);
void lidar_setDistanceCalibration(int8_t value);
int8_t lidar_getDistanceCalibration();
int8_t lidar_writeRegister(uint8_t reg, uint8_t value);
int8_t lidar_readRegister(uint8_t reg, uint8_t len);


#endif /* LIDAR_H_ */