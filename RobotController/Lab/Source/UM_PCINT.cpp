/*************************************************/
/*                UM_PCINT.cpp                   */
/*                                               */
/* This module handles the ultrasonic module.    */
/* It requires one pin for echo pulse input,     */
/* and another pin as output to trigger the UM.  */
/* It uses Timer1 module in capture mode.        */
/* It uses Timer1 overflow interrupt as well.    */
/* This software driver requires:                */
/* 1) delay.h						             */
/* 2) interrupt.h                                */
/* 3) System_Hardware.h                          */
/*   (For External Hardware Definitions)         */
/*                                               */
/*************************************************/

#ifndef UM_SR04_PCINT_CPP
#define UM_SR04_PCINT_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include <util/delay.h>
#include <avr/interrupt.h>
#include "System_Hardware.h"

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// UM Usage (1 for Used, 0 for not Used)
#define UM1				SYS_UM1
#define UM2				SYS_UM2

#if (UM1)
// UM1 Pins
#define UM1_TRIG_PIN	SYS_UM1_TRIG_PIN
#define UM1_ECHO_PIN	SYS_UM1_ECHO_PIN
// UM1 Ports
#define UM1_TRIG_PORT	SYS_UM1_TRIG_PORT
#define UM1_TRIG_DIR	SYS_UM1_TRIG_DIR
#define UM1_ECHO_PORT	SYS_UM1_ECHO_PORT
#define UM1_ECHO_DIR	SYS_UM1_ECHO_DIR
#define UM1_ECHO_PRTIN	SYS_UM1_ECHO_PRTIN
#define UM1_ECHO_PCIE	SYS_UM1_ECHO_PCIE
#define UM1_ECHO_PCINT	SYS_UM1_ECHO_PCINT	
#endif // UM1

#if (UM2)
// UM2 Pins
#define UM2_TRIG_PIN	SYS_UM2_TRIG_PIN
#define UM2_ECHO_PIN	SYS_UM2_ECHO_PIN
// UM2 Ports
#define UM2_TRIG_PORT	SYS_UM2_TRIG_PORT
#define UM2_TRIG_DIR	SYS_UM2_TRIG_DIR
#define UM2_ECHO_PORT	SYS_UM2_ECHO_PORT
#define UM2_ECHO_DIR	SYS_UM2_ECHO_DIR
#define UM2_ECHO_PRTIN	SYS_UM2_ECHO_PRTIN
#define UM2_ECHO_PCIE	SYS_UM2_ECHO_PCIE
#define UM2_ECHO_PCINT	SYS_UM2_ECHO_PCINT
#endif // UM2

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Defines
#ifndef LOW
#define LOW		0
#endif

#ifndef HIGH
#define HIGH	1
#endif

// Declaration of Constants
const unsigned char RESPONSE_DELAY_MS = 50;
const unsigned char RESPONSE_N_EDGES = 2;
const unsigned long MAX_VALUE_16_BIT_PLUS_1 = 65536;
const unsigned int  UM_MAX_DISTANCE = 330;

// Declaration of Global Variables
volatile unsigned char timer1OverflowCount;
volatile unsigned int  timer1Capture;
volatile unsigned char UM1_EdgeInterruptCount;
volatile unsigned char UM2_EdgeInterruptCount;

// Declaration of Functions
void UM1_Init(void);
void UM2_Init(void);
unsigned int UM1_Get_Distance(void);
unsigned int UM2_Get_Distance(void);
static void UM1_Edge_Handling(void);
static void UM2_Edge_Handling(void);
static void UM1_Send_Trigger(void);
static void UM2_Send_Trigger(void);
static void Setup_Timer1_Overflow(void);
static void Disable_Timer1_Overflow(void);

/*************************************************/
/*                Implementation                 */
/*************************************************/

// Disable Timer1 Overflow
static void Disable_Timer1_Overflow(void)
{
	TCCR1B = 0;							// Stop Timer1
	TIMSK1 &= ~(1 << TOIE1);			// Disable Timer1 Overflow Interrupt
}

// Setup Timer1 Overflow
static void Setup_Timer1_Overflow(void)
{
	TCNT1 = 0;							// Clear Timer counter
	TCCR1A = 0;							// Timer1 in Normal operation
	TCCR1B = (1 << CS10);				// No prescaler
	timer1OverflowCount = 0;			// Clear Timer1 Overflow Count
	TIFR1  |= (1 << TOV1);				// Clear Timer1 Overflow Flag
	TIMSK1 |= (1 << TOIE1);  			// Enable Timer1 overflow
}

/*************************************************/

static void UM1_Edge_Handling(void)
{
	static bool UM1_Previous_Pin_State = LOW;
	static bool UM1_Current_Pin_State = LOW;
	// Update Current Pin State
	UM1_Current_Pin_State = UM1_ECHO_PRTIN & (1 << UM1_ECHO_PIN);
	
	// If UM1 triggered the ISR, check for Edge
	if ((UM1_Current_Pin_State == HIGH) && (UM1_Previous_Pin_State == LOW))
	{
		// It's a rising edge!
		UM1_Previous_Pin_State = HIGH;	// Update Previous Pin State
		Setup_Timer1_Overflow();		// Setup Timer1 Overflow
		UM1_EdgeInterruptCount++;		// First Edge
	}
	else if ((UM1_Current_Pin_State == LOW) && (UM1_Previous_Pin_State == HIGH))
	{
		// It's a falling edge!
		UM1_Previous_Pin_State = LOW;	// Update Previous Pin State
		timer1Capture = TCNT1;			// Capture Timer1
		Disable_Timer1_Overflow();		// Stop and Disable Timer1 Overflow
		UM1_EdgeInterruptCount++;		// Second Edge
	}
}

/*************************************************/

static void UM2_Edge_Handling(void)
{
	static bool UM2_Previous_Pin_State = LOW;
	static bool UM2_Current_Pin_State = LOW;
	// Update Current Pin State
	UM2_Current_Pin_State = UM2_ECHO_PRTIN & (1 << UM2_ECHO_PIN);

	// If UM2 triggered the ISR, check for Edge
	if ((UM2_Current_Pin_State == HIGH) && (UM2_Previous_Pin_State == LOW))
	{
		// It's a rising edge!
		UM2_Previous_Pin_State = HIGH;	// Update Previous Pin State
		Setup_Timer1_Overflow();		// Setup Timer1 Overflow
		UM2_EdgeInterruptCount++;		// First Edge
	}
	else if ((UM2_Current_Pin_State == LOW) && (UM2_Previous_Pin_State == HIGH))
	{
		// It's a falling edge!
		UM2_Previous_Pin_State = LOW;	// Update Previous Pin State
		timer1Capture = TCNT1;			// Capture Timer1
		Disable_Timer1_Overflow();		// Stop and Disable Timer1 Overflow
		UM2_EdgeInterruptCount++;		// Second Edge
	}
}

/*************************************************/

// Timer 1 Overflow ISR
ISR(TIMER1_OVF_vect)
{
	timer1OverflowCount++;				// Increment Timer Overflow count
}

// Interrupt on Change PORTB
ISR (PCINT0_vect)
{
	#if ((UM1) && (UM1_ECHO_PCIE == PCIE0))
	UM1_Edge_Handling();
	#endif // (UM1)
		
	#if ((UM2) && (UM2_ECHO_PCIE == PCIE0))
	UM2_Edge_Handling();
	#endif // (UM2)
}

// Interrupt on Change PORTC
ISR (PCINT1_vect)
{
	#if ((UM1) && (UM1_ECHO_PCIE == PCIE1))
	UM1_Edge_Handling();
	#endif // (UM1)
	
	#if ((UM2) && (UM2_ECHO_PCIE == PCIE1))
	UM2_Edge_Handling();
	#endif // (UM2)
}

// Interrupt on Change PORTD
ISR (PCINT2_vect)
{ 
	#if ((UM1) && (UM1_ECHO_PCIE == PCIE2))
	UM1_Edge_Handling();
	#endif // (UM1)
	
	#if ((UM2) && (UM2_ECHO_PCIE == PCIE2))
	UM2_Edge_Handling();
	#endif // (UM2)
}

#if (UM1)
// Send Trigger for Ultrasonic Module 1
static void UM1_Send_Trigger(void)
{
	UM1_TRIG_PORT |=   (1 << UM1_TRIG_PIN);		// Trigger PIN High
	_delay_us(10);								// 10us Burst
	UM1_TRIG_PORT &=  ~(1 << UM1_TRIG_PIN);		// Trigger PIN Low
}

// Initialize Ultrasonic Module 1
void UM1_Init(void)
{
	UM1_TRIG_DIR |=   (1 << UM1_TRIG_PIN);		// Trigger PIN Output
	UM1_ECHO_DIR &=  ~(1 << UM1_ECHO_PIN);		// Echo PIN Input
	PCICR |= (1<<UM1_ECHO_PCIE);				// Enable PORT Interrupt on Change
	#if (UM1_ECHO_PCIE == PCIE2)				// Enable UM1 Pin Interrupt on Change
	PCMSK2 |= (1<<UM1_ECHO_PCINT);
	#elif (UM1_ECHO_PCIE == PCIE1)
	PCMSK1 |= (1<<UM1_ECHO_PCINT);
	#elif (UM1_ECHO_PCIE == PCIE0)
	PCMSK0 |= (1<<UM1_ECHO_PCINT);
	#endif	
	sei();
}

// Get Distance from Sensor Module 1 in Centimeters
unsigned int UM1_Get_Distance(void)
{
	unsigned long count = 0;
	unsigned long distance = 0;
	UM1_Send_Trigger();
	UM1_EdgeInterruptCount = 0;
	timer1OverflowCount = 0;
	_delay_ms(RESPONSE_DELAY_MS);
	if (UM1_EdgeInterruptCount == RESPONSE_N_EDGES)
	{	
		count = timer1Capture + (unsigned long)(MAX_VALUE_16_BIT_PLUS_1 * timer1OverflowCount); 
		distance = (count / 466.47);		// 8MHz Timer Frequency, Sound Speed = 343 m/s
		#if (F_CPU == 16000000UL)
		distance = distance >> 1;			// Divide by 2
		#endif
	}
	return distance;
}
#endif // UM1

/*************************************************/

#if (UM2)
// Send Trigger for Ultrasonic Module 2
static void UM2_Send_Trigger(void)
{
	UM2_TRIG_PORT |=   (1 << UM2_TRIG_PIN);		// Trigger PIN High
	_delay_us(10);								// 10us Burst
	UM2_TRIG_PORT &=  ~(1 << UM2_TRIG_PIN);		// Trigger PIN Low
}

// Initialize Ultrasonic Module 2
void UM2_Init(void)
{
	UM2_TRIG_DIR |=   (1 << UM2_TRIG_PIN);		// Trigger PIN Output
	UM2_ECHO_DIR &=  ~(1 << UM2_ECHO_PIN);		// Echo PIN Input
	PCICR |= (1<<UM2_ECHO_PCIE);				// Enable PORT Interrupt on Change
	#if (UM2_ECHO_PCIE == PCIE2)				// Enable UM2 Pin Interrupt on Change
	PCMSK2 |= (1<<UM2_ECHO_PCINT);
	#elif (UM2_ECHO_PCIE == PCIE1)
	PCMSK1 |= (1<<UM2_ECHO_PCINT);
	#elif (UM2_ECHO_PCIE == PCIE0)
	PCMSK0 |= (1<<UM2_ECHO_PCINT);
	#endif
	sei();
}

// Get Distance from Sensor Module 2 in Centimeters
unsigned int UM2_Get_Distance(void)
{
	unsigned long count = 0;
	unsigned long distance = 0;
	UM2_Send_Trigger();
	UM2_EdgeInterruptCount = 0;
	timer1OverflowCount = 0;
	_delay_ms(RESPONSE_DELAY_MS);
	if (UM2_EdgeInterruptCount == RESPONSE_N_EDGES)
	{
		count = timer1Capture + (unsigned long)(MAX_VALUE_16_BIT_PLUS_1 * timer1OverflowCount);
		distance = (count / 466.47);		// 8MHz Timer Frequency, Sound Speed = 343 m/s
		#if (F_CPU == 16000000UL)
		distance = distance >> 1;			// Divide by 2
		#endif
	}
	return distance;
}
#endif // UM2

#endif

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/