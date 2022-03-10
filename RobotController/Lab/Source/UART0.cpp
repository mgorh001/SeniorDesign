/*************************************************/
/*                 UART0.cpp                     */
/*                                               */
/* This module handles the UART0 module.         */
/* TX0 and RX0 pins are required.                */
/* It uses UART0 module.                         */
/* No software dependency.   	                 */
/*                                               */
/*************************************************/

#ifndef UART0_CPP
#define UART0_CPP

/*************************************************/
/*              Software Dependency              */
/*************************************************/

// ...

/*************************************************/
/*                 Declarations                  */
/*************************************************/

// Declaration of Constants
// ...

// Declaration of Global Variables
volatile bool uart0_rx_flag = false;
volatile unsigned char uart0_rx_data;

// Declaration of Functions
void UART0_Init(unsigned int);
unsigned char UART0_Getchar(void);
void UART0_Putchar(unsigned char);
void UART0_Putcharhex(unsigned char);
void UART0_Puts(unsigned char*);
void UART0_Put_Message(const char*);

// Declaration of Look-Up Table
// _DEC_TO_HEX: look-up table that converts a decimal number (0->15) to its hex form (0->9, A->F)
unsigned char _DEC_TO_HEX[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

/*************************************************/
/*                Implementation                 */
/*************************************************/

// This ISR receives one character over serial port using Interrupt
ISR(USART_RX_vect)
{
	uart0_rx_data = UDR0;						// Get and return received data from buffer
	uart0_rx_flag = true;
}

// UART0_Init()
// This function:
// + Configures the baud-rate of the serial port
// + Enables the serial port transmission and reception
// + Configures the frame format: 8-bits for data, 1-bit for stop
void UART0_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char) (ubrr >> 8);		// Set baud rate higher byte
	UBRR0L = (unsigned char) ubrr;				// Set baud rate lower byte
	UCSR0B = (1 << RXEN0)  | (1 << TXEN0);		// Enable receiver and transmitter
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);	    // 8-bits for data, 1-bit for stop
	UCSR0B |= (1 << RXCIE0);					// Enable UART0 receiver interrupt
	sei();										// Enable global interrupt
}

// UART0_Getchar()
// This function receives one character over serial port using Polling
unsigned char UART0_Getchar(void)
{
	while (!(UCSR0A & (1 << RXC0)));	// Wait for data to be received
	return UDR0;						// Get and return received data from buffer
}

// UART0_Putchar()
// This function sends one character over serial port
void UART0_Putchar(unsigned char data)
{
	while (!( UCSR0A & (1 << UDRE0)));	// Wait for empty transmit buffer
	UDR0 = data;						// Put data into buffer, sends the data
}

// UART0_Putcharhex()
// This function sends the hexadecimal format of a character over serial port
// For example for character '0' it will sends "0x30"
void UART0_Putcharhex(unsigned char data)
{
	unsigned char digit_high = data / 16;
	unsigned char digit_low = data % 16;
	
	digit_high = _DEC_TO_HEX[digit_high];
	digit_low = _DEC_TO_HEX[digit_low];
	
	UART0_Putchar('0');
	UART0_Putchar('x');
	UART0_Putchar(digit_high);
	UART0_Putchar(digit_low);
	UART0_Putchar(' ');
}

// UART0_Puts()
// This function sends a string over serial port
void UART0_Puts(unsigned char* s)
{
	while (*s)
	{
		UART0_Putchar(*s);
		s++;
	}
}

// UART0_PutMessage()
// This function sends a string over serial port
void UART0_Put_Message(const char* s)
{
	while (*s)
	{
		UART0_Putchar(*s);
		s++;
	}
}

#endif

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/