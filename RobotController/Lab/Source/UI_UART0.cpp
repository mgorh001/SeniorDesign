/*************************************************/
/*                UI_UART0.cpp                   */
/*                                               */
/* This module handles the LCD user interface.   */
/* No hardware connectivity for this module.     */
/* No hardware resources required as well.       */ 
/* This software driver requires:                */
/* 1) UART0.cpp                                  */
/*                                               */
/*************************************************/

#ifndef UI_UART0_CPP
#define UI_UART0_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include "UART0.cpp"

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
void UI_UART0_Display_Homepage();
void UI_UART0_Display_Sensors_Values();
void UI_UART0_Display_Bad_Command();
void UI_UART0_Display_Command_1();
void UI_UART0_Display_Command_2();
void UI_UART0_Display_Distance(unsigned int);
void UI_UART0_Display_Distances(unsigned int, unsigned int);
void UI_UART0_Display_All_Distances(unsigned int, unsigned int, unsigned int);

/*************************************************/
/*                Implementation                 */
/*************************************************/

// Display Homepage
void UI_UART0_Display_Homepage()
{
	UART0_Put_Message("Robot Control!\n");
	UART0_Put_Message("Senior Project\n");
}

// Display a Two Digits Number over UART0
void UI_UART0_Display_Number_2D(unsigned char number)
{
	if (number > 99)
		return;
	
	unsigned char ten, unit;

	ten = (number  / 10) % 10;
	unit = (number - ten * 10);
	
	UART0_Putchar(DEC_TO_ASCII[ten]);
	UART0_Putchar(DEC_TO_ASCII[unit]);
}

// Display a Three Digits Number over UART0
void UI_UART0_Display_Number_3D(unsigned int number)
{
	if (number > 999)
		return;
	
	unsigned char hundred, ten, unit;

	hundred = (number / 100) % 10;
	ten = ((number - hundred * 100) / 10) % 10;
	unit = (number - hundred * 100 - ten * 10);
	
	UART0_Putchar(DEC_TO_ASCII[hundred]);
	UART0_Putchar(DEC_TO_ASCII[ten]);
	UART0_Putchar(DEC_TO_ASCII[unit]);
}

// Display Distance (cm)
void UI_UART0_Display_Distance(unsigned int distanceValue)
{
	UART0_Put_Message("D=");
	UI_UART0_Display_Number_3D(distanceValue);
	UART0_Put_Message("cm\n");
}

// Display Distances (cm) for two Ultrasonic Modules
void UI_UART0_Display_Distances(unsigned int distanceValue1, unsigned int distanceValue2)
{
	UART0_Put_Message("D1=");
	UI_UART0_Display_Number_3D(distanceValue1);
	UART0_Put_Message("cm\n");

	UART0_Put_Message("D2=");
	UI_UART0_Display_Number_3D(distanceValue2);
	UART0_Put_Message("cm\n");
}

// Display Distances (cm) for one Range Finder and two Ultrasonic Modules
void UI_UART0_Display_All_Distances(unsigned int distanceValueSR, unsigned int distanceValue1, unsigned int distanceValue2)
{
	UART0_Put_Message("SR=");
	UI_UART0_Display_Number_3D(distanceValueSR);
	UART0_Put_Message("cm\n");
	
	UART0_Put_Message("D1=");
	UI_UART0_Display_Number_3D(distanceValue1);
	UART0_Put_Message("cm\n");

	UART0_Put_Message("D2=");
	UI_UART0_Display_Number_3D(distanceValue2);
	UART0_Put_Message("cm\n");
}

// Display Steps
void UI_UART0_Display_Steps(unsigned char Steps)
{
	UART0_Put_Message("Steps=");
	UI_UART0_Display_Number_3D(Steps);
	UART0_Put_Message("cm\n");
}

#endif

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/