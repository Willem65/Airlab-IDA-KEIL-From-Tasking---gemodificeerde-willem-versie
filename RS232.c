#include "reg552.h"
//#include "Delay.h"
//#include "LCD.h"
#include "airlabbrm-willem.h"
#include "RS232.h"
#include "I2C.h"
#include "ConsoleTimer.h"

//char IDString[30]= "D&R Airlab v2.04 - 08/2006";

void DoSerial()
{
	unsigned char Event = SerialBufferIn[SerialBufferInPtrBottom++];
	unsigned char Channel = SerialBufferIn[SerialBufferInPtrBottom++];
	unsigned char Data = SerialBufferIn[SerialBufferInPtrBottom++];

	switch (Event)
	{
		case SERIAL_ON:
		{
			EventBuffer[EventBufferPtrTop++] = (unsigned int)ON | (Channel<<3);
		}
		break;
		case SERIAL_CUE:
		{
			EventBuffer[EventBufferPtrTop++] = (unsigned int)PFL | (Channel<<3);
		}
		break;
		case TIMER_START:
		{
			GlobalTimerActive = 1;
		}
		break;
		case TIMER_STOP:
		{
			GlobalTimerActive = 0;
		}
		break;
		case TIMER_RESET:
		{
			TimeAndDate.Hours = 0;
			TimeAndDate.Minutes = 0;
			TimeAndDate.Seconds = 0;
			TimeAndDate.Frames = 0;
		}
		break;
		case SERIAL_MODULESETUPDATA:
		{
			unsigned char cntByte;
			ConsoleModuleSettings[Channel][0] = Data;
			for (cntByte=1; cntByte<29; cntByte++)
			{
				ConsoleModuleSettings[Channel][cntByte] = SerialBufferIn[SerialBufferInPtrBottom++];
			}
		}
		break;
		case SERIAL_MASTERSETUPDATA:
		{
			unsigned char cntByte;
			ConsoleMasterSettings[0] = Data;
			for (cntByte=1; cntByte<5; cntByte++)
			{
				ConsoleMasterSettings[cntByte] = SerialBufferIn[SerialBufferInPtrBottom++];
			}
		}
		break;
		case GETSERIAL_IDSTRING:
		{
			unsigned char cntByte;
			SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_IDSTRING;
			SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
			for (cntByte=0; cntByte<30; cntByte++)
			{
				SerialBufferOut[SerialBufferOutPtrTop++] = IDString[cntByte]&0x7F;
 		  }
		}
		break;
		case GETSERIAL_MODULESETUPDATA:
		{
			unsigned char cntByte;

			SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MODULESETUPDATA;
			SerialBufferOut[SerialBufferOutPtrTop++] = Channel;
			for (cntByte=0; cntByte<29; cntByte++)
			{
				SerialBufferOut[SerialBufferOutPtrTop++] =
				ConsoleModuleSettings[Channel][cntByte]&0x7F;
 		  }
		}
		break;
		case GETSERIAL_MASTERSETUPDATA:
		{
			unsigned char cntByte;

			SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MASTERSETUPDATA;
			SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
			for (cntByte=0; cntByte<5; cntByte++)
			{
				SerialBufferOut[SerialBufferOutPtrTop++] = ConsoleMasterSettings[cntByte]&0x7F;
			}
		}
		break;
		case GETSERIAL_CONFIG:
		{
			unsigned char cntByte;

			SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_CONFIG;
			SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
			for (cntByte=0; cntByte<16; cntByte++)
			{
				SerialBufferOut[SerialBufferOutPtrTop++] = ModuleType[cntByte]&0x7F;
			}
		}
		break;
		case STORE_EEPROM:
		{
			unsigned char cntModule;
			unsigned int Address = 0x0000;
			WriteData(I2CAddressEEProm, Address, ModuleType, NROFMODULES);
			Address += NROFMODULES;
			for (cntModule=0; cntModule<NROFMODULES; cntModule++)
			{
				WriteData(I2CAddressEEProm, Address, &(ConsoleModuleSettings[cntModule][0]), 29);
				Address += 29;
			}
			WriteData(I2CAddressEEProm, Address, ConsoleMasterSettings, 5);
		}
		break;
		case STORE_SMARTCARD:
		{
			if (CardInserted)
			{
				unsigned char cntModule;
				unsigned int Address = 0x0000;
				WriteData(I2CAddressSmartCard, Address, ModuleType, NROFMODULES);
				Address += NROFMODULES;
				for (cntModule=0; cntModule<NROFMODULES; cntModule++)
				{
					WriteData(I2CAddressSmartCard, Address, &(ConsoleModuleSettings[cntModule][0]), 29);
					Address += 29;
				}
				WriteData(I2CAddressSmartCard, Address, ConsoleMasterSettings, 5);
			}
		}
		break;
		case GETCARD_INSERTED:
		{		 
			SerialBufferOut[SerialBufferOutPtrTop++] = CARD_INSERTED;
			SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
//			SerialBufferOut[SerialBufferOutPtrTop++] = CardInserted;
		}
		break;
		case FROMDIRECTOR_TELCO_ON_CONNECT:
		{
			if (Data)
			{
				char ModuleNr;
			
				ModuleNr = GetModuleNrFromTelcoNr(Channel);

				if (ModuleNr != -1)
				{
					EventBuffer[EventBufferPtrTop++] = (ModuleNr<<3) | ON;
				}
			}
		}
		break;
		case FROMDIRECTOR_TELCO_MUTE:
		{
			char ModuleNr;
			
			ModuleNr = GetModuleNrFromTelcoNr(Channel);

			if (ModuleNr != -1)
			{
				if (Data)
				{
					EventBuffer[EventBufferPtrTop++] = (unsigned int)(ModuleNr<<3) | EXT_MUTE;
				}
				else
				{
					EventBuffer[EventBufferPtrTop++] = (unsigned int)(ModuleNr<<3) | EXT_MUTE | 0x100;
				}
			}
		}
//		break;
//		case FROMDIRECTOR_TELCO_TB:
//		{
//			if (Data)
//			{
//				char ModuleNr;
//			
//				ModuleNr = GetModuleNrFromTelcoNr(Channel);
//	
//				if (ModuleNr != -1)
//				{
//					EventBuffer[EventBufferPtrTop++] = (ModuleNr<<3) | PFL;
//				}
//			}
//		}
//		break;
//		case FROMDIRECTOR_TB:
//		{
//			if (Channel == TB_CONSOLE)
//			{
//				if (Data)
//				{
//					DirectorTBCUEBussActive = 1;
//				}
//				else
//				{
//					DirectorTBCUEBussActive = 0;
//				}
//			}
//			else if (Channel == TB_ALL)
//			{
//				if (Data)
//				{
//					DirectorTBCUEBussActive = 1;
//				}
//				else
//				{
//					DirectorTBCUEBussActive = 0;
//				}
//			}
//			CueControl();
//		}
//		break;
	}
}


void SetupSIO0()
{
	// initialise serial port                              
	// timer 1 in mode 2, do not touch timer 0 settings    
	PCON = PCON | 0x80;					// SMOD = 1 
	TMOD = (TMOD & 0x0F) | 0x20;		// timer1 mode 2,gate = 0 
	// reload value for 19.2 kbaud (processor at 11.059 Mhz) is 0xFD 

	TH1 = 0xFF;
	// 8 bits, 1 start, 1 stop, receive enabled             
	// when receive should be disabled, fill SCON           
	// with '0x60'                                          
	S0CON	= 0x70;
	// start timer 1                                        
	TR1 = 1;

	TI	= 0;		// reset transmit, receive flags        
	RI	= 0;		// reset recieve interrupt              
	ES0 = 1;		// enable interrupt                     
	PS0 = 0;
}




void SIO0(void) interrupt 4 using 2 
{
	if(RI)
	{
		SerialByte = S0BUF;
		RI = 0;					/*	reset receive flag */

		if (SerialByte&0x80)
		{
			SerialCnt = 0;
			switch (SerialByte)
			{
				case SERIAL_MODULESETUPDATA:
				{
					SerialLength = 31;
				}
				break;
				case SERIAL_MASTERSETUPDATA:
				{
					SerialLength = 7;
				}
				break;
				default:
				{
					SerialLength = 3;
				}
				break;
			}
		}

		DummySerialBuffer[SerialCnt++] = SerialByte;
		if (SerialCnt >= SerialLength)
		{
			unsigned char cnt;
			for (cnt=0; cnt<SerialLength; cnt++)
			{
				SerialBufferIn[SerialBufferInPtrTop++] = DummySerialBuffer[cnt];
			}
		}
	}

	if(TI)
	{
		//SerialBufferOutPtrBottom=0x2B;
//		S0BUF = (!(SerialBufferOut[SerialBufferOutPtrBottom]&0x80));
//		S0BUF = SerialBufferOutPtrTop;
//		S0BUF = SerialBufferOutPtrBottom;
		
		
		//if (SerialBufferOutPtrBottom != SerialBufferOutPtrTop)
		
		if ((SerialBufferOutPtrBottom != SerialBufferOutPtrTop) &&  (!(SerialBufferOut[SerialBufferOutPtrBottom]&0x80)))			
		{
			S0BUF = SerialBufferOut[SerialBufferOutPtrBottom++];
		}
		else
		{
			SendSerialReady = 1;
		}
		TI = 0;
	}
}