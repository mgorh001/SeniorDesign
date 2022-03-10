/*************************************************/
/*                  L293D.cpp                    */
/*                                               */
/* This module handles the L293D motor diver.    */
/* The 1st H-Bridge requires three output pins.  */
/* L293D_IN1_PIN, L293D_IN2_PIN	for direction.   */
/* L293D_EN1_PIN for velocity.                   */
/* L293D_EN1_PIN is of PWM type.                 */
/* This software driver requires:                */
/* The 2nd H-Bridge requires three output pins.  */
/* L293D_IN3_PIN, L293D_IN4_PIN	for direction.   */
/* L293D_EN2_PIN for velocity.                   */
/* L293D_EN2_PIN is of PWM type.                 */
/* This software driver requires:                */
/* 1) PWM.cpp                                    */
/* 2) System_Hardware.h                          */
/*   (For External Hardware Definitions)         */
/*                                               */
/*************************************************/

#ifndef L293D_CPP
#define L293D_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include "PWM.cpp"
#include "System_Hardware.h"

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// Which H-Bridge is Used? (1 for Used, 0 for not Used)
#define USE_H_BRIDGE_1  SYS_USE_H_BRIDGE_1
#define USE_H_BRIDGE_2  SYS_USE_H_BRIDGE_2

#if (USE_H_BRIDGE_1)
// L293D H Bridge 1 Pins
#define L293D_IN1_PIN	SYS_L293D_IN1_PIN
#define L293D_IN2_PIN	SYS_L293D_IN2_PIN
#define L293D_EN1_PIN	SYS_L293D_EN1_PIN
// L293D H Bridge 1 Ports
#define L293D_IN1_PORT	SYS_L293D_IN1_PORT
#define L293D_IN1_DIR	SYS_L293D_IN1_DIR
#define	L293D_IN2_PORT	SYS_L293D_IN2_PORT
#define L293D_IN2_DIR	SYS_L293D_IN2_DIR
#define L293D_EN1_PORT	SYS_L293D_EN1_PORT
#define L293D_EN1_DIR	SYS_L293D_EN1_DIR
#endif // (USE_H_BRIDGE_1)

#if (USE_H_BRIDGE_2)
// L293D H Bridge 2 Pins
#define L293D_IN3_PIN	SYS_L293D_IN3_PIN
#define L293D_IN4_PIN	SYS_L293D_IN4_PIN
#define L293D_EN2_PIN	SYS_L293D_EN2_PIN
// L293D H Bridge 2 Ports
#define L293D_IN3_PORT	SYS_L293D_IN3_PORT
#define L293D_IN3_DIR	SYS_L293D_IN3_DIR
#define L293D_IN4_PORT	SYS_L293D_IN4_PORT
#define L293D_IN4_DIR	SYS_L293D_IN4_DIR
#define L293D_EN2_PORT	SYS_L293D_EN2_PORT
#define L293D_EN2_DIR	SYS_L293D_EN2_DIR
#endif // (USE_H_BRIDGE_2)

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Defines
#define MAX_SPEED_PERCENT	100
#define HALT_SPEED_PERCENT	0

// Declaration of Constants
// ...

// Declaration of Global Variables
// ...

// Declaration of Functions
#if (USE_H_BRIDGE_1)
void L293D_HB1_Init(void);
void L293D_HB1_Stop();
void L293D_HB1_Navigate_Forward(unsigned char speed);
void L293D_HB1_Navigate_Backward(unsigned char speed);
void L293D_HB1_Navigate_Forward_Full_Speed(void);
void L293D_HB1_Navigate_Backward_Full_Speed(void);
#endif

#if (USE_H_BRIDGE_2)
void L293D_HB2_Init(void);
void L293D_HB2_Stop();
void L293D_HB2_Navigate_Forward(unsigned char speed);
void L293D_HB2_Navigate_Backward(unsigned char speed);
void L293D_HB2_Navigate_Forward_Full_Speed(void);
void L293D_HB2_Navigate_Backward_Full_Speed(void);
#endif
/*************************************************/
/*                Implementation                 */
/*************************************************/

#if (USE_H_BRIDGE_1)
// Initialize L293D Module
void L293D_HB1_Init(void)
{
	L293D_IN1_DIR  |=   (1 << L293D_IN1_PIN);	// IN1 PIN Output
	L293D_IN2_DIR  |=   (1 << L293D_IN2_PIN);	// IN2 PIN Output
	L293D_EN1_DIR  |=   (1 << L293D_EN1_PIN);	// EN1 PIN Output
	L293D_IN1_PORT &=  ~(1 << L293D_IN1_PIN);	// IN1 PIN Low
	L293D_IN2_PORT &=  ~(1 << L293D_IN2_PIN);	// IN2 PIN Low
	L293D_EN1_PORT &=  ~(1 << L293D_EN1_PIN);	// EN1 PIN Low
	PWM0A_Init();
}

// Stop Motor
void L293D_HB1_Stop()
{
	L293D_IN1_PORT &=  ~(1 << L293D_IN1_PIN);	// IN1 PIN Low
	L293D_IN2_PORT &=  ~(1 << L293D_IN2_PIN);	// IN2 PIN Low
	// L293D_EN1_PORT &=  ~(1 << L293D_EN1_PIN);	// EN1 PIN Low
	PWM0A_Set_Duty_Cycle_Percent(HALT_SPEED_PERCENT);	
}

// Navigate Robot in the Forward Direction at the Desired Speed (0->100%)
void L293D_HB1_Navigate_Forward(unsigned char speed)
{
	L293D_IN1_PORT |=  (1 << L293D_IN1_PIN);	// IN1 PIN High
	L293D_IN2_PORT &=  ~(1 << L293D_IN2_PIN);	// IN2 PIN Low
	PWM0A_Set_Duty_Cycle_Percent(speed);
}

// Navigate Robot in the Backward Direction at the Desired Speed (0->100%)
void L293D_HB1_Navigate_Backward(unsigned char speed)
{
	L293D_IN1_PORT &=  ~(1 << L293D_IN1_PIN);	// IN1 PIN Low
	L293D_IN2_PORT |=  (1 << L293D_IN2_PIN);	// IN2 PIN High
	PWM0A_Set_Duty_Cycle_Percent(speed);
}

// Navigate Robot in the Forward Direction at the full Speed
void L293D_HB1_Navigate_Forward_Full_Speed(void)
{
	L293D_IN1_PORT |=  (1 << L293D_IN1_PIN);	// IN1 PIN High
	L293D_IN2_PORT &=  ~(1 << L293D_IN2_PIN);	// IN2 PIN Low
	// L293D_EN1_PORT |=  (1 << L293D_EN1_PIN);	// EN1 PIN High
	PWM0A_Set_Duty_Cycle_Percent(MAX_SPEED_PERCENT);			
}

// Navigate Robot in the Backward Direction at the full Speed
void L293D_HB1_Navigate_Backward_Full_Speed(void)
{
	L293D_IN1_PORT &=  ~(1 << L293D_IN1_PIN);	// IN1 PIN Low
	L293D_IN2_PORT |=  (1 << L293D_IN2_PIN);	// IN2 PIN High
	// L293D_EN1_PORT |=  (1 << L293D_EN1_PIN);	// EN1 PIN High
	PWM0A_Set_Duty_Cycle_Percent(MAX_SPEED_PERCENT);
}
#endif  // USE_H_BRIDGE_1

/*************************************************/

#if (USE_H_BRIDGE_2)
// Initialize L293D Module
void L293D_HB2_Init(void)
{
	L293D_IN3_DIR  |=   (1 << L293D_IN3_PIN);	// IN3 PIN Output
	L293D_IN4_DIR  |=   (1 << L293D_IN4_PIN);	// IN4 PIN Output
	L293D_EN2_DIR  |=   (1 << L293D_EN2_PIN);	// EN2 PIN Output
	L293D_IN3_PORT &=  ~(1 << L293D_IN3_PIN);	// IN3 PIN Low
	L293D_IN4_PORT &=  ~(1 << L293D_IN4_PIN);	// IN4 PIN Low
	L293D_EN2_PORT &=  ~(1 << L293D_EN2_PIN);	// EN2 PIN Low
	PWM0B_Init();
}

// Stop Motor
void L293D_HB2_Stop()
{
	L293D_IN3_PORT &=  ~(1 << L293D_IN3_PIN);	// IN3 PIN Low
	L293D_IN4_PORT &=  ~(1 << L293D_IN4_PIN);	// IN4 PIN Low
	// L293D_EN2_PORT &=  ~(1 << L293D_EN2_PIN);	// EN2 PIN Low
	PWM0B_Set_Duty_Cycle_Percent(HALT_SPEED_PERCENT);
}

// Navigate Robot in the Forward Direction at the Desired Speed (0->100%)
void L293D_HB2_Navigate_Forward(unsigned char speed)
{
	L293D_IN3_PORT |=   (1 << L293D_IN3_PIN);	// IN3 PIN High
	L293D_IN4_PORT &=  ~(1 << L293D_IN4_PIN);	// IN4 PIN Low
	PWM0B_Set_Duty_Cycle_Percent(speed);
}

// Navigate Robot in the Backward Direction at the Desired Speed (0->100%)
void L293D_HB2_Navigate_Backward(unsigned char speed)
{
	L293D_IN3_PORT &=  ~(1 << L293D_IN3_PIN);	// IN3 PIN Low
	L293D_IN4_PORT |=   (1 << L293D_IN4_PIN);	// IN4 PIN High
	PWM0B_Set_Duty_Cycle_Percent(speed);
}

// Navigate Robot in the Forward Direction at the full Speed
void L293D_HB2_Navigate_Forward_Full_Speed(void)
{
	L293D_IN3_PORT |=  (1 << L293D_IN3_PIN);	// IN3 PIN High
	L293D_IN4_PORT &=  ~(1 << L293D_IN4_PIN);	// IN4 PIN Low
	// L293D_EN2_PORT |=  (1 << L293D_EN2_PIN);	// EN2 PIN High
	PWM0B_Set_Duty_Cycle_Percent(MAX_SPEED_PERCENT);
}

// Navigate Robot in the Backward Direction at the full Speed
void L293D_HB2_Navigate_Backward_Full_Speed(void)
{
	L293D_IN3_PORT &=  ~(1 << L293D_IN3_PIN);	// IN3 PIN Low
	L293D_IN4_PORT |=  (1 << L293D_IN4_PIN);	// IN4 PIN High
	// L293D_EN2_PORT |=  (1 << L293D_EN2_PIN);	// EN2 PIN High
	PWM0B_Set_Duty_Cycle_Percent(MAX_SPEED_PERCENT);
}
#endif  // USE_H_BRIDGE_2

#endif

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/