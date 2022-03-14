/*************************************************/
/*                   LCD.cpp                     */
/*                                               */
/* This module handles the LCD module.           */
/* It requires one output port for LCD data,     */
/* and another three pins as output for control. */
/* This software driver requires:                */
/* 1) delay.h                                    */
/* 2) System_Hardware.h                          */
/*   (For External Hardware Definitions)         */
/*                                               */
/*************************************************/

#ifndef LCD_I2C_CPP
#define LCD_I2C_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include <util/delay.h>
#include <avr/io.h>
#include "System_Hardware.h"

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// LCD Pins
#define LCD_I2C_RS_PIN	0 // SYS_LCD_I2C_RS_PIN
#define LCD_I2C_EN_PIN	1 // SYS_LCD_I2C_EN_PIN

// LCD Ports
// ...

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Declaration of Constants
// ...

// Declaration of Global Variables
// ...

// Declaration of Functions
void LCD_I2C_Init();
void LCD_I2C_Send_Latch();
void LCD_I2C_Send_Command(uint8_t);
void LCD_I2C_Send_Command_Half(uint8_t);
void LCD_Send_Data(uint8_t);
void LCD_Clear(void);
void LCD_Set_Cursor(uint8_t, uint8_t);
void LCD_Putchar(uint8_t);
void LCD_Puts(uint8_t*);

/*************************************************/
/*                Implementation                 */
/*************************************************/

// Initialize LCD
void LCD_I2C_Init()
{
	LCD_I2C_Send_Command_Half(0x30);	// Sequence for initializing LCD
	LCD_I2C_Send_Command_Half(0x30);	//
	LCD_I2C_Send_Command_Half(0x20);	//
	LCD_I2C_Send_Command(0x28);			// Selecting 16 x 2 LCD in 4Bit mode
	LCD_I2C_Send_Command(0x0C);			// Display ON Cursor OFF
	LCD_I2C_Send_Command(0x01);			// Clear display
	LCD_I2C_Send_Command(0x06);			// Cursor Auto Increment
	LCD_I2C_Send_Command(0x80);			// 1st line 1st location of LCD
}

// Latching data into LCD data register using High to Low signal on the LCD_I2C_EN_PIN
void LCD_I2C_Send_Latch()
{
	TWDR |=  (1<<LCD_I2C_EN_PIN);		// PIN LCD_EN = 1.
	TWCR = (1<<TWINT) | (1<<TWEN);		// Enable I2C and clear interrupt
	while  (!(TWCR & (1<<TWINT)));		// Wait for I2C Transfer Completion
	
	_delay_ms(1);
	
	TWDR &= ~(1<<LCD_I2C_EN_PIN);		// PIN LCD_EN = 0. 
	TWCR = (1<<TWINT) | (1<<TWEN);		// Enable I2C and Clear Interrupt
	while  (!(TWCR & (1<<TWINT)));		// Wait for Completion
}

//
void LCD_I2C_Send_Command_Half(uint8_t data)
{
	TWDR &= ~(1<<LCD_I2C_RS_PIN);		// PIN LCD_RS = 0. Selecting register as Command register
	TWCR = (1<<TWINT) | (1<<TWEN);		// Enable I2C and clear interrupt
	while  (!(TWCR & (1<<TWINT)));		// Wait for I2C Transfer Completion
	
	TWDR &= 0x0F;						// Clearing the Higher 4 bits
	TWCR = (1<<TWINT) | (1<<TWEN);		// Enable I2C and clear interrupt
	while  (!(TWCR & (1<<TWINT)));		// Wait for I2C Transfer Completion
	
	TWDR |= (data & 0xF0);				// Masking higher 4 bits and sending to LCD
	TWCR = (1<<TWINT) | (1<<TWEN);		// Enable I2C and clear interrupt
	while  (!(TWCR & (1<<TWINT)));		// Wait for I2C Transfer Completion
	
	LCD_I2C_Send_Latch();
}

void LCD_I2C_Send_Command(uint8_t command)
{
	TWDR &= ~(1<<LCD_I2C_RS_PIN);		// RS = 0. Selecting register as command register
	TWCR = (1<<TWINT) | (1<<TWEN);		// Enable I2C and clear interrupt
	while (!(TWCR & (1<<TWINT)));
	
	TWDR &= 0x0F;						// Clearing the Higher 4 bits
	TWCR = (1<<TWINT) | (1<<TWEN);		// Enable I2C and clear interrupt
	while (!(TWCR & (1<<TWINT)));		// Wait for I2C Transfer Completion
	
	TWDR |= (command & 0xF0);			// Masking higher 4 bits and sending to LCD
	TWCR = (1<<TWINT) | (1<<TWEN);		// Enable I2C and clear interrupt
	while (!(TWCR & (1<<TWINT)));		// Wait for I2C Transfer Completion
	
	LCD_I2C_Send_Latch();
	
	TWDR &= 0x0F;						// Clearing the Higher 4 bits
	TWCR = (1<<TWINT) | (1<<TWEN);		// Enable I2C and clear interrupt
	while (!(TWCR & (1<<TWINT)));		// Wait for I2C Transfer Completion
	
	TWDR |= ((command & 0x0F) << 4);	// Masking lower 4 bits and sending to LCD
	TWCR = (1<<TWINT) | (1<<TWEN);		// Enable I2C and clear interrupt
	while (!(TWCR & (1<<TWINT)));		// Wait for I2C Transfer Completion
	
	LCD_I2C_Send_Latch();
}

void LCD_I2C_Send_Data(uint8_t data)
{
	TWDR |= (1<<LCD_I2C_RS_PIN);	// RS = 1. Selecting register as data register
	TWCR = (1<<TWINT) | (1<<TWEN);	// Enable I2C and clear interrupt
	while (!(TWCR & (1<<TWINT)));   // Wait for I2C Transfer Completion
	
	TWDR &= 0x0F;				    // clearing the Higher 4 bits
	TWCR = (1<<TWINT) | (1<<TWEN);	// Enable I2C and clear interrupt
	while (!(TWCR & (1<<TWINT)));   // Wait for I2C Transfer Completion
	
	TWDR |= (data & 0xF0);			// Masking higher 4 bits and sending to LCD
	TWCR = (1<<TWINT) | (1<<TWEN);	// Enable I2C and clear interrupt
	while (!(TWCR & (1<<TWINT)));   // Wait for I2C Transfer Completion
	
	LCD_I2C_Send_Latch();
	
	TWDR &= 0x0F;					// clearing the Higher 4 bits
	TWCR = (1<<TWINT) | (1<<TWEN);	// Enable I2C and clear interrupt
	while (!(TWCR & (1<<TWINT)));   // Wait for I2C Transfer Completion
	
	TWDR |= ((data & 0x0F) << 4);	// Masking lower 4 bits and sending to LCD
	TWCR = (1<<TWINT) | (1<<TWEN);	// Enable I2C and clear interrupt
	while (!(TWCR & (1<<TWINT)));   // Wait for I2C Transfer Completion
	
	LCD_I2C_Send_Latch();
}


// Command for right Shift
void LCD_I2C_Cursor_Shift_Right()
{
	LCD_I2C_Send_Command(0x1C);      
	_delay_ms(200);
}

// Command for Left Shift
void LCD_I2C_Cursor_Shift_Left()
{
	LCD_I2C_Send_Command(0x18);      
	_delay_ms(200);
}

// Write Character to LCD
void LCD_I2C_Putchar(char c)
{
	LCD_I2C_Send_Data(c);
}

// Write Message to LCD
void LCD_I2C_Put_Message(const char* s)
{
	while (*s)
	{
		LCD_I2C_Putchar(*s);
		s++;
	}
}

// Write String to LCD
void LCD_I2C_Puts(unsigned char* s)
{
	while (*s)
	{
		LCD_I2C_Putchar(*s);
		s++;
	}
}

// Specify Cursor Position
void LCD_I2C_Set_Cursor(unsigned char line, unsigned char column)
{
	// Line 1-> 4
	// Column 1-> 16
	unsigned char address = 0;
	switch (line)
	{
		case 1:
			address = 0x80 + (column - 1);
			break;

		case 2:
			address = 0xC0 + (column - 1);
			break;
		
		case 3:
			address = 0x94 + (column - 1);
			break;
		
		case 4:
			address = 0xD4 + (column - 1);
			break;
	}
	LCD_I2C_Send_Command(address);		// Move Cursor
}

// Clear LCD
void LCD_I2C_Clear()
{
	LCD_I2C_Send_Command(0x01);			// Clear LCD
	_delay_us(4000);					// Pause
}

#endif

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/