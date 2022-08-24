#include <stdio.h>
#include "reg552.h"
#include "ReadModules.h"
#include "airlabbrm-willem.h"
#include "LCD.h"
#include "Delay.h"

	unsigned char Bits;

void ReadConsole()
{
	ReadMasterSwitches();
	ReadModuleSwitches(MODULE1_8);    // 0
	ReadModuleSwitches(MODULE9_16);   // 1
}

void ReadMasterSwitches()
{
	unsigned char cntByte;

	P10 = 0; //read master
	P11 = 0; 
	P12 = 0; 
	P13 = 0; 
	P14 = 1; 

	for (Wait=0; Wait<SET_SWITCH_WAIT; Wait++);

	for (cntByte=0; cntByte<8; cntByte++)
	{
		// If bit set and switch isn't ON send event is necessery, else reset counter
		unsigned char Bits = P5;
		if ((!(Bits&(0x01<<cntByte))) && (!(MasterData[cntByte]&0x80)))
		{
			if ((--MasterData[cntByte])&0x80)
			{
				EventBuffer[EventBufferPtrTop++] = 0x80 | cntByte;
			}
		}
		else if ((Bits&(0x01<<cntByte))&& (MasterData[cntByte]&0x80))
		{
			MasterData[cntByte]=ATTACK;
			EventBuffer[EventBufferPtrTop++] = 0x180 | cntByte;
		}
	}
}




void ReadModuleSwitches(unsigned char High)
{
	unsigned char cntByte;
	unsigned char cntModule;
//	unsigned char str[8];
	unsigned char Bits;
	

	P13 = High;  // AS3
	P14 = 0;     // AS4   Not read master

	for (cntByte=0; cntByte<8; cntByte++)
	{
		for (cntModule=0; cntModule<8; cntModule++)
		{
			//unsigned char Bits;

			// Select switch to read
			P10 = cntByte&0x01;   // AS0
			P11 = cntByte&0x02;   // AS1
			P12 = cntByte&0x04;   // AS2
			
//			P10 = 1; 
//			P11 = 1; 
//			P12 = 0; 
//			P13 = 1; 
//			P14 = 0; 
			

			for (Wait=0; Wait<SET_SWITCH_WAIT; Wait++);

			// If bit set and switch isn't ON send event is necessery, else reset counter
      Bits = P5;
			
			//S0BUF = 0x31;

			
			
			if ((!(Bits&(0x01<<cntModule))) && (!(ModuleData[cntModule+(8*High)][cntByte]&0x80)))
			{
				if ((--ModuleData[cntModule+(8*High)][cntByte])&0x80)
				{
					EventBuffer[EventBufferPtrTop++] = ((cntModule+(8*High))<<3) | cntByte;
				}
			}
			else if ((Bits&(0x01<<cntModule)) && (ModuleData[cntModule+(8*High)][cntByte]&0x80))
			{
				ModuleData[cntModule+(8*High)][cntByte]=ATTACK;
				EventBuffer[EventBufferPtrTop++] = 0x100 | ((cntModule+(8*High))<<3) | cntByte;
			}
		}
	}
	
	
	
//						SetLCD(0,"\xC0");
//					
//					// -------------------------
//					// ----- int 2 string ------
//					//--------------------------

//					
//					sprintf(str, "%d  ", ModuleData[2][0]);
//					// ------------------------------------
//					
//					SetLCD(1,str);
//					//Delay(10);
}



void ShowConfig()
{
	char TextBuffer[17];
	unsigned char Ptr=0;

	
	TextBuffer[Ptr++] = 'S'; //"StMod:x Telco:xx"
	TextBuffer[Ptr++] = 't'; //"StMod:xx Telco:x"
	TextBuffer[Ptr++] = 'M';
	TextBuffer[Ptr++] = 'o';
	TextBuffer[Ptr++] = 'd';
	TextBuffer[Ptr++] = ':';
	if ((NrOfTriples/10)!=0)
	{
		TextBuffer[Ptr++] = 0x30+(NrOfTriples/10);
	}

	TextBuffer[Ptr++] = 0x30+(NrOfTriples%10);
	TextBuffer[Ptr++] = ' ';
	TextBuffer[Ptr++] = 'T';
	TextBuffer[Ptr++] = 'e';
	TextBuffer[Ptr++] = 'l';
	TextBuffer[Ptr++] = 'c';
	TextBuffer[Ptr++] = 'o';
	TextBuffer[Ptr++] = ':';
	if ((NrOfTelcos/10)!=0)
	{
		TextBuffer[Ptr++] = 0x30+(NrOfTelcos/10);
	}
//	else
//	{
//		TextBuffer[Ptr++] = ' ';
//	}
	TextBuffer[Ptr++] = 0x30+(NrOfTelcos%10);
	TextBuffer[Ptr++] = 0;
	SetLCD(0,"\xC0");
	SetLCD(1,TextBuffer);
}
