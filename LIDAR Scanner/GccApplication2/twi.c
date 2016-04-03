/*
 * twi.c
 *
 * Created: 26.10.2015 19:14:56
 *  Author: Mathias Parys
 */ 

#include "twi.h"

uint8_t timeoutValue =	TIMEOUT_TRYS;

//initialize TWI

/*! \brief Initialize the TWI-Communication
 *
 *  This function inizialize the TWI-communication.
 */
void twi_init(void){
	
	//Initialize twi Prescaler
	TWSR |= (0 << TWPS0);
	TWSR |= (0 << TWPS1);
	
	//TWBR Abhängig von F_CPU, daher per Define berechnet
	//set SCL to 100kHz
	TWBR = TWBR_val;
	
	//Enable TWI anc ACKS
	TWCR = (1<<TWEN) | (1<<TWEA);
}

static void twi_lockUp(){
	//Release SDA and SCL to high
	TWCR =0;
	TWCR = (1<<TWEN) | (1<<TWEA);
	
}

// Stop TWI Communication
static uint8_t twi_stop(void){
	//Stop Communication
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	
	uint8_t timeout = timeoutValue;
	while (TWCR & (1<<TWSTO)){
		if (timeout > 0)
		{
			_delay_us(TIMEOUT_DELAYWERT);
			timeout--;
		}
		else
		{
			return TWI_TIMEOUT_ERROR;
		}
	}
	
	return TWI_STOP_OK;
}

//Start TWI Communication
static uint8_t twi_start(){
	
	uint8_t status = twi_get_status();
	//uint8_t timeout =0;
	//Send Start condition
	TWCR |= (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	//Wait for TWINT Flag set; -> Start condition has been transmitted
	uint8_t timeout = timeoutValue;
	while( !(TWCR & (1<<TWINT)) ){
		if (timeout > 0)
		{
			_delay_us(TIMEOUT_DELAYWERT);
			timeout--;
		}
		else
		{
			return TWI_CONNECTION_ERROR;
		}
	}
	

	status = twi_get_status();
	if ((status == TW_START) || (status == TW_REP_START))
	{
		return TWI_OK;
	}
	else
	{
		if (status == TW_MT_ARB_LOST)
		{
			twi_lockUp();
			return status;
		} 
		else
		{
			return status;
		}
	}
}

static uint8_t twi_sendAdress(uint8_t address){	
	//Write LIDAR-Address to TWDR
	TWDR = address;
	
	//Clear TWINT; Start transmission
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	//Wait for TWINT-FLag; ACK has been received
	uint8_t timeout = timeoutValue;
	while( !(TWCR & (1<<TWINT)) ){
		if (timeout > 0)
		{
			_delay_us(TIMEOUT_DELAYWERT);
			timeout--;
		}
		else
		{
			return TWI_TIMEOUT_ERROR;
		}
	}
	
	//Check if the Device has acknowledged the READ / WRITE mode
	if ((TW_STATUS == TW_MT_SLA_ACK) || (TW_STATUS == TW_MR_SLA_ACK)){
		return TWI_OK;
	}
	if ((TW_STATUS == TW_MT_SLA_NACK) || (TW_STATUS == TW_MR_SLA_NACK))
	{
		uint8_t status = TW_STATUS;
		twi_stop();
		return status;
	} 
	else
	{
		uint8_t status = TW_STATUS;
		twi_lockUp();
		return status;
	}
}

//Write to TWDR Register; Shift to SDA 
static uint8_t twi_write(uint8_t data){
	//Load DATA to TWDR Register
	TWDR = data;
	//Clear TWINT to start transmission
	TWCR = (1 << TWINT) | (1 << TWEN);
	//Wait for TWINT Flag; -> DATA transmitted and ACK/NACK received.
uint8_t timeout = timeoutValue;
while( !(TWCR & (1<<TWINT)) ){
	if (timeout > 0)
	{
		_delay_us(TIMEOUT_DELAYWERT);
		timeout--;
	}
	else
	{
		return TWI_TIMEOUT_ERROR;
	}
}
	//Check TWI Status;
	if (TW_STATUS == TW_MT_DATA_ACK ){ 
		return TWI_WRITE_OK;
	}
	if (TW_STATUS == TW_MR_DATA_NACK)
	{
		uint8_t status = TW_STATUS;
		twi_stop();
		return status;
	} 
	else
	{
		uint8_t status = TW_STATUS;
		twi_lockUp();
		return status;
		
	}
}

//Read with ACK signal
static uint8_t twi_read_ack(void){
	//READ with ACK
	TWCR = ((1<<TWINT) | (1<<TWEN) | (1<<TWEA));
uint8_t timeout = timeoutValue;
while( !(TWCR & (1<<TWINT)) ){
	if (timeout > 0)
	{
		_delay_us(TIMEOUT_DELAYWERT);
		timeout--;
	}
	else
	{
		return TWI_TIMEOUT_ERROR;
	}
}
	
	if (TW_STATUS == TW_MT_ARB_LOST)
	{
		uint8_t status = TW_STATUS;
		twi_lockUp();
		return status;
	}
	
	//Return Status
	return TW_STATUS;
}

//Read without ACK signal
static uint8_t twi_read_nack(void){
	//READ without ACK
	TWCR = ((1<<TWINT) | (1<<TWEN));
uint8_t timeout = timeoutValue;
while( !(TWCR & (1<<TWINT)) ){
	if (timeout > 0)
	{
		_delay_us(TIMEOUT_DELAYWERT);
		timeout--;
	}
	else
	{
		return TWI_TIMEOUT_ERROR;
	}
}
	
	if (TW_STATUS == TW_MT_ARB_LOST)
	{
		uint8_t status = TW_STATUS;
		twi_lockUp();
		return status;
	}
	
	//Return Status
	return TW_STATUS;
}

/*! \brief Get the status of the TWI-communication
 *
 *  Get the Status of the TWI/I2C Communication. Status are the upper 5 Bits
 */
uint8_t twi_get_status(void){
	//status with mask. 
	return TW_STATUS;	
}


/*! \brief Write in the register
 *
 *  Write the Value in the register of the device
 *	\param devaddr Address of the Device
 *	\param regaddr Address of the Register
 *	\param data	Data to write
 *
 */
uint8_t twi_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t data)
{
	//Start TWI/I2C Communication
	uint8_t error =0;
	error = twi_start();
	if (error != TWI_OK) {
		return error;
	}
	//Device-Address with write
	error = twi_sendAdress(devaddr<<1);
	if (error != TWI_OK)
	{
		return error;
	}
	//Send Register-Address for Write-Operation to Device
	error = twi_write(regaddr);
	if (error != TWI_WRITE_OK)
	{
		return error;
	}
	
	//Send Value to Device
	error = twi_write(data);
	if (error != TWI_WRITE_OK){
		return error;
	}
	//End TWI/I2C Communication
	error = twi_stop();
	if (error != TWI_STOP_OK) {
		return error;
	}
	return TWI_WRITE_REG_OK;
}


/*! \brief Read out of the register
 *
 *  Read value out of the device register on the position of data pointer.
 *	\param devaddr	Address of the Device
 *	\param regaddr	Address of the Register
 *	\param data		Pointer for saving point of the data
 *	\param length	Length of byte to read
 */
uint8_t twi_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint8_t length)
{
	//Start TWI/I2C Communication (Write-Mode)
	uint8_t nack = length -1;
	uint8_t error =0;
	error = twi_start();
	if (error) {
		return error;
	}
	//Device-Address with write
	error = twi_sendAdress(devaddr<<1);
	if (error)
	{
		return error;
	}
	//Send Register-Address for Read-Operation to Device
	error = twi_write(regaddr);
	if (error)
	{
		return error;
	}
	
	error = twi_start();
	if (error) {
		return error;
	}
	
	//Switch Communication to Read-Mode
	error = twi_sendAdress((devaddr<<1) + 0x01);
	if (error) {
		return error;
	}
	
	//Read Values from Device
	for (uint8_t i = 0; i < (length); i++)
	{
		if (i == nack)
		{
			error = twi_read_nack();
			if (error != TW_MR_DATA_NACK)
			{
				return error;
			}
		} 
		else
		{
			error = twi_read_ack();
			if (error != TW_MR_DATA_ACK)
			{
				return error;
			}
		}
		data[i] = TWDR;
	}
	//Stop Communication
	error = twi_stop();
	if (error != TWI_STOP_OK)
	{
		return error;
	}
	return TWI_READ_REG_OK;
}
