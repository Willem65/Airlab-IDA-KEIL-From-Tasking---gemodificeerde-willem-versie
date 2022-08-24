#include "reg552.h"
#include "ReadModules.h"
#include "airlabbrm-willem.h"



//unsigned int  EventBuffer[256];
//unsigned char EventBufferPtrTop;

//unsigned char ModuleData[NROFMODULES][8];

//unsigned char MasterData[8];


void ReadConsole()
{
	ReadMasterSwitches();
	ReadModuleSwitches(MODULE1_8);
	ReadModuleSwitches(MODULE9_16);

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

	P14 = 0; // Not read master
	P13 = High;

	for (cntByte=0; cntByte<8; cntByte++)
	{
		for (cntModule=0; cntModule<8; cntModule++)
		{
			unsigned char Bits;

			// Select switch to read
			P10 = cntByte&0x01;
			P11 = cntByte&0x02;
			P12 = cntByte&0x04;

			for (Wait=0; Wait<SET_SWITCH_WAIT; Wait++);

			// If bit set and switch isn't ON send event is necessery, else reset counter
      Bits = P5;
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
}