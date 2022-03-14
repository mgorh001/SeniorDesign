/*************************************************/
/*                 LCD_I2C.cpp                   */
/*                                               */
/* This module handles the LCD module,           */
/* When using an I2C output expander.            */
/* It requires an I2C module.					 */
/* This software driver requires:                */
/* 1) io.h                                       */
/*                                               */
/*************************************************/

// ...

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include <avr/io.h>

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// ....

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Declaration of Constants
// ...

// Declaration of Global Variables
// ...

// Declaration of Functions
void I2C_Init();
void I2C_Start();
void I2C_Write(char);
char I2C_Read();

/*************************************************/
/*                Implementation                 */
/*************************************************/

// I2C Initialize
void I2C_Init()
{
	TWBR = 0x62;		// Baud rate is set by calculating 
	TWCR = (1<<TWEN);	// Enable I2C
	TWSR = 0x00;		// Prescaler set to 1 
}

// I2C Start Condition
void I2C_Start()
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);	// Start condition
	while (!(TWCR & (1<<TWINT)));				// Check for start condition
}

// I2C stop condition
void I2C_Write(char x)
{
	TWDR = x;									// Move value to I2C
	TWCR = (1<<TWINT) | (1<<TWEN);				// Enable I2C and clear interrupt
	while  (!(TWCR & (1<<TWINT)));
}

// I2C Read
char I2C_Read()
{
	TWCR  = (1<<TWEN) | (1<<TWINT);			// Enable I2C and clear interrupt
	while (!(TWCR & (1<<TWINT)));			// Read successful with all data received in TWDR
	return TWDR;
}

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/