#define F_CPU		16000000UL
//#define F_CPU		8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include "Source/I2C.cpp"
#include "Source/LCD_I2C.cpp"	

// Defines

// Declaration of Variables
unsigned char Temperature = 47;

// Declaration of Look-Up Tables
// DEC_TO_ASCII: look-up table that converts a decimal number (0->15) to its hex form (0->9, A->F)
unsigned char DEC_TO_ASCII[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

// Declaration of Functions
void UI_Display_Homepage();
void UI_Display_Temperature();

int main( void )
{
	I2C_Init();
	I2C_Start();
	I2C_Write(0x70);
	LCD_I2C_Init();
	//UI_Display_Homepage();

	LCD_I2C_Set_Cursor(1, 1);
	LCD_I2C_Put_Message("LCD I2C Master ");
	LCD_I2C_Set_Cursor(2, 3);
	LCD_I2C_Put_Message("Simple Testing");
	
	_delay_ms(500);
	while(1)	{		UI_Display_Temperature();		_delay_ms(1000);			}
	return 0;
}

// Display Homepage
void UI_Display_Homepage()
{
	unsigned char messageTitle1[] =	" Robot  Control ";
	unsigned char messageTitle2[] =	"Testing  I2C LCD";
	LCD_I2C_Clear();
	LCD_I2C_Set_Cursor(1, 1);
	LCD_I2C_Puts(messageTitle1);
	LCD_I2C_Set_Cursor(2, 1);
	LCD_I2C_Puts(messageTitle2);
}

// Display Temperature ( 0 < T < 99 )
void UI_Display_Temperature()
{
	unsigned char messageS1[] = "  Temperature   ";
	unsigned char messageS2[] = "  T=";
	
	LCD_I2C_Clear();
	LCD_I2C_Set_Cursor(1, 1);
	LCD_I2C_Puts(messageS1);
	LCD_I2C_Set_Cursor(2, 3);
	LCD_I2C_Puts(messageS2);
	LCD_I2C_Putchar(DEC_TO_ASCII[Temperature / 10]);
	LCD_I2C_Putchar(DEC_TO_ASCII[Temperature % 10]);
}

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/