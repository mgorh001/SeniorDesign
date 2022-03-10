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

#ifndef I2C_CPP
#define I2C_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include <avr/io.h>

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// ....
#define TW_SDA_PIN PINC4
#define TW_SCL_PIN PINC5

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Defines
#define TW_SLA_W(ADDR)		((ADDR << 1) | TW_WRITE)
#define TW_SLA_R(ADDR)		((ADDR << 1) | TW_READ)

// Declaration of Constants
typedef enum {
	TW_FREQ_100K,
	TW_FREQ_250K,
	TW_FREQ_400K
} tw_freq_mode_t;

// Declaration of Global Variables
// ...

// Declaration of Functions
void I2C_Init(tw_freq_mode_t, bool);
void I2C_Start();
void I2C_Stop();
void I2C_Write(char);
char I2C_Read();

/*************************************************/
/*                Implementation                 */
/*************************************************/

// I2C Initialize
void I2C_Init(tw_freq_mode_t tw_freq_mode, bool pullup_en)
{
	TWCR = (1<<TWEN);	// Enable I2C
	TWSR = 0x00;		// Prescaler set to 1 
	
	DDRC  |= (1 << TW_SDA_PIN) | (1 << TW_SCL_PIN);
	if (pullup_en)
	{
		PORTC |= (1 << TW_SDA_PIN) | (1 << TW_SCL_PIN);
	}
	else
	{
		PORTC &= ~((1 << TW_SDA_PIN) | (1 << TW_SCL_PIN));
	}
	DDRC  &= ~((1 << TW_SDA_PIN) | (1 << TW_SCL_PIN));
	
	switch (tw_freq_mode)
	{
		case TW_FREQ_100K:
			/* Set bit rate register 72 and prescaler to 1 resulting in
			SCL_freq = 16MHz/(16 + 2*72*1) = 100KHz	*/
			TWBR = 72;
			break;
		
		case TW_FREQ_250K:
			/* Set bit rate register 24 and prescaler to 1 resulting in
			SCL_freq = 16MHz/(16 + 2*24*1) = 250KHz	*/
			TWBR = 24;
			break;
		
		case TW_FREQ_400K:
			/* Set bit rate register 12 and prescaler to 1 resulting in
			SCL_freq = 16MHz/(16 + 2*12*1) = 400KHz	*/
			TWBR = 12;
			break;
		
		default:
			break;
	}
}

// I2C Start Condition
void I2C_Start()
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);	// Start condition
	while (!(TWCR & (1<<TWINT)));				// Check for start condition
}

// I2C stop condition
void I2C_Stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

// I2C write to slave
void I2C_Write(char x)
{
	TWDR = x;									// Move value to I2C
	TWCR = (1<<TWINT) | (1<<TWEN);				// Enable I2C and clear interrupt
	while  (!(TWCR & (1<<TWINT)));				// Wait for I2C Transfer Completion
}

// I2C Read
char I2C_Read()
{
	TWCR  = (1<<TWEN) | (1<<TWINT);			// Enable I2C and clear interrupt
	while (!(TWCR & (1<<TWINT)));			// Read successful with all data received in TWDR
	return TWDR;
}

#endif		// I2C

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/