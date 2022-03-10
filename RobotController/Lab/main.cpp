#include "main.h"

int main( void )
{	
	// Setup Section
	Setup_Microcontroller();
	UI_UART0_Display_Homepage();
	UI_LCD_I2C_Display_Homepage();	
	
	/*
	Test_Speed();
	Test_Steering_Motor();
	Test_Traction_Motor();
	Test_LEDs();
	*/
	_delay_ms(500);
	LCD_I2C_Clear();
	
	// Loop Section
	while(1)
	{
		static unsigned char count = COUNT_MAX;	
		
		if(count >= COUNT_MAX)		// To Limit CPU Burden
		{	
			count = 0;
			distanceSR = Sonar_Get_Distance();
			distanceUM1 = UM1_Get_Distance();
			distanceUM2 = UM2_Get_Distance();
			
			UI_LCD_I2C_Display_Sonar_Value(distanceSR);
			UI_LCD_I2C_Display_UM1_Value(distanceUM1);
			UI_LCD_I2C_Display_UM2_Value(distanceUM2);
			
			// Display all values on LCD at one Time:
			// UI_LCD_I2C_Display_All_Distances(distanceSR, distanceUM1, distanceUM2);
			
			// Display all values over UART:
			// UI_UART0_Display_All_Distances(distanceSR, distanceUM1, distanceUM2);	
		}
		
		if(uart0_rx_flag)
		{
			uart0_rx_flag = false;			// Reset flag
			Remote_Control_Process_Data();		
		}
		
		currCameraCMD = Get_OpenMV_Camera_Command();
		if ((enableMotorFlag) && (currCameraCMD != CMD_INVALID) && (prevCameraCMD != currCameraCMD))
		{
			// Action(s) taken when valid
			Servo_Rotate(LUT_SERVO[currCameraCMD]);		
			L293D_HB1_Navigate_Forward(LUT_TRACTION[currCameraCMD]);
			
			// Displaying Command over Serial Port
			UART0_Puts(LUT_STRING[currCameraCMD]);
			UART0_Putchar('\n');  // Enter
			
			// Displaying Command on LCD
			LCD_I2C_Set_Cursor(1, 9);
			LCD_I2C_Puts(LUT_STRING[currCameraCMD]);
			
			// Update Previous Camera Command
			prevCameraCMD = currCameraCMD;
		}
		
		count++;		
	}
	return 0;
}

// This Function initializes all Hardware Modules
void Setup_Microcontroller()
{
	UM1_Init();
	UM2_Init();
	I2C_Init(TW_FREQ_100K, 1);
	LCD_I2C_Init();
	LCD_I2C_Backlight_On();
	ADC_Init();
	Sonar_Init();
	UART0_Init(MYUBRR);
	L293D_HB1_Init();
	Servo_Init();
	Misc_Init();
	sei();
}

// Initializing LEDs, PB, Camera Module
void Misc_Init()
{
	LED1_DIR |= (1 << LED1_PIN);
	LED2_DIR |= (1 << LED2_PIN);
	BUTTON_DIR &= ~(1 << BUTTON_PIN);
	CAM0_DIR &= ~(1 << CAM0_PIN);
	CAM1_DIR &= ~(1 << CAM1_PIN);
	CAM2_DIR &= ~(1 << CAM2_PIN);
	CAM3_DIR &= ~(1 << CAM3_PIN);
	CAM0_PORT |= (1 << CAM0_PIN);
	CAM1_PORT |= (1 << CAM1_PIN);
	CAM2_PORT |= (1 << CAM2_PIN);
	CAM3_PORT |= (1 << CAM3_PIN);	
}

// Get Camera Command using Parallel Interface of 4-Bits
unsigned char Get_OpenMV_Camera_Command()
{
	unsigned char  cmd = ((PINB & 0x3C) >> 2);
	//  0B0011_1100 >> 2 = 0B0000_1111 => [0, ..., 15]
	if (cmd > CMD_RT_MOST)
	cmd = CMD_INVALID;
	return cmd;
}

// Process Received Byte from Remote Control
void Remote_Control_Process_Data()
{
	unsigned char receivedCommand;
	receivedCommand = uart0_rx_data;	// Save received character
	UART0_Putchar(receivedCommand);		// Send it back to sender

	switch (receivedCommand)
	{
		case 's':		// Stop
		case 'S':
			UART0_Put_Message("Motor Stopped\n");
			L293D_HB1_Stop();		// Stop Traction
			Servo_Stop();			// Stop Steering
			prevCameraCMD = CMD_STOPPED;
			currCameraCMD = CMD_STOPPED;
			LCD_I2C_Set_Cursor(1, 9);
			LCD_I2C_Puts(LUT_STRING[currCameraCMD]);
			enableMotorFlag = 0;
			Turn_Off_LED1();
			Turn_Off_LED2();
			break;

		case 'e':		// Enable
		case 'E':
			UART0_Put_Message("Motor Enabled\n");
			enableMotorFlag = 1;
			Turn_On_LED1();
			Turn_On_LED2();
			break;
		
		default:
			UART0_Put_Message("Bad Command\n");
			break;
	}
	_delay_ms(100);
}

// Test Servo Motor
void Test_Steering_Motor()
{
	Servo_Rotate(SERVO_LT_MOST);
	_delay_ms(500);
	Servo_Rotate(SERVO_LT_HALF);
	_delay_ms(500);
	Servo_Rotate(SERVO_CENTER);
	_delay_ms(500);
	Servo_Rotate(SERVO_RT_HALF);
	_delay_ms(500);
	Servo_Rotate(SERVO_RT_MOST);
	_delay_ms(500);
	Servo_Rotate(SERVO_CENTER);
	_delay_ms(500);
	Servo_Stop();
}

// Test Traction Motor
void Test_Traction_Motor()
{
	L293D_HB1_Navigate_Forward_Full_Speed();
	_delay_ms(1000);
	L293D_HB1_Stop();
	_delay_ms(1000);
	L293D_HB1_Navigate_Backward_Full_Speed();
	_delay_ms(1000);
	L293D_HB1_Stop();
}

// Test Speed Control
void Test_Speed()
{
	for(int i = 0; i<=100; i=i+10)
	{
		L293D_HB1_Navigate_Forward(i);
		_delay_ms(10);
	}
	_delay_ms(500);
	for(int i = 100; i>=0; i=i-10)
	{
		L293D_HB1_Navigate_Forward(i);
		_delay_ms(10);
	}
}

// Test LEDs
void Test_LEDs()
{
	for (uint8_t i=0; i<10; i++)
	{
		Turn_On_LED1();
		Turn_Off_LED2();
		_delay_ms(250);
		Turn_Off_LED1();
		Turn_On_LED2();
		_delay_ms(250);
	}
	Turn_Off_LED1();
	Turn_Off_LED2();
}

/*************************************************/
/*   -----------  END OF FILE  ---------------   */
/*************************************************/