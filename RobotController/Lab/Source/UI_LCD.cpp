/*************************************************/
/*                 UI_LCD.cpp                    */
/*                                               */
/* This module handles the LCD user interface.   */
/* No hardware connectivity for this module.     */
/* No hardware resources required as well.       */ 
/* This software driver requires:                */
/* 1) LCD_I2C.cpp                                */
/*                                               */
/*************************************************/

#ifndef UI_LCD_CPP
#define UI_LCD_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include "LCD_I2C.cpp"

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// Hardware Connectivity
// ...

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Declaration of Constants
// ...

// Declaration of Global Variables
// ...

// Declaration of Functions
void UI_LCD_I2C_Display_Homepage();
void UI_LCD_I2C_Display_Number_3D(unsigned int);
void UI_LCD_I2C_Display_Sonar_Value(unsigned int);
void UI_LCD_I2C_Display_UM1_Value(unsigned int);
void UI_LCD_I2C_Display_UM2_Value(unsigned int);
void UI_LCD_I2C_Display_Camera_Command(unsigned int);
void UI_LCD_I2C_Display_Distance(unsigned int);
void UI_LCD_I2C_Display_Distances(unsigned int, unsigned int);
void UI_LCD_I2C_Display_Command_1();
void UI_LCD_I2C_Display_Command_2();
void UI_LCD_I2C_Display_Bad_Command();

/*************************************************/
/*                Implementation                 */
/*************************************************/

// Display Homepage
void UI_LCD_I2C_Display_Homepage()
{
	LCD_I2C_Clear();
	LCD_I2C_Set_Cursor(1, 1);
	LCD_I2C_Put_Message(" Robot Control  ");
	LCD_I2C_Set_Cursor(2, 1);
	LCD_I2C_Put_Message(" Senior Project ");
}

// Display Number
void UI_LCD_I2C_Display_Number_3D(unsigned int number)
{
	if (number > 999)
		return;
	
	unsigned char hundred, ten, unit;

	hundred = (number / 100) % 10;
	ten = ((number - hundred * 100) / 10) % 10;
	unit = (number - hundred * 100 - ten * 10);
	
	LCD_I2C_Putchar(DEC_TO_ASCII[hundred]);
	LCD_I2C_Putchar(DEC_TO_ASCII[ten]);
	LCD_I2C_Putchar(DEC_TO_ASCII[unit]);
}

// Display Sonar Distance
void UI_LCD_I2C_Display_Sonar_Value(unsigned int distanceValue)
{
	LCD_I2C_Set_Cursor(1, 1);
	LCD_I2C_Put_Message("D=");
	UI_LCD_I2C_Display_Number_3D(distanceValue);
	LCD_I2C_Put_Message("cm");
}

void UI_LCD_I2C_Display_UM1_Value(unsigned int distanceValue)
{
	LCD_I2C_Set_Cursor(2, 1);
	LCD_I2C_Put_Message("D1=");
	UI_LCD_I2C_Display_Number_3D(distanceValue);
	LCD_I2C_Put_Message("cm");
}

void UI_LCD_I2C_Display_UM2_Value(unsigned int distanceValue)
{
	LCD_I2C_Set_Cursor(2, 9);
	LCD_I2C_Put_Message("D2=");
	UI_LCD_I2C_Display_Number_3D(distanceValue);
	LCD_I2C_Put_Message("cm");
}

void UI_LCD_I2C_Display_Camera_Command(unsigned int commnad)
{
	LCD_I2C_Set_Cursor(1, 9);
	LCD_I2C_Puts(LUT_STRING[currCameraCMD]);
}

// Display Distance (cm)
void UI_LCD_I2C_Display_Distance(unsigned int distanceValue)
{
	LCD_I2C_Set_Cursor(1, 1);
	LCD_I2C_Put_Message("D=");
	UI_LCD_I2C_Display_Number_3D(distanceValue);
	LCD_I2C_Put_Message("cm");
}

// Display Distances (cm) for two Ultrasonic Modules
void UI_LCD_I2C_Display_Distances(unsigned int distanceValue1, unsigned int distanceValue2)
{
	LCD_I2C_Set_Cursor(2, 1);
	LCD_I2C_Put_Message("D1=");
	UI_LCD_I2C_Display_Number_3D(distanceValue1);
	LCD_I2C_Put_Message("cm");
	
	LCD_I2C_Set_Cursor(2, 9);
	LCD_I2C_Put_Message("D2=");
	UI_LCD_I2C_Display_Number_3D(distanceValue2);
	LCD_I2C_Put_Message("cm");
}

// Display Distances (cm) for one Range Finder and two Ultrasonic Modules
void UI_LCD_I2C_Display_All_Distances(unsigned int distanceValueSR, unsigned int distanceValue1, unsigned int distanceValue2)
{
	LCD_I2C_Set_Cursor(1, 1);
	LCD_I2C_Put_Message("D=");
	UI_LCD_I2C_Display_Number_3D(distanceValueSR);
	LCD_I2C_Put_Message("cm");	
	
	LCD_I2C_Set_Cursor(2, 1);
	LCD_I2C_Put_Message("D1=");
	UI_LCD_I2C_Display_Number_3D(distanceValue1);
	LCD_I2C_Put_Message("cm");
	
	LCD_I2C_Set_Cursor(2, 9);
	LCD_I2C_Put_Message("D2=");
	UI_LCD_I2C_Display_Number_3D(distanceValue2);
	LCD_I2C_Put_Message("cm");
}

// Display Bad Command Received
void UI_LCD_I2C_Display_Bad_Command()
{
	LCD_I2C_Clear();
	LCD_I2C_Set_Cursor(1, 1);
	LCD_I2C_Put_Message(" Bad Command    ");
	LCD_I2C_Set_Cursor(2, 1);
	LCD_I2C_Put_Message(" Received       ");
}

// Display Command 1 Received
void UI_LCD_I2C_Display_Command_1()
{
	LCD_I2C_Clear();
	LCD_I2C_Set_Cursor(1, 1);
	LCD_I2C_Put_Message("  Command '1'   ");
	LCD_I2C_Set_Cursor(2, 1);
	LCD_I2C_Put_Message("   Received     ");
}

// Display Command 2 Received
void UI_LCD_I2C_Display_Command_2()
{
	LCD_I2C_Clear();
	LCD_I2C_Set_Cursor(1, 1);
	LCD_I2C_Put_Message("  Command '2'   ");
	LCD_I2C_Set_Cursor(2, 1);
	LCD_I2C_Put_Message("   Received     ");
}

#endif

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/