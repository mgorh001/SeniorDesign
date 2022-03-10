/*************************************************/
/*                 Servo.cpp                     */
/*                                               */
/* This module handles the PWM module.           */
/* One pin is required as servo motor control.   */
/* It uses the Timer0 module.                    */
/* This software driver requires:                */
/* 1) interruprt.h                               */
/* 2) System_Hardware.h                          */
/*   (For External Hardware Definitions)         */
/*                                               */
/*************************************************/

#ifndef SERVO_CPP
#define SERVO_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include <avr/interrupt.h>
#include "System_Hardware.h"

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// Servo Pin
#define SERVO_PIN	SYS_SERVO_PIN

// Servo Port
#define SERVO_PORT	SYS_SERVO_PORT
#define SERVO_DIR	SYS_SERVO_DIR

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Declaration of Constants
const unsigned int TOTAL_ANGLES = 181;
const unsigned int TMR2_PRE = 256;
const unsigned int MIN_PULSE_WIDTH = 550;   // the shortest pulse sent to a servo
const unsigned int MAX_PULSE_WIDTH = 2450;   // the longest pulse sent to a servo

// Declaration of Global Variables
volatile unsigned char lowPulseFlag;
volatile unsigned char highPulseFlag;
volatile unsigned char loadValueTCNT2;
volatile unsigned char loadComplementValueTCNT2;
unsigned char angle;

// Declaration of Functions
void Servo_Init(void);
void Servo_Rotate(unsigned char);
void Servo_Stop(void);

// Declaration of Look-Up Tables
unsigned char SERVO_TCNT2_LUT[TOTAL_ANGLES];		// [0 ... 180]

/*************************************************/
/*                Implementation                 */
/*************************************************/

// Timer 2 Overflow ISR
ISR(TIMER2_OVF_vect)
{
	static unsigned char timer2OverflowCount;
	TIFR2 |= (1<<TOV2);
	if(lowPulseFlag)
	{	
		SERVO_PORT &= ~(1 << SERVO_PIN);		// Clear PIN
		timer2OverflowCount++;							
		if (timer2OverflowCount == 1)
		{
			TCCR2B = 0;	
			TCCR2B |= (1 << CS22) | (1 << CS21);   	// Prescaler = 256
			TCCR2A = 0;								// Normal Mode
			TCNT2 = loadComplementValueTCNT2;	
		}
		else if (timer2OverflowCount == 2)
		{
			TCCR2B = 0;
			TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20);// Prescaler = 1024
			TCCR2A = 0;
			TCNT2 = 123;
		}
		else if (timer2OverflowCount == 3)
		{
			TCCR2B = 0;
			TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20);// Prescaler = 1024
			TCCR2A = 0;
			TCNT2 = 123;
			timer2OverflowCount = 0;
			lowPulseFlag = 0;
			highPulseFlag = 1;			
		}
	}
	else if (highPulseFlag)
	{
		lowPulseFlag = 1;
		highPulseFlag = 0;
		SERVO_PORT |=  (1 << SERVO_PIN);		// Set PIN
		TCCR2B = 0;
		TCCR2B |= (1 << CS22) | (1 << CS21);   	// Prescaler = 256
		TCCR2A = 0;								// Normal Mode
		TCNT2 = loadValueTCNT2;
	}
}

// Initialize Servo Module
void Servo_Init()
{
	SERVO_DIR  |=  (1 << SERVO_PIN);	     // Make Servo Pin Output
	SERVO_PORT &= ~(1 << SERVO_PIN);	     // Make Servo Pin Zero
	
	for (unsigned short a = 0; a < TOTAL_ANGLES; a++)
	{
		unsigned int pulseWidth = MIN_PULSE_WIDTH + (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) * (a / 180.0);
		unsigned int stepCount = (pulseWidth * (F_CPU / 1000000UL)) / TMR2_PRE;
		SERVO_TCNT2_LUT[a] = (unsigned char) (256 - stepCount);
	}
	
	lowPulseFlag = 0;
	highPulseFlag = 0;
	
	TCCR2A = 0;								// Normal Mode
	TCCR2B = 0;
	TCCR2B |= (1 << CS22);               	// Prescaler = 64
	TCNT2 = 0;
	TIMSK2 |= (1 << TOIE2);					// Enable Timer2 Overflow Interrupt
	sei();
}

// Send Command to Servo
void Servo_Rotate(unsigned char angle)
{
	loadValueTCNT2 = SERVO_TCNT2_LUT[angle];
	loadComplementValueTCNT2 = SERVO_TCNT2_LUT[TOTAL_ANGLES - 1 - angle]; 
	lowPulseFlag = 0;
	highPulseFlag = 1;
	TIMSK2 |= (1 << TOIE2);					// Enable Timer2 Overflow Interrupt
}

// Stop Servo Motor
void Servo_Stop()
{
	lowPulseFlag = 0;
	highPulseFlag = 0;
	TIMSK2 &= ~(1 << TOIE2);				// Disable Timer2 Overflow Interrupt
}
#endif // SERVO_CPP

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/