/*
* serial.c
*
* Created: 26.10.2015 17:41:58
*  Author: Stalev
*/


#include "serial.h"



/*! \brief Serial-Buffer
 *  
 *  This is the buffer for serial reading. The ISR write any new character in this array.
 */

volatile char buffer[BUFFER_SIZE];


/*! \brief Readpointer for  buffer
 *  
 *  This is the readpointer for the buffer. This position is the position of the next readable character.
 */

volatile char* readpointer;


/*! \brief Writepointer for  buffer
 *  
 *  This is the writepointer for the buffer. This position in the array is the position for the next character in the array.
 */

volatile char* writepointer;


/*! \brief Initialize the UART communication
 *
 *  This function initialize the communication for UART. The readpointer and the writepointer will be set on the first position of the array.
 *  After this, the BAUD-Rate will be set and the Interrupts will be enabled. 
 */

void serial_init(void){
	
	readpointer = buffer;
	writepointer = buffer;
	
	//Set BAUD-Rate
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	//Set 8 Data-Bit, 1 Stop-Bit, No Parity
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);

	
	//Set RX and TX ENABLE
	UCSR0B = (1 << RXEN0) | (1 << TXEN0)  | (1<<RXCIE0);
	sei();
}


/*! \brief Send character
 *
 *  Send a character over the UART.
 *  \param c charater to send
 */

void serial_write_char(unsigned char c){
	// Wait for empty DATA Register
	while ( !(UCSR0A & (1 << UDRE0)) ){
	}
	//Write CHAR to DATA Register
	UDR0 = c;
}


/*! \brief Send string
 *
 *  Send a string over the UART.
 *  \param string pointer of the first character of the string
 */

void serial_write_string(char *string){
	while (*string != '\0'){
		serial_write_char(*string);
		string ++;
	}
}


/*! \brief Send integer
 *
 *  Send an integer over the UART.
 *  \param i integer to send
 */


void serial_write_int(uint16_t i){
	char buffer[NUMBER_OF_DIGITS+1];
	itoa(i, buffer, 10 );
	serial_write_string(buffer);
}


/*! \brief Read char
 *
 *  Read a character out of the buffer
 *  \return Received character
 */

char serial_read_char(void){
	char data;
	if (writepointer != readpointer)
	{
		data = *readpointer;
		if (readpointer == (buffer + BUFFER_SIZE-1))
		{
			readpointer = buffer;
		}
		else
		{
			readpointer++;
		}
	}
	else
	{
		data = '\0';
	}
	return data;
}


/*! \brief Read string
 *
 *  Read a sting out of the buffer with a peak of 20 characters
 *  \return Pointer of the first character of the received string
 */

char* serial_read_string(){
	char word[STRING_LENGTH+1];
	char *wp =  &word[0];
	for (uint8_t i =0; i <= STRING_LENGTH; i++){
		word[i] =  serial_read_char();
		if (word[i] == '\0')
		{
			break;
		}
		if (i == STRING_LENGTH)
		{
			word[STRING_LENGTH+1] = '\0';
		}
	}
	//Return read DATA
	return wp ;
}


/*! \brief Read integer
 *
 *  Read a signed integer (16-Bit) out of the buffer. 
 *  \return Received integer
 */

int16_t serial_read_int(){
	int16_t value = 0;
	char c = 0;
	char minus = 1;
	for (uint8_t i = 0; i<=NUMBER_OF_DIGITS;i++)
	{
		c = serial_read_char();
		if (value == 0 && c == '-')
		{
			minus = -1;
			continue;
		}
		if (c < 58 && c > 47 )
		{
			if (value < 5000)
			{
				value = value * 10 + atoi(&c);
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return value * minus;
		}
	}
	return value * minus;
}


/*! \brief Interrupt Service Routine
 *
 *  Interrupt Service Routine for an UART receive Interrupt.
 */

ISR (USART_RX_vect){
	cli();
	*writepointer = UDR0;
	if (writepointer == (buffer + BUFFER_SIZE-1))
	{
		writepointer = buffer;
	}
	else
	{
		writepointer++;
	}
	sei();
}