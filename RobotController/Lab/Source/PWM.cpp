/*************************************************/
/*                   PWM.cpp                     */
/*                                               */
/* This module handles the PWM module.           */
/* PWM pin required as output pin.               */
/* It uses Timer0 module in the PWM mode.        */
/* This software driver requires:                */
/* 1) System_Hardware.h                          */ 
/*   (For External Hardware Definitions)         */
/*                                               */
/*************************************************/

#ifndef PWM_CPP
#define PWM_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include "System_Hardware.h"

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// PWM Usage (1 for Used, 0 for not Used)
#define PWM0A		SYS_PWM0A
#define PWM0B		SYS_PWM0B

#if (PWM0A)
// PWM0A Pin
#define PWM0A_PIN	SYS_PWM0A_PIN
// PWM0A Port
#define PWM0A_PORT	SYS_PWM0A_PORT
#define PWM0A_DIR	SYS_PWM0A_DIR
#endif // (PWM0A)

#if (PWM0B)
// PWM0B Pin
#define PWM0B_PIN	SYS_PWM0B_PIN
// PWM0B Port
#define PWM0B_PORT	SYS_PWM0A_PORT
#define PWM0B_DIR	SYS_PWM0A_DIR
#endif // (PWM0B)

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Declaration of Constants
const unsigned char DUTY_MAX = 255;

// Declaration of Global Variables
// ...

// Declaration of Functions
#if PWM0A
void PWM0A_Init(void);
void PWM0A_Set_Duty_Cycle(unsigned char);
void PWM0A_Set_Duty_Cycle_Percent(unsigned char);
#endif // PWM0A

#if PWM0B
void PWM0B_Init(void);
void PWM0B_Set_Duty_Cycle(unsigned char);
void PWM0B_Set_Duty_Cycle_Percent(unsigned char);
#endif // PWM0B

/*************************************************/
/*                Implementation                 */
/*************************************************/

#if PWM0A

// Initialize PWM Module
void PWM0A_Init()
{	
	TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00); // Non-Inverting Fast PWM mode 3 using OCR0A
	TCCR0B |= (1<<CS00);							 // No Prescalar
	
	PWM0A_DIR  |=  (1 << PWM0A_PIN);				 // Make PWM Pin Output
	PWM0A_PORT &= ~(1 << PWM0A_PIN);				 // Make PWM Pin Zero
	PWM0A_Set_Duty_Cycle_Percent(0);				 // Duty cycle of 0
}

// Set Duty Cycle for OCR0A
void PWM0A_Set_Duty_Cycle(unsigned char duty)
{
	OCR0A = duty;
}

// Set Duty Cycle Percent 0 -> 100
void PWM0A_Set_Duty_Cycle_Percent(unsigned char duty_percent)
{
	unsigned char duty; 
	duty = (duty_percent * DUTY_MAX) / 100;
	if (duty != 0)
	{
		TCCR0A |= (1<<COM0A1);							// Clear OC0A on compare match
		OCR0A = duty;
	}
	else
	{
		TCCR0A &= ~(1<<COM0A0);							// Normal Port Operation, OC0A Disconnected	
		TCCR0A &= ~(1<<COM0A1);							// Normal Port Operation, OC0A Disconnected				
		PWM0A_PORT &= ~(1 << PWM0A_PIN);				// Make PWM Pin Zero
	}
}

#endif // PWM0A

/*************************************************/

#if PWM0B

// Initialize PWM Module
void PWM0B_Init()
{
	TCCR0A |= (1<<COM0B1) | (1<<WGM01) | (1<<WGM00); // Non-Inverting Fast PWM mode 3 using OCR0B
	TCCR0B |= (1<<CS00);							 // No Prescalar
	
	PWM0B_DIR  |=  (1 << PWM0B_PIN);				 // Make PWM Pin Output
	PWM0B_PORT &= ~(1 << PWM0B_PIN);				 // Make PWM Pin Zero
	PWM0A_Set_Duty_Cycle_Percent(0);				 // Duty cycle of 0
}

// Set Duty Cycle for OCR0B
void PWM0B_Set_Duty_Cycle(unsigned char duty)
{
	OCR0B = duty;
}

// Set Duty Cycle Percent 0 -> 100
void PWM0B_Set_Duty_Cycle_Percent(unsigned char duty_percent)
{
	unsigned char duty;
	duty = (duty_percent * DUTY_MAX) / 100;
	if (duty != 0)
	{
		TCCR0A |= (1<<COM0B1);							// Clear OC0B on compare match
		OCR0B = duty;
	}
	else
	{
		TCCR0A &= ~(1<<COM0B0);							// Normal Port Operation, OC0B Disconnected
		TCCR0A &= ~(1<<COM0B1);							// Normal Port Operation, OC0B Disconnected
		PWM0B_PORT &= ~(1 << PWM0B_PIN);				// Make PWM Pin Zero
	}
}

#endif // PWM0B

#endif // PWM_CPP

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/