/*************************************************/
/*                 Sonar.cpp                     */
/*                                               */
/* This module handles the ultrasonic module.    */
/* It requires one pin for input capture,        */
/* and another pin as output to trigger the UM.  */
/* It uses Timer1 module in capture mode.        */
/* It uses Timer1 overflow interrupt as well.    */
/* This software driver requires:                */
/* 1) delay.h                                    */
/* 2) interrupt.h                                */
/* 3) System_Hardware.h                          */
/*   (For External Hardware Definitions)         */
/*                                               */
/*************************************************/

// Range:
// ------
// From the sensor, the output voltage is given by:
// sensorVoltageOutput = inchRange * (5V/512)			(Eq.1)
//
// ADC Output:
// -----------
// 1 Step -> 5V/1024		// 1024 = 2^10 (10-Bit ADC Conversion)
// For adcReading steps the voltage is:	
// sensorVoltageOutput = adcReading * (5V/1024)			(Eq.2)
//
// Equating Eq.1 and Eq.2
// ----------------------
// inchRange * 5V/512 = adcReading * (5V/1024)
// inchRange = adcReading * (5V/1024) / (5V/512)
// inchRange = adcReading * (1/2)
// inchRange = adcReading >> 1							(Eq.3)
// cmRange = inchRange * 2.54							(Eq.4)

#ifndef SONAR_CPP
#define SONAR_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

#include "ADC.cpp"
#include "System_Hardware.h"

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

#define SONAR_AN_PIN	SYS_SONAR_AN_PIN

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Declaration of Constants
// ...

// Declaration of Global Variables
// ...

// Declaration of Functions
void Sonar_Init(void);
unsigned int Sonar_Get_Distance(void);

/*************************************************/
/*                Implementation                 */
/*************************************************/

// Initialize Sonar Module
void Sonar_Init(void)
{
	ADC_Init();	
}

// Get Distance from Sonar Module in Centimeters
unsigned int Sonar_Get_Distance(void)
{
	unsigned int adcReading, inchRange, cmRange;
	adcReading = ADC_Read(SONAR_AN_PIN);
	inchRange = adcReading >> 1;		// Divide by 2
	cmRange = (unsigned int) (inchRange * 2.54);
	return cmRange;
}

#endif

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/