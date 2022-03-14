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

#ifndef LCD4B_CPP
#define LCD4B_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include <util/delay.h>
#include "System_Hardware.h"

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// LCD_RW is it Used? (1 for Used, 0 for not Used)
#define USE_LCD_RW		SYS_USE_LCD_RW
#define USE_UN_LCD_PORT SYS_USE_UN_LCD_PORT

// LCD Pins
#define LCD_EN_PIN		SYS_LCD_EN_PIN
#define LCD_RS_PIN		SYS_LCD_RS_PIN
#if (USE_LCD_RW)
#define LCD_RW_PIN		SYS_LCD_RW_PIN
#endif // USE_LCD_RW

// LCD Ports
#define LCD_DATA_PORT	SYS_LCD_DATA_PORT
#define LCD_DATA_DIR	SYS_LCD_DATA_DIR
#define LCD_CMD_PORT	SYS_LCD_CMD_PORT
#define LCD_CMD_DIR		SYS_LCD_CMD_DIR

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Declaration of Constants
// ...

// Declaration of Global Variables
// ...

// Declaration of Functions
void LCD_Init(void);
void LCD_Send_Command(unsigned char);
void LCD_Send_Data(unsigned char);
void LCD_Clear(void);
void LCD_Set_Cursor(unsigned char, unsigned char);
void LCD_Putchar(char);
void LCD_Puts(unsigned char*);

/*************************************************/
/*                Implementation                 */
/*************************************************/

// Initialize LCD
void LCD_Init(void)
{
	LCD_CMD_DIR |= (1 << LCD_RS_PIN);
	LCD_CMD_DIR |= (1 << LCD_EN_PIN);
	#if (USE_LCD_RW)
	LCD_CMD_DIR |= (1 << LCD_RW_PIN);
	#endif // USE_LCD_RW
	
	#if (USE_UN_LCD_PORT)
	LCD_DATA_DIR |= 0xF0;				// Higher Port Pins are Outputs
	#else
	LCD_DATA_DIR |= 0x0F;				// Higher Port Pins are Outputs
	#endif // (USE_UN_LCD_PORT)
	LCD_Send_Command(0x33);				// Send 0x33 for Initialization
	_delay_ms(1);
	LCD_Send_Command(0x32);				// Send 0x32 for Initialization
	_delay_ms(1);
	LCD_Send_Command(0x28);				// Initialize LCD: 2 Lines, 5x8 Matrix
	LCD_Send_Command(0x0E);				// Display ON, Cursor ON	
	LCD_Send_Command(0x01);				// Clear LCD
	_delay_ms(4);						// Pause
	LCD_Send_Command(0x06);				// Shift cursor to the right (Increment Cursor)
	_delay_ms(4);						// Pause
}

void LCD_Send_Command(unsigned char cmnd)
{
	LCD_CMD_PORT &= ~(1 << LCD_RS_PIN);	// Select command register: RS=0
	#if (USE_LCD_RW)
	LCD_CMD_PORT &= ~(1 << LCD_RW_PIN);	// Display type: writing (R/W = 0)
	#endif // USE_LCD_RW
	
	LCD_CMD_PORT |=  (1 << LCD_EN_PIN);	// Prepare&Write: strobe front EN=1
	#if (USE_UN_LCD_PORT)
	LCD_DATA_PORT = (LCD_DATA_PORT & 0X0F) | (cmnd & 0xF0);
	#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0XF0) | (cmnd >> 4);
	#endif
	_delay_us(1);					    // Pause
	LCD_CMD_PORT &= ~(1 << LCD_EN_PIN);	// Write: Strobe front EN=1=>0
	_delay_us(400);					    // Pause
	
	LCD_CMD_PORT |=  (1 << LCD_EN_PIN);	// Prepare&Write: strobe front EN=1
	#if (USE_UN_LCD_PORT)
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (cmnd << 4);
	#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0XF0) | (cmnd & 0x0F);
	#endif
	_delay_us(1);					    // Pause
	LCD_CMD_PORT &= ~(1 << LCD_EN_PIN);	// Write: Strobe front EN=1=>0
	_delay_us(400);					    // Pause
}

// Sending data to LCD subroutine
void LCD_Send_Data(unsigned char data)
{
	LCD_CMD_PORT |=  (1 << LCD_RS_PIN);	// Select data register: RS=1
	#if (USE_LCD_RW)
	LCD_CMD_PORT &= ~(1 << LCD_RW_PIN);	// Display type: writing (R/W = 0)
	#endif // USE_LCD_RW
	
	LCD_CMD_PORT |=  (1 << LCD_EN_PIN);	// Prepare&Write: strobe front EN=1
	
	#if (USE_UN_LCD_PORT)
	LCD_DATA_PORT = (LCD_DATA_PORT & 0X0F) | (data & 0xF0);
	#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0XF0) | (data >> 4);
	#endif
	
	_delay_us(1);					    // Pause
	LCD_CMD_PORT &= ~(1 << LCD_EN_PIN);	// Write: Strobe front EN=1=>0
	_delay_us(400);					    // Pause
	
	LCD_CMD_PORT |=  (1 << LCD_EN_PIN);	// Prepare&Write: strobe front EN=1
	#if (USE_UN_LCD_PORT)
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data << 4);
	#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0XF0) | (data & 0x0F);
	#endif
	
	_delay_us(1);					    // Pause
	LCD_CMD_PORT &= ~(1 << LCD_EN_PIN);	// Write: Strobe front EN=1=>0
	_delay_us(400);					    // Pause
}

// Write Character to LCD
void LCD_Putchar(char c)
{
	LCD_Send_Data(c);
}

// Write String to LCD
void LCD_Puts(unsigned char* s)
{
	while (*s)
	{
		LCD_Putchar(*s);
		s++;
	}
}

// Specify Cursor Position
void LCD_Set_Cursor(unsigned char line, unsigned char column)
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
	LCD_Send_Command(address);		// Move Cursor
}

// Clear LCD
void LCD_Clear()
{
	LCD_Send_Command(0x01); // Clear LCD
	_delay_us(4000);		// Pause
}

#endif

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/