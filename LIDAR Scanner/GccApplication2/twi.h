/*
 * twi.h
 *
 * Created: 26.10.2015 19:14:49
 *  Author: Mathias Parys
 */ 


#ifndef TWI_H_
#define TWI_H_

/*! \def F_CPU
 *
 *  Define the CPU Clock
 */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/*! \def F_SCL
 *
 *  Define the SCL frequency
 */
#define F_SCL 100000UL // SCL frequency

/*! \def PRESCALER
 *
 *  Define the Value of the Presacler for TWI
 */
#define PRESCALER 1

/*! \def TWBR_val
 *
 *  Define the Value for TWBR
 */
#define TWBR_val ((((F_CPU / F_SCL) / PRESCALER) - 16 ) / 2)

/*! \def TWI_OK
 *
 *  Define the success value for TWI-Connection
 */
#define TWI_OK			0

/*! \def TWI_START_ERROR
 *
 *  Define the error code for TWI-Start
 */
#define	TWI_START_ERROR			1

/*! \def TWI_START_ACK_ERROR
 *
 *  Define the error code for TWI_START_ACK_ERROR
 */
#define	TWI_START_ACK_ERROR		2

/*! \def TWI_TIMEOUT_ERROR
 *
 *  Define the error code for TWI-Timeout
 */
#define TWI_TIMEOUT_ERROR		3

/*! \def TWI_WRITE_OK
 *
 *  Define the success code for TWI-Write-Operation
 */
#define TWI_WRITE_OK			0

/*! \def TWI_WRITE_ERROR
 *
 *  Define the error code for TWI-Write-Operation
 */
#define TWI_WRITE_ERROR			4

/*! \def TWI_STOP_OK
 *
 *  Define the success code for TWI-Write-Operation
 */
#define TWI_STOP_OK				0

/*! \def TWI_WRITE_REG_OK
 *
 *  Define the success for TWI-Write-Register-Operation
 */
#define TWI_WRITE_REG_OK		0

/*! \def TWI_READ_REG_OK
 *
 *  Define the success for TWI-Read-Register-Operation
 */
#define TWI_READ_REG_OK			0

/*! \def TWI_CONNECTION_ERROR
 *
 *  Define the error value for no TWI-Connections
 */
#define TWI_CONNECTION_ERROR	5

/*! \def TIMEOUT_DELAYWERT
 *
 *  Define the delay value for TWI-Connection-Timeout
 */
#define TIMEOUT_DELAYWERT	25

/*! \def TIMEOUT_TRYS
 *
 *  Define the Quatity of trys for TWI-Connection
 */
#define TIMEOUT_TRYS		250

/*! \def SCL
 *
 *  Define the SCL Pin
 */
#define SCL	PINC5

/*! \def SDA
 *
 *  Define the SDA Pin
 */
#define SDA PINC4


#include <util/delay.h>
#include <avr/io.h>
#include <util/twi.h>


void twi_init(void);
uint8_t twi_get_status(void);
uint8_t twi_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t data);
uint8_t twi_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint8_t length);


#endif /* TWI_H_ */