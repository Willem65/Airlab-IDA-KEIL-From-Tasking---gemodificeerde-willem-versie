#include "reg552.h"
//#include "definitions.h"
#include "Delay.h"

#define LCD_E_Wait2					30

unsigned char Wait2;

void SetupLCD()
{
	unsigned char TimeOut = 0x0f;

	// Wait2 minimal 15 ms
	Delay(15);

	LCD_E = 0;
	// Setup/Reset Display
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 0;
	LCD_D6 = 0;
	LCD_D5 = 1;
	LCD_D4 = 1;
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;
	// Wait2 minimal 4.1 ms
	Delay(1);

	// Setup/Reset Display
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 0;
	LCD_D6 = 0;
	LCD_D5 = 1;
	LCD_D4 = 1;
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;
	// Wait2 minimal 100 us
	Delay(1);

	// Setup/Reset Display
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 0;
	LCD_D6 = 0;
	LCD_D5 = 1;
	LCD_D4 = 1;
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;

	// Busy cannot be checked
	Delay(1);

	// Setup/Reset Display to 4 bit mode
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 0;
	LCD_D6 = 0;
	LCD_D5 = 1;
	LCD_D4 = 0;
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;

	Delay(1);

	// Set number of lines etc...
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 0;		// Function set
	LCD_D6 = 0;		// Function set
	LCD_D5 = 1;		// Function set
	LCD_D4 = 0;		// 4 bit
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;

	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 1;		// 2 lines
	LCD_D6 = 0;		// 5x7 dots chars
	LCD_D5 = 0;		// *
	LCD_D4 = 0;		// *
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;

	TimeOut = 0x0F;
	LCD_D7 = 1;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;
	LCD_E = 1;
	while ((LCD_D7) && (TimeOut--))
	{
		LCD_E = 0;
		for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
		LCD_E = 1;
	}
	LCD_E = 0;

	// Display Off
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 0;		// Function set
	LCD_D6 = 0;		// Function set
	LCD_D5 = 0;		// Function set
	LCD_D4 = 0;		
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;

	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 1;		// Display off
	LCD_D6 = 1;		
	LCD_D5 = 0;	 	// Cursor on
	LCD_D4 = 0;	 	// Cursor Blink
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;

	TimeOut = 0x0F;
	LCD_D7 = 1;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;
	LCD_E = 1;
	while ((LCD_D7) && (TimeOut--))
	{
		LCD_E = 0;
		for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
		LCD_E = 1;
	}
	LCD_E = 0;

	// Display Clear
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 0;		// Function set
	LCD_D6 = 0;		// Function set
	LCD_D5 = 0;		// Function set
	LCD_D4 = 0;		
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;

	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 0;		
	LCD_D6 = 0;		
	LCD_D5 = 0;	 
	LCD_D4 = 1;		
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;

	TimeOut = 0x0F;
	LCD_D7 = 1;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;
	LCD_E = 1;
	while ((LCD_D7) && (TimeOut--))
	{
		LCD_E = 0;
		for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
		LCD_E = 1;
	}
	LCD_E = 0;

	// Entry mode
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 0;
	LCD_D6 = 0;
	LCD_D5 = 0;
	LCD_D4 = 0;		
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;

	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 0;		
	LCD_D6 = 1;
	LCD_D5 = 1;		// Position increment after char
	LCD_D4 = 0;		// No shift
	LCD_E = 1;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
	LCD_E = 0;

	TimeOut = 0x0F;
	LCD_D7 = 1;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_E = 1;
	while ((LCD_D7) && (TimeOut--))
	{
		LCD_E = 0;
		for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
		LCD_E = 1;
	}
	LCD_E = 0;


	// Display on
/*	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 0;
	LCD_D6 = 0;
	LCD_D5 = 0;
	LCD_D4 = 0;		
	LCD_E = 1;
	LCD_E = 0;

	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 1;		
	LCD_D6 = 1;
	LCD_D5 = 1;
	LCD_D4 = 1;
	LCD_E = 1;
	LCD_E = 0;

	TimeOut = 0x0F;
	LCD_D7 = 1;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_E = 1;
	LCD_E = 0;
	while ((LCD_D7) && (TimeOut--))
	{
		LCD_E = 1;
		LCD_E = 0;
	}*/
}

/****************************************
* place data on LCD	RS = 0:	Instruction *
*						 				RS = 1:	Data        *
****************************************/
void SetLCD(unsigned char RS, char *Buffer)
{
	unsigned char TimeOut = 0x0F;
	unsigned char cntBuffer = 0;

	LCD_E = 0;
	for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);

	while (Buffer[cntBuffer] != 0)
	{
		// Entry mode
		LCD_RS = RS;
		LCD_RW = 0;
		LCD_D7 = (unsigned char)Buffer[cntBuffer]&0x80;
		LCD_D6 = (unsigned char)Buffer[cntBuffer]&0x40;
		LCD_D5 = (unsigned char)Buffer[cntBuffer]&0x20;
		LCD_D4 = (unsigned char)Buffer[cntBuffer]&0x10;		
		LCD_E = 1;
		for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
		LCD_E = 0;

		TimeOut = 0x0F;
		LCD_D7 = 1;
		LCD_RS = 0;
		LCD_RW = 1;
		LCD_E = 1;
		for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
		LCD_E = 0;

		LCD_E = 1;
		while ((LCD_D7) && (TimeOut--))
		{
			LCD_E = 0;
			for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
			LCD_E = 1;
		}
		LCD_E = 0;

		LCD_RS = RS;
		LCD_RW = 0;
		LCD_D7 = (unsigned char)Buffer[cntBuffer]&0x08;
		LCD_D6 = (unsigned char)Buffer[cntBuffer]&0x04;
		LCD_D5 = (unsigned char)Buffer[cntBuffer]&0x02;
		LCD_D4 = (unsigned char)Buffer[cntBuffer]&0x01;		
		LCD_E = 1;
		for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
		LCD_E = 0;

		TimeOut = 0x0F;
		LCD_D7 = 1;
		LCD_RS = 0;
		LCD_RW = 1;
		LCD_E = 1;
		for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
		LCD_E = 0;

		LCD_E = 1;
		while ((LCD_D7) && (TimeOut--))
		{
			LCD_E = 0;
			for (Wait2=0; Wait2<LCD_E_Wait2; Wait2++);
			LCD_E = 1;
		}
		LCD_E = 0;

		cntBuffer++;
	}
}



