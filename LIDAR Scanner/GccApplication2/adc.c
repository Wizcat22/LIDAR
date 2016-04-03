/**********************************************************************************************//**
 * @file	adc.c
 *
 * @brief	ADC class.
 **************************************************************************************************/

#include <avr/io.h>
#include <assert.h>

/**********************************************************************************************//**
 * @fn	void adc_init()
 *
 * @brief	Initialises the ADC.
 *
 * @author	Alex
 * @date	22.12.2015
 **************************************************************************************************/

void adc_init(){
	
	ADMUX |= (1<<REFS0) | (1<<MUX0);
	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	
	while (ADCSRA & (1<<ADSC))
	{
	}
	
	(void) ADCW;
	
}

/**********************************************************************************************//**
 * @fn	uint16_t adc_read()
 *
 * @brief	Reads a single value.
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @return	Value.
 **************************************************************************************************/

uint16_t adc_read(){
	
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC))
	{
	}
	return ADCW;
}

/**********************************************************************************************//**
 * @fn	uint16_t adc_read_avg(char x)
 *
 * @brief	Reads multiple values and calculates the average value.
 *
 * @author	Alex
 * @date	22.12.2015
 *
 * @param	x	Number of values to make.
 *
 * @return	Average.
 **************************************************************************************************/

uint16_t adc_read_avg(char x){
	uint16_t avg = 0;
	for (char i = 0 ; i<x ;i++)
	{
		avg+=adc_read();
	}
	return (avg/x);
}