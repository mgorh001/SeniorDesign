/*************************************************/
/*           General System Defines              */
/*************************************************/

// CPU Defines
#ifndef F_CPU
#define F_CPU			(16000000UL)
#endif

// UART Defines
#define BAUD			(9600)
#define MYUBRR			(F_CPU/16/BAUD-1)

// Servo Defines
#define SERVO_LT_MOST	(0)#define SERVO_LT_HALF	(20)#define SERVO_CENTER	(40)#define SERVO_RT_HALF	(60)#define SERVO_RT_MOST	(80)

// Camera Defines
#define CMD_LT_MOST		(0)
#define CMD_LT_HALF		(1)
#define CMD_CENTER		(2)
#define CMD_RT_HALF		(3)
#define CMD_RT_MOST		(4)
#define CMD_INVALID		(5)
#define CMD_STOPPED		(6)

// Other Defines
#define COUNT_MAX		(5)

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// Digital Pins
#define BUTTON_PIN		PIND2
#define LED1_PIN 		PINC2
#define LED2_PIN 		PINC3
#define CAM0_PIN 		PINB2
#define CAM1_PIN 		PINB3
#define CAM2_PIN 		PINB4
#define CAM3_PIN 		PINB5

// Digital Ports
#define BUTTON_PORT		PORTD
#define BUTTON_PRTIN	PIND
#define BUTTON_DIR		DDRD
#define LED1_PORT		PORTC
#define LED1_DIR		DDRC
#define LED2_PORT		PORTC
#define LED2_DIR		DDRC
#define CAM0_DIR 		DDRB
#define CAM1_DIR 		DDRB
#define CAM2_DIR 		DDRB
#define CAM3_DIR 		DDRB
#define CAM0_PORT 		PORTB
#define CAM1_PORT 		PORTB
#define CAM2_PORT 		PORTB
#define CAM3_PORT 		PORTB
#define CAM0_PRTIN 		PINB
#define CAM1_PRTIN 		PINB
#define CAM2_PRTIN 		PINB
#define CAM3_PRTIN 		PINB

/*************************************************/
/*                   Macros                      */
/*************************************************/

// Lowest Layer Macros
#define _BV(bit)				(1 << (bit))
#define setBit(byte, bit)		(byte |=  _BV(bit))
#define clearBit(byte, bit)		(byte &= ~_BV(bit))
#define toggleBit(byte, bit)    (byte ^=  _BV(bit))
#define bit_is_set(sfr, bit)    (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr, bit)	(!(_SFR_BYTE(sfr) & _BV(bit)))
#define loop_until_bit_is_set(sfr, bit)   do { } while (bit_is_clear(sfr, bit))
#define loop_until_bit_is_clear(sfr, bit) do { } while (bit_is_set(sfr, bit))

// Highest Layer Macros
#define Button_Is_Pressed()		bit_is_clear (BUTTON_PORT, BUTTON_PIN)
#define Button_Is_Deressed()	bit_is_set (BUTTON_PORT, BUTTON_PIN)
#define Turn_On_LED1()			setBit (LED1_PORT, LED1_PIN)
#define Turn_Off_LED1()			clearBit (LED1_PORT, LED1_PIN)
#define Toggle_LED1()			toggleBit (LED1_PORT, LED1_PIN)
#define Turn_On_LED2()			setBit (LED2_PORT, LED2_PIN)
#define Turn_Off_LED2()			clearBit (LED2_PORT, LED2_PIN)
#define Toggle_LED2()			toggleBit (LED2_PORT, LED2_PIN)

/*************************************************/
/*                Declarations                   */
/*************************************************/

// Declaration of Global Variables
unsigned char enableMotorFlag = 1;
unsigned int distanceSR, distanceUM1, distanceUM2; 
unsigned long prevCameraCMD = 0xFF,currCameraCMD;   

// Declaration of Constants
// ...

//  Declaration of Main Functions
void Setup_Microcontroller();
void Misc_Init();
void Test_Speed();
void Test_Steering_Motor();
void Test_Traction_Motor();
void Test_LEDs();
void Remote_Control_Process_Data();
unsigned char Get_OpenMV_Camera_Command();

// Declaration of Look-Up Tables:

// LUT_TRACTION: look-up table that converts a decimal number (0->4) to a traction speed
unsigned char LUT_TRACTION[] =
{
	(25), // CMD_LT_MOST
	(40), // CMD_LT_HALF
	(80), // CMD_CENTER
	(40), // CMD_RT_HALF
	(25)  // CMD_RT_MOST
};

// LUT_SERVO: look-up table that converts a decimal number (0->4) to a servo angle
unsigned char LUT_SERVO[] =
{
	SERVO_LT_MOST,	SERVO_LT_HALF,	SERVO_CENTER,	SERVO_RT_HALF,	SERVO_RT_MOST
};

// LUT_STRING: look-up table that converts a decimal number (0->6) to a String
unsigned char LUT_STRING[7][8] =
{
	"LT_MOST",	"LT_HALF",
	"CENTER ",
	"RT_HALF",
	"RT_MOST",
	"INVALID",
	"STOPPED"
};

// DEC_TO_HEX: look-up table that converts a decimal number (0->15) to its hex form (0->9, A->F)
unsigned char DEC_TO_HEX[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

// DEC_TO_ASCII: look-up table that converts a decimal number (0->15) to its hex form (0->9, A->F)
unsigned char DEC_TO_ASCII[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

#include "Source/ADC.cpp" 
#include "Source/Sonar.cpp"
#include "Source/UM_PCINT.cpp"
#include "Source/PWM.cpp"
#include "Source/L293D.cpp"
#include "Source/Servo.cpp"
#include "Source/UART0.cpp"
#include "Source/UI_UART0.cpp"
#include "Source/I2C.cpp"
#include "Source/LCD_I2C.cpp"
#include "Source/UI_LCD.cpp"

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/