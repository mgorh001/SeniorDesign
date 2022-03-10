/*************************************************/
/*                   ADC.cpp                     */
/*                                               */
/* This module handles the ADC module.           */
/* Analog pins required.                         */
/* It uses ADC module.                           */
/* No software dependency.   	                 */
/*                                               */
/*************************************************/

#ifndef ADC_CPP
#define ADC_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

// ...

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// ...

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Declaration of Constants
// ...

// Declaration of Global Variables
// ...

// Declaration of Functions
void ADC_Init(void);
unsigned int ADC_Read(unsigned char);

/*************************************************/
/*                Implementation                 */
/*************************************************/

// ADC Initialize Function
void ADC_Init(void)
{
	ADMUX  |= (1<<REFS0);						// Select Vref=AVcc
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	// Prescaler = 128
	ADCSRA |= (1<<ADSC);						// Single conversion mode
	ADCSRA |= (1<<ADEN);						// Enable ADC					
}

// Read from an Analog Channel
unsigned int ADC_Read(unsigned char channel)
{
	// Modifying the lower nibble of ADMUX without altering the upper one
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);	// Select ADC channel	
	ADCSRA |= (1<<ADSC);						// Single conversion mode
	while( ADCSRA & (1<<ADSC) );				// Wait until conversion is completed
	return ADC;
}

#endif

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/