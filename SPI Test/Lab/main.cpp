//#define F_CPU		16000000UL
#define F_CPU		8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include "Source/LCD4B.cpp"
#include "Source/SPI.cpp"	

// Defines
// TC72 Registers
#define CNTL_W    0x80      // 0b1000_0000 => A7 = 1
#define CNTL_R    0x00      // 0b0000_0000 => A7 = 0
#define TEMP_LSB  0x01
#define TEMP_MSB  0x02
#define ID        0x03
// CS Connectivity
#define _CS_PIN			PINB2
#define _SPI_DIR		DDRB
#define _SPI_PORT		PORTB

// Declaration of Variables
unsigned char Temperature;

// Declaration of Look-Up Tables
// DEC_TO_ASCII: look-up table that converts a decimal number (0->15) to its hex form (0->9, A->F)
unsigned char DEC_TO_ASCII[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

// Declaration of Functions
void UI_Display_Homepage();
void UI_Display_Temperature();
void TC72_Init();
unsigned char TC72_Get_Temperature();

int main( void )
{
	SPI_Init();
	TC72_Init();
	LCD_Init();
	UI_Display_Homepage();
	_delay_ms(500);
	while(1)	{		Temperature = TC72_Get_Temperature();		 UI_Display_Temperature();		_delay_ms(1000);			}
	return 0;
}

void Make_CS_High()
{ 
	_SPI_PORT |=  (1<<_CS_PIN);
}

void Make_CS_Low() 
{
	_SPI_PORT &= ~(1<<_CS_PIN);
}

// TC72 Initialize
void TC72_Init()
{
	Make_CS_High();
	SPI_Master_Write_Register(CNTL_W, 0x00);
	Make_CS_Low();
}

// TC72 Get Temperature
unsigned char TC72_Get_Temperature()
{
	unsigned char _Temperature;
	Make_CS_High();
	SPI_Master_Transmit_Byte(TEMP_MSB);
	_delay_us(100);
	_Temperature = SPI_Master_Receive_Byte();
	Make_CS_Low();
	return _Temperature;
}

// Display Homepage
void UI_Display_Homepage()
{
	unsigned char messageTitle1[] =	" Robot  Control ";
	unsigned char messageTitle2[] =	"  Testing  SPI  ";
	LCD_Clear();
	LCD_Set_Cursor(1, 1);
	LCD_Puts(messageTitle1);
	LCD_Set_Cursor(2, 1);
	LCD_Puts(messageTitle2);
}

// Display Temperature ( 0 < T < 99 )
void UI_Display_Temperature()
{
	unsigned char messageS1[] = "  Temperature   ";
	unsigned char messageS2[] = "  T=";
	
	LCD_Clear();
	LCD_Set_Cursor(1, 1);
	LCD_Puts(messageS1);
	LCD_Set_Cursor(2, 3);
	LCD_Puts(messageS2);
	LCD_Putchar(DEC_TO_ASCII[Temperature / 10]);
	LCD_Putchar(DEC_TO_ASCII[Temperature % 10]);
}




/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/