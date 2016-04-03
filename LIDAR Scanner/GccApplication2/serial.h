/*
 * serial.h
 *
 * Created: 26.10.2015 17:41:46
 *  Author: Mathias Parys
 */ 


#ifndef SERIAL_HEADER
#define SERIAL_HEADER

/*! \def F_CPU
 *
 *  Define the CPU Clock
 */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/*! \def BAUD
 *
 *  Define the BAUD-Rate. It is the rate of symbols for UART
 */
#ifndef BAUD
#define BAUD 56000
#endif

/*! \def NUMBER_OF_DIGITS
 *
 *  Define the peak of digits for an integer
 */

#define NUMBER_OF_DIGITS 5

/*! \def STRING_LENGTH
 *
 *  Define the peak of characters for a string
 */
#define STRING_LENGTH	20

/*! \def BUFFER_SIZE
 *
 *  Define the size of the array for the buffer
 */
#define BUFFER_SIZE		20



#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>
#include <stdlib.h>
#include <util/delay.h>


void serial_init(void);
void serial_write_char(unsigned char c);
void serial_write_string(char *string);
void serial_write_int(uint16_t i);
char serial_read_char(void);
char* serial_read_string();
int16_t serial_read_int();

#endif



