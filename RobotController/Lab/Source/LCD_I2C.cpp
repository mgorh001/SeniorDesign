/*************************************************/
/*                 LCD_I2C.cpp                   */
/*                                               */
/* This module handles the LCD module,           */
/* When using an I2C output expander.            */
/* It requires an I2C module.					 */
/* This software driver requires:                */
/* 1) io.h                                       */
/* 2) delay.h                                    */
/* 3) System_Hardware.h                          */
/*   (For External Hardware Definitions)         */
/*                                               */
/*************************************************/

#ifndef LCD_I2C_CPP
#define LCD_I2C_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include <util/delay.h>
#include "System_Hardware.h"
#include "I2C.cpp"

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// LCD IO Expander Pins
#define LCD_I2C_RS_PIN	0
#define LCD_I2C_RW_PIN	1
#define LCD_I2C_EN_PIN	2
#define LCD_I2C_BL_PIN	3
#define LCD_I2C_D4_PIN	4
#define LCD_I2C_D5_PIN	5
#define LCD_I2C_D6_PIN	6
#define LCD_I2C_D7_PIN	7

// LCD Ports
// ...

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Defines
#define EXP_ADDRESS		0x4E		// Hardware
//#define EXP_ADDRESS		0x70	// Simulator

// Declaration of Constants
// ...

// Declaration of Global Variables
unsigned char Exp_Reg;

// Declaration of Functions
void LCD_I2C_Init();
void LCD_I2C_Assert_RW_Low();
void LCD_I2C_Assert_RW_High();
void LCD_I2C_Send_Latch();
void LCD_I2C_Send_Command(uint8_t);
void LCD_I2C_Send_Command_Half(uint8_t);
void LCD_I2C_Send_Data(uint8_t);
void LCD_I2C_Clear(void);
void LCD_I2C_Set_Cursor(uint8_t, uint8_t);
void LCD_I2C_Putchar(uint8_t);
void LCD_I2C_Puts(char*);
void LCD_I2C_Put_Message(const char* s);
void LCD_I2C_Cursor_Shift_Right();
void LCD_I2C_Cursor_Shift_Left();
void LCD_I2C_Backlight_On();
void LCD_I2C_Backlight_Off();

/*************************************************/
/*                Implementation                 */
/*************************************************/

// Write to Expander
void I2C_Expander_Write(uint8_t data)
{
	I2C_Start();
	I2C_Write(EXP_ADDRESS);
	I2C_Write(data);
	I2C_Stop();
}

// Make RW Low
void LCD_I2C_Assert_RW_Low()
{
	Exp_Reg &= ~(1<<LCD_I2C_RW_PIN);	// PIN LCD_RW = 0.
	I2C_Expander_Write(Exp_Reg);
}

// Make RW High
void LCD_I2C_Assert_RW_High()
{
	Exp_Reg |= (1<<LCD_I2C_RW_PIN);		// PIN LCD_RW = 1.
	I2C_Expander_Write(Exp_Reg);
}

// Initialize LCD
void LCD_I2C_Init()
{
	_delay_ms(50); // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	LCD_I2C_Assert_RW_Low();			// Write mode only!
	// Sequence for initializing LCD in 4-Bit mode
	LCD_I2C_Send_Command_Half(0x30);	// First Try
	_delay_us(4500);					// Min. 4.1ms
	LCD_I2C_Send_Command_Half(0x30);	// Second Try
	_delay_us(4500);					// Min. 4.1ms
	LCD_I2C_Send_Command_Half(0x30);	// Third Try
	_delay_us(150);						// Wait
	LCD_I2C_Send_Command_Half(0x20);	// Set it to 4-Bit Finally
	LCD_I2C_Send_Command(0x28);			// Selecting 16 x 2 LCD in 4Bit mode
	LCD_I2C_Send_Command(0x0C);			// Display ON Cursor OFF
	LCD_I2C_Send_Command(0x01);			// Clear display
	LCD_I2C_Send_Command(0x06);			// Cursor Auto Increment
	LCD_I2C_Send_Command(0x80);			// 1st line 1st location of LCD
}

// Latching data into LCD data register using High to Low signal on the LCD_I2C_EN_PIN
void LCD_I2C_Send_Latch()
{
	Exp_Reg |= (1<<LCD_I2C_EN_PIN);		// PIN LCD_EN = 1.
	I2C_Expander_Write(Exp_Reg);
	_delay_us(1);						// Enable pulse must be > 450ns
	
	Exp_Reg &= ~(1<<LCD_I2C_EN_PIN);	// PIN LCD_EN = 0.
	I2C_Expander_Write(Exp_Reg);
	_delay_us(50);						// Commands need > 37us to settle
}

// Send Half Command to LCD
void LCD_I2C_Send_Command_Half(uint8_t command)
{
	Exp_Reg &= ~(1<<LCD_I2C_RS_PIN);	// PIN LCD_RS = 0. Selecting register as Command register
	Exp_Reg &= 0x0F;					// Clearing the Higher 4 bits
	Exp_Reg |= (command & 0xF0);		// Masking higher 4 bits and sending to LCD
	I2C_Expander_Write(Exp_Reg);
	LCD_I2C_Send_Latch();
}

// Send Command to LCD
void LCD_I2C_Send_Command(uint8_t command)
{
	Exp_Reg &= ~(1<<LCD_I2C_RS_PIN);	// RS = 0. Selecting register as command register
	Exp_Reg &= 0x0F;					// Clearing the Higher 4 bits
	Exp_Reg |= (command & 0xF0);		// Masking higher 4 bits and sending to LCD
	I2C_Expander_Write(Exp_Reg);
	LCD_I2C_Send_Latch();
	
	Exp_Reg &= ~(1<<LCD_I2C_RS_PIN);	// RS = 0. Selecting register as command register
	Exp_Reg &= 0x0F;					// Clearing the Higher 4 bits
	Exp_Reg |= ((command & 0x0F) << 4);	// Masking lower 4 bits and sending to LCD
	I2C_Expander_Write(Exp_Reg);
	LCD_I2C_Send_Latch();
}

// Send Data to LCD
void LCD_I2C_Send_Data(uint8_t data)
{
	Exp_Reg |= (1<<LCD_I2C_RS_PIN);		// RS = 1. Selecting register as data register
	Exp_Reg &= 0x0F;				    // clearing the Higher 4 bits
	Exp_Reg |= (data & 0xF0);			// Masking higher 4 bits and sending to LCD
	I2C_Expander_Write(Exp_Reg);
	LCD_I2C_Send_Latch();
	
	Exp_Reg |= (1<<LCD_I2C_RS_PIN);		// RS = 1. Selecting register as data register
	Exp_Reg &= 0x0F;					// clearing the Higher 4 bits
	Exp_Reg |= ((data & 0x0F) << 4);	// Masking lower 4 bits and sending to LCD
	I2C_Expander_Write(Exp_Reg);
	LCD_I2C_Send_Latch();
}

void LCD_I2C_Backlight_On()
{
	Exp_Reg |= (1<<LCD_I2C_BL_PIN);		// BL = 1. Backlight ON
	I2C_Expander_Write(Exp_Reg);
}


void LCD_I2C_Backlight_Off()
{
	Exp_Reg &= ~(1<<LCD_I2C_BL_PIN);	// BL = 0. Backlight OFF
	I2C_Expander_Write(Exp_Reg);
}

// Shift Right LCD Cursor
void LCD_I2C_Cursor_Shift_Right()
{
	LCD_I2C_Send_Command(0x1C);
	_delay_ms(200);
}

// Shift Left LCD Cursor
void LCD_I2C_Cursor_Shift_Left()
{
	LCD_I2C_Send_Command(0x18);
	_delay_ms(200);
}

// Write Character to LCD
void LCD_I2C_Putchar(uint8_t c)
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
	_delay_ms(4);						// Pause
}

#endif

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/