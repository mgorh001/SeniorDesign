/*************************************************/
/*              System_Hardware.h                */
/*                                               */
/* This module defines hardware connectivity.    */
/* No software dependency.   	                 */
/* Modify here the definitions of hardware.      */
/*                                               */
/*************************************************/

#define ENABLE	 1
#define DISABLE  0

/*************************************************/
/*           PWM0 Module Connections             */
/*************************************************/

// PWM0 Usage
#define SYS_PWM0A			ENABLE
#define SYS_PWM0B			DISABLE

// PWM0 Module Pins
#define SYS_PWM0A_PIN		PIND6
#define SYS_PWM0B_PIN		PIND5

// PWM0 Module Ports
#define SYS_PWM0A_PORT		PORTD
#define SYS_PWM0A_DIR		DDRD
#define SYS_PWM0A_PORT		PORTD
#define SYS_PWM0A_DIR		DDRD

/*************************************************/
/*       L293D Motors' Driver Connections        */
/*************************************************/

// H-Bridges Usage
#define SYS_USE_H_BRIDGE_1  ENABLE
#define SYS_USE_H_BRIDGE_2  DISABLE

#if (SYS_USE_H_BRIDGE_1)
// L293D H Bridge 1 Pins
#define SYS_L293D_IN1_PIN	PINB0
#define SYS_L293D_IN2_PIN	PINB1
#define SYS_L293D_EN1_PIN	PIND6

// L293D H Bridge 1 Ports
#define SYS_L293D_IN1_PORT	PORTB
#define SYS_L293D_IN1_DIR	DDRB
#define SYS_L293D_IN2_PORT	PORTB
#define SYS_L293D_IN2_DIR	DDRB
#define SYS_L293D_EN1_PORT	PORTD
#define SYS_L293D_EN1_DIR	DDRD
#endif // (SYS_USE_H_BRIDGE_1)

#if (SYS_USE_H_BRIDGE_2)
// L293D H Bridge 2 Pins
#define SYS_L293D_IN3_PIN	
#define SYS_L293D_IN4_PIN	
#define SYS_L293D_EN2_PIN	

// L293D H Bridge 2 Ports
#define SYS_L293D_IN3_PORT	
#define SYS_L293D_IN3_DIR	
#define SYS_L293D_IN4_PORT	
#define SYS_L293D_IN4_DIR	
#define SYS_L293D_EN2_PORT	
#define SYS_L293D_EN2_DIR	
#endif // (SYS_USE_H_BRIDGE_2)

/*************************************************/
/*            LCD Module Connections             */
/*************************************************/
/*
// LCD_RW
#define SYS_USE_LCD_RW		DISABLE

// Upper Nibble Usage of the LCD Port
#define SYS_USE_UN_LCD_PORT	DISABLE

// LCD Module Pins
#define SYS_LCD_EN_PIN		PINC4
#define SYS_LCD_RS_PIN		PINC5
#if (SYS_USE_LCD_RW)
#define SYS_LCD_RW_PIN		
#endif // (SYS_USE_LCD_RW)

// LCD Module Ports
#define SYS_LCD_DATA_PORT	PORTC
#define SYS_LCD_DATA_DIR	DDRC
#define SYS_LCD_CMD_PORT	PORTC
#define SYS_LCD_CMD_DIR		DDRC
*/
/*************************************************/
/*       Ultrasonic Modules' Connections         */
/*************************************************/

// UM Usage
#define SYS_UM1				ENABLE
#define SYS_UM2				ENABLE

#if (SYS_UM1)
// UM1 Pins
#define SYS_UM1_TRIG_PIN	PINC0
#define SYS_UM1_ECHO_PIN 	PINC1
// UM1 Ports
#define SYS_UM1_TRIG_PORT	PORTC
#define SYS_UM1_TRIG_DIR	DDRC
#define SYS_UM1_ECHO_PORT	PORTC
#define SYS_UM1_ECHO_DIR	DDRC
#define SYS_UM1_ECHO_PRTIN	PINC
#define SYS_UM1_ECHO_PCIE	PCIE1
#define SYS_UM1_ECHO_PCINT	PCINT9
#endif // (SYS_UM1)

#if (SYS_UM2)
// UM2 Pins
#define SYS_UM2_TRIG_PIN	PIND7
#define SYS_UM2_ECHO_PIN	PIND5
// UM2 Ports
#define SYS_UM2_TRIG_PORT	PORTD
#define SYS_UM2_TRIG_DIR	DDRD
#define SYS_UM2_ECHO_PORT	PORTD
#define SYS_UM2_ECHO_DIR	DDRD
#define SYS_UM2_ECHO_PRTIN	PIND
#define SYS_UM2_ECHO_PCIE	PCIE2
#define SYS_UM2_ECHO_PCINT	PCINT21
#endif // (SYS_UM2)

/*************************************************/
/*           Servo Motor Connection              */
/*************************************************/

// Servo Pin
#define SYS_SERVO_PIN		PIND3

// Servo Port
#define SYS_SERVO_PORT		PORTD
#define SYS_SERVO_DIR		DDRD

/*************************************************/
/*          Sonar Module Connection              */
/*************************************************/

// Sonar Pin
#define SYS_SONAR_AN_PIN	PINC6	// ADC6

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/