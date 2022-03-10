/*************************************************/
/*                   SPI.cpp                     */
/*                                               */
/* This module handles the SPI module.           */
/* SPI pins required.                            */
/* It uses SPI module in Master Mode.            */
/* No software dependency.   	                 */
/*                                               */
/*************************************************/

#ifndef SPI_CPP
#define SPI_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

// ...

/*************************************************/
/*            Hardware Connectivity              */
/*************************************************/

// SPI Pins
#define CS_PIN			PINB2
#define MOSI_PIN		PINB3
#define MISO_PIN		PINB4
#define SCK_PIN			PINB5

// SPI Port
#define SPI_DDR			DDRB
#define SPI_PORT		PORTB

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Declaration of Constants
// ...

// Declaration of Global Variables
// ...

// Declaration of Functions
void SPI_Init();
void SPI_Master_Transmit_Byte(uint8_t);
void SPI_Master_Write_Register(uint8_t, uint8_t);
uint8_t SPI_Master_Receive_Byte();
uint8_t SPI_Master_Transmit_and_Receive(uint8_t);

/*************************************************/
/*                Implementation                 */
/*************************************************/

// Initializing SPI as Master
void SPI_Init()
{
	// Set CS, MOSI and SCK to output
	SPI_DDR |= (1 << CS_PIN) | (1 << MOSI_PIN) | (1 << SCK_PIN);

	// Enable SPI, set as master, and clock to Fosc/128 -> 125KHz for 16MHz Oscillator
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
	
	// Specify the mode here
	//SPCR |= (1 << CPOL);
	SPCR |= (1 << CPHA);
	//SPCR |= (1 << CPOL) | (1 << CPHA);
}

// Transmitting Data Byte over SPI (SPI Master)
void SPI_Master_Transmit_Byte(uint8_t data)
{
	SPDR = data;					// Data to Transmit
	while(!(SPSR & (1 << SPIF)));	// Wait for Completion
}

// Write to Register a Data Byte over SPI (SPI Master)
void SPI_Master_Write_Register(uint8_t Register_Address, uint8_t Register_Value)
{
	SPI_Master_Transmit_Byte(Register_Address);
	_delay_us(100);
	SPI_Master_Transmit_Byte(Register_Value);
}

// Receiving Data Byte over SPI.
uint8_t SPI_Master_Receive_Byte()
{
	SPDR = 0xFF;					// Data to Transmit
	while(!(SPSR & (1 << SPIF)));	// Wait for Completion
	return SPDR;					// Return Received Data
}

// Transmitting and Receiving Data Byte over SPI
uint8_t SPI_Master_Transmit_and_Receive(uint8_t data)
{
	SPDR = data;					// Data to Transmit
	while(!(SPSR & (1 << SPIF)));	// Wait for Completion
	return SPDR;					// Return Received Data
}

#endif

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/