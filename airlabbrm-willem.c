#include "reg552.h"
#include "airlabbrm-willem.h"
#include "LCD.h"
#include "Delay.h"
#include "RS232.h"
#include "I2C.h"
#include "ReadModules.h"
#include "ConsoleTimer.h"
#include "Events.h"

// defines
#define DELAY								10
#define	PULSWIDTH						10
#define SET_SERIAL_WAIT			5

#define	NORMALMODE					0
#define	MASTERMODE			 		2
#define LCD_E_WAIT					30

																																	 
#define TBL_MUTE						PWM0

sbit  CLK1 = 0xB4;
sbit  DATA1 = 0xB2;
sbit CLK2 = 0xB5;
sbit DATA2 = 0xB3;
sbit STRB2	= 0x95;			

#define TELCO								2
#define ONAIR1_FUNCTION								0x20
#define ONAIR2_FUNCTION						 		0x10
#define STUDIOMUTE_FUNCTION_LED				0x04
#define MASTER_CUE_ON_FUNCTION_LED		0x02
#define CRMMUTE_FUNCTION_LED					0x01

#define	ON_LED					    0x20
#define START_FUNCTION	    0x02


// Defines for Events				  
#define FADERON							0x01
#define MODULETYPEA					0x04
#define	MODULETYPEB					0x05
#define LINEB								0x06
#define ESCAPE							0x80
#define ENTER								0x81
#define RIGHT 							0x82
#define DOWN								0x83
#define UP									0x84
#define LEFT								0x85
#define CUERESET						0x86
#define SW_CC								0x87


// defines for ConsoleSettings
#define		TIMERLINEA	0
#define		TIMERLINEB	1
#define		TIMERMIC		2
#define		STARTLINEA	3
#define		STARTLINEB	4
#define		STARTMIC		5
#define		STARTFADER	6
#define		STARTON			7
#define		STARTCUE		8
#define		STARTPULS		9
#define		REMLINEA		10
#define		REMLINEB		11
#define		DJMIC				13

#define		STUDIOMIC		15
#define		ONAIR1LINEA	16
#define		ONAIR1LINEB	17
#define		ONAIR1MIC		18 
#define		ONAIR2LINEA	19
#define		ONAIR2LINEB	20
#define		ONAIR2MIC		21
#define		POWERON			22
#define		MICATLINEA	23
#define		MICATLINEB	24
#define		REMONMIC		25
#define		REMONLINEA 	26
#define		REMONLINEB	27
#define		SWITCHSTART	28
#define		RS232					3
#define		ENABLESETUP		4

char IDString[30]= "D&R Airlab v2.04 - 08/2006";

unsigned char Hex2ASCII[17] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47};
	
unsigned char TextString[4][17] = {	{"Timer           "},
																	{"Module Settings "},
																	{"Master Settings "},
																	{"System          "},};

unsigned char	SystemTextString[6][17] = {	{"Config          "},
																				{"Recall          "},
																				{"Store           "},
																				{"Recall Mem. Card"},
																				{"Store Mem. Card "},
																				{"Eprom String    "}};

unsigned char	ModuleTextString[29][17] ={	{"LineA Timerstrt?"},
																				{"LineB Timerstrt?"},
																				{"Mic. Timerstrt? "},
																				{"LineA Start?    "},
																				{"LineB Start?    "},
																				{"Mic. Start?     "},
																				{"Start by Fader? "},
																				{"Start by On?    "},
																				{"Start by Cue?   "},
																				{"Puls */Cont O  ?"},
																				{"LineA Rem. Cue? "},
																				{"LineB Rem. Cue? "},
																				{"Mic. Rem. Cue?  "},
																				{"DJ. Module?     "},
																				{"Ann. Module?    "},
																				{"Studio Module?  "},
																				{"LineA On Air-1? "},
																				{"LineB On Air-1? "},
																				{"Mic. On Air-1?  "},
																				{"LineA On Air-2? "},
																				{"LineB On Air-2? "},
																				{"Mic. On Air-2?  "},
																				{"ON at Power up? "},
																				{"MIC at LineA?   "},
																				{"MIC at LineB?   "},
																				{"MIC Rem. On?    "},
																				{"LineA Rem. On?  "},
																				{"LineB Rem. On?  "},
																				{"Switch Start?   "}};

unsigned char MasterTextString[4][17] = {	{"Auto Cue Reset? "},
																				{"CUE to CRM?     "},
																				{"Auto Comm?      "},
																				{"Enable RS232?   "}};

void Init(void);
void SetupTimer0(void);
void ReadConsole();
void ReadMasterSwitches();
void ReadModuleSwitches(unsigned char High);
void CueControl(void);
void CheckTBMic();
																				
unsigned char TextStringPtr;
unsigned char CUEByRemoteMIC[NROFMODULES];



// variables
unsigned char EventBufferPtrBottom;
unsigned char SerialBufferOut[256];
unsigned char SerialBufferIn[256];
unsigned char SerialBufferInPtrTop;
unsigned char SerialBufferInPtrBottom;
unsigned char SerialBufferOutPtrTop;
unsigned char SerialBufferOutPtrBottom;
unsigned char SerialByte;
unsigned char SerialLength;
unsigned char SerialCnt;
unsigned char SendSerialReady;
unsigned char DummySerialBuffer[256];
unsigned char SendSerialData;
unsigned char DirectorTBCUEBussActive;
unsigned char EventBufferPtrTop;
unsigned int  EventBuffer[256];
unsigned char ModuleData[NROFMODULES][8];

//Byte 0:					Module Ext. Remote In			 
//Byte 1:					Module Fader-On						 
//Byte 2:					Module PFL-switch					 
//Byte 3:					Module On-switch					 
//Byte 4:					Module Type(B)						 
//Byte 5:					Module Type(A)							
//Byte 6:					Module Line B Select				
//Byte 7:					Module Mic Select						


unsigned char MasterData[8];

//Byte 0:	Escape
//Byte 1:	Enter
//Byte 2:	Right
//Byte 3:	Down
//Byte 4:	Up
//Byte 5:	Left
//Byte 6:	CUE-Reset
//Byte 7:	SW-CC


unsigned char ShowFunctionData[17];
unsigned char ConsoleFunctionData[17];

//												Bit7		Bit6		Bit5		Bit4		Bit3		Bit2		Bit1		Bit0
//Module 1/Byte 0:				N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 2/Byte 1:				N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 3/Byte 2:				N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 4/Byte 3:				N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 5/Byte 4:				N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 6/Byte 5:				N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 7/Byte 6:				N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 8/Byte 7:				N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 9/Byte 8:				N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 10/Byte 9:				N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 11/Byte 10:			N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 12/Byte 11:			N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 13/Byte 12:			N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 14/Byte 13:			N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 15/Byte 14:			N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed
//Module 16/Byte 15:			N.U.		N.U.		OnLed		Mute		CueLed	Cue			Start		RemoteLed

//Master/Byte16: 					Bit7	N.U.
//												Bit6	PowerOnCC
//												Bit5	OnAir2
//												Bit4	OnAir1
//												Bit3	AnnouncerComm
//												Bit2	StudioMute
//												Bit1	MasterCUEOn
//												Bit0	CRMMute



unsigned char ConsoleMasterSettings[5];
unsigned char UndoConsoleMasterSettings[5];

////Byte 0:				AUTOCUERESET
////Byte 1:				CUETOCRM
////Byte 2:				AUTOCOM
////Byte 3:				RS232
////Byte 4:				ENABLESETUP

unsigned char TempEnableSetup;

unsigned char ModuleType[NROFMODULES];

unsigned char ConsoleModuleSettings[NROFMODULES][29];
unsigned char UndoConsoleModuleSettings[NROFMODULES][29];
////Byte 0:		TIMERLINEA
////Byte 1:				TIMERLINEB
////Byte 2:				TIMERMIC
////Byte 3:				STARTLINEA
////Byte 4:				STARTLINEB
////Byte 5:				STARTMIC
////Byte 6:				STARTFADER
////Byte 7:				STARTON
////Byte 8:				STARTCUE
////Byte 9:				STARTPULS
////Byte 10:			REMLINEA
////Byte 11:			REMLINEB
////Byte 12:			REMMIC
////Byte 13:			DJMIC
////Byte 14:			ANNMIC
////Byte 15:			STUDIOMIC
////Byte 16:			ONAIR1LINEA
////Byte 17:			ONAIR1LINEB
////Byte 18:			ONAIR1MIC
////Byte 19:			ONAIR2LINEA
////Byte 20:			ONAIR2LINEB
////Byte 21:			ONAIR2MIC
////Byte 22:			POWERON
////Byte 23:			MICATLINEA
////Byte 24:			MICATLINEB
////Byte 27:			REMONMIC
////Byte 25:			REMONLINEA
////Byte 26:			REMONLINEB
////Byte 27:			SWITCHSTART

//unsigned char Count;
unsigned char Wait;  // extern gemaakt
unsigned char OperatingMode;
unsigned char TimerActive[NROFMODULES];
unsigned char cntStartPuls[NROFMODULES];

unsigned char cntStartFunction[NROFMODULES];
unsigned char cntCueFunction[NROFMODULES];
unsigned char cntMuteFunction[NROFMODULES];

unsigned char NrOfTelcos;
unsigned char NrOfTriples;
unsigned char CardInserted;

unsigned char StartActive[NROFMODULES];

//unsigned char cntByte, cntChannel;







void CheckStartCondition(unsigned char Module)
{
	unsigned char StartOn = 0;

	if (!ConsoleModuleSettings[Module][SWITCHSTART])
	{
		if  ((ConsoleModuleSettings[Module][STARTON]) && (ConsoleModuleSettings[Module][STARTFADER]))
		{
			if (((ModuleData[Module][FADERON]&0x80)) && (ConsoleFunctionData[Module]&ON_LED))
			{	 //!JB
				if (!StartActive[Module])
				{
					StartControl(Module,1);
					StartActive[Module] = 1;
				}
				StartOn = 1;
			}
			else
			{
				StartControl(Module,0);
				StartActive[Module] = 0;
			}
		}
		else if  ((ConsoleModuleSettings[Module][STARTON]) && (!(ConsoleModuleSettings[Module][STARTFADER])))
		{
			if (ConsoleFunctionData[Module]&ON_LED)
			{
				if (!StartActive[Module])
				{
					StartControl(Module,1);
					StartActive[Module] = 1;
				}
				StartOn = 1;
			}
			else
			{
				StartControl(Module,0);
				StartActive[Module] = 0;
			}
		}
		else if  ((!ConsoleModuleSettings[Module][STARTON]) && (ConsoleModuleSettings[Module][STARTFADER]))
		{
			if ((ModuleData[Module][FADERON]&0x80)) 
			{	 // !JB
				if (!StartActive[Module])
				{
					StartControl(Module,1);
					StartActive[Module] = 1;
				}
				StartOn = 1;
			}
			else
			{
				StartControl(Module,0);
				StartActive[Module] = 0;
			}
		}	
	}
	if (ConsoleModuleSettings[Module][STARTCUE])
	{
		if (ConsoleFunctionData[Module]&CUE_FUNCTION)
		{
			StartControl(Module,1);
			StartActive[Module] = 1;
		}
		else if (!StartOn)
		{
			StartControl(Module,0);
			StartActive[Module] = 0;
		}
	}
}

	
	
	


void CheckCUEByRemoteMIC(void)
{
	char Status = 0;
	int cntModule;

	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if  (ModuleType[cntModule]==TRIPLE)
		{
			if (ModuleData[cntModule][MIC_RING]&0x80)
			{
				Status |= CUEByRemoteMIC[cntModule];
			}
		}
	}

	SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_FUNCTION;
	SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_CUE_BY_MIC_REMOTE;
	SerialBufferOut[SerialBufferOutPtrTop++] = Status;
}



char GetTelcoNrFromModuleNr(char ModuleNr)
{
	char cntTelco = -1;
	char TelcoNr = -1;
	char cntModule;
	
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if (TelcoNr == -1)
		{
			if (ModuleType[cntModule] == TELCO)
			{
				cntTelco++;
	
				if (cntModule == ModuleNr)
				{
					TelcoNr = cntTelco;
				}
			}
		}
	}

	return TelcoNr;
}



void ShowFunction(unsigned char Function)
{
	unsigned char cnt;
	for (cnt=0; cnt<NROFMODULES; cnt++)
	{
		ShowFunctionData[cnt] &= CUE_LED^0xFF;
		if (ConsoleModuleSettings[cnt][Function])
		{
			ShowFunctionData[cnt] |= CUE_LED;
		}
	}
	SendSerialData = 1;
}




void Init(void)
{
	unsigned int Address;
	unsigned char cntModule;
	unsigned char cntByte;
	unsigned char ClearMem = 0;

	EA = 0;

	// Watchdog
	PCON |= 0x10;
	T3 = 0;

  OperatingMode = NORMALMODE;
	DirectorTBCUEBussActive = 0;

	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		ConsoleFunctionData[cntModule] = MUTE_FUNCTION;
		ShowFunctionData[cntModule] = 0x00;
	}
	ConsoleFunctionData[16] = NOT_POWER_ON_CC;
	SetConsoleData();

	TBL_MUTE = 0xFF;
	CS_CC = 0x00;

  CLK1 = 0;
	CLK2 = 0;
	STRB1 = 1;	
	STRB2 = 1;

	OldTimeAndDate.Hours = 0;
	OldTimeAndDate.Minutes = 0;
	OldTimeAndDate.Seconds = 0;
	OldTimeAndDate.Frames = 0;
	TimeAndDate.Hours = 0;
	TimeAndDate.Minutes = 0;
	TimeAndDate.Seconds = 0;
	TimeAndDate.Frames = 0;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		TimerActive[cntModule] = 0;
		cntStartPuls[cntModule] = 0;
		cntStartFunction[cntModule] = 0;
		cntCueFunction[cntModule] = 0;
		cntMuteFunction[cntModule] = 0;
		StartActive[cntModule] = 0;
		CUEByRemoteMIC[cntModule] = 0;
	}
	GlobalTimerActive = 0;
	TextStringPtr = 0;

	SerialBufferInPtrTop = 0;
	SerialBufferInPtrBottom = 0;
	SerialBufferOutPtrTop = 0;
	SerialBufferOutPtrBottom = 0;
	SerialCnt = 0;
	SerialLength = 0;
	SendSerialReady = 1;

	I2CBufferInPtrTop = 0;
	I2CBufferInPtrBottom = 0;
	I2CBufferOutPtrTop = 0;
	I2CBufferOutPtrBottom = 0;
	I2CSendReady=1;
	I2CMasterReciever = 0;
	I2CError = 0;
	CardInserted = 0;

	SetupSIO1();

	SetupLCD();

	/* Set display */
	SetLCD(1,"   D&R Airlab   ");
	SetLCD(0,"\xC0");
	SetLCD(1,"  Initializing  ");

	NrOfTelcos = 0;
	NrOfTriples = 0;

 	// Watchdog
 	PCON |= 0x10;
 	T3 = 0;

	EA=1;
	AA=1;

	// First Load all data from EEPROM (or Card?)
	Address = 0x0000;
  ReadData(I2CAddressEEProm, Address, &(ModuleType[0]), NROFMODULES);
  Address += NROFMODULES;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		ReadData(I2CAddressEEProm, Address, &(ConsoleModuleSettings[cntModule][0]), 29);
	  Address += 29;

  	// Watchdog
  	PCON |= 0x10;
  	T3 = 0;
	}
	ReadData(I2CAddressEEProm, Address, &(ConsoleMasterSettings[0]), 5);
	
	
	


	
	

	EA=0;
	AA = 0;

	SetupSIO0();
	SetupTimer0();
	
	
	

	
	

	// Clear readbuffers
  for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		for (cntByte=0; cntByte<8; cntByte++)
		{
			ModuleData[cntModule][cntByte]=ATTACK;
		}
	}
	for (cntByte=0; cntByte<8; cntByte++)
	{
		MasterData[cntByte]=ATTACK;
	}

	Delay(10);

	for (cntModule=0; cntModule<100; cntModule++)
	{
		ReadModuleSwitches(MODULE1_8);
		ReadModuleSwitches(MODULE9_16);
  	// Watchdog
  	PCON |= 0x10;
  	T3 = 0;
	}
	EventBufferPtrTop = 0;
	EventBufferPtrBottom = 0;

	// Determine Nr Of Modules and Telco's
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		unsigned char Type = 0;

		if (ModuleData[cntModule][4]&0x80)
			Type = 0x02;
		if (ModuleData[cntModule][5]&0x80)
			Type |= 0x01;

		if (ModuleType[cntModule] != Type)
		{
			ModuleType[cntModule] = Type;
			SetLCD(0,"\xC0");
			SetLCD(1,"Config changed");
			Delay(20);

//			ConsoleModuleSettings[cntModule][TIMERLINEA] = 0x00;
//			ConsoleModuleSettings[cntModule][TIMERLINEB] = 0x00;
//			ConsoleModuleSettings[cntModule][TIMERMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][STARTLINEA] = 0x01;
//			ConsoleModuleSettings[cntModule][STARTLINEB] = 0x00;
//			ConsoleModuleSettings[cntModule][STARTMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][STARTFADER] = 0x01;
//			ConsoleModuleSettings[cntModule][STARTON] = 0x00;
//			ConsoleModuleSettings[cntModule][STARTCUE] = 0x00;
//			ConsoleModuleSettings[cntModule][STARTPULS] = 0x00;
//			ConsoleModuleSettings[cntModule][REMLINEA] = 0x00;
//			ConsoleModuleSettings[cntModule][REMLINEB] = 0x00;
//			ConsoleModuleSettings[cntModule][REMMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][DJMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][ANNMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][STUDIOMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][ONAIR1LINEA] = 0x00;
//			ConsoleModuleSettings[cntModule][ONAIR1LINEB] = 0x00;
//			ConsoleModuleSettings[cntModule][ONAIR1MIC] = 0x00;
//			ConsoleModuleSettings[cntModule][ONAIR2LINEA] = 0x00;
//			ConsoleModuleSettings[cntModule][ONAIR2LINEB] = 0x00;
//			ConsoleModuleSettings[cntModule][ONAIR2MIC] = 0x00;
//			ConsoleModuleSettings[cntModule][POWERON] = 0x00;
//			ConsoleModuleSettings[cntModule][MICATLINEA] = 0x00;
//			ConsoleModuleSettings[cntModule][MICATLINEB] = 0x00;
//			ConsoleModuleSettings[cntModule][REMONMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][REMONLINEA] = 0x00;
//			ConsoleModuleSettings[cntModule][REMONLINEB] = 0x00;
//			ConsoleModuleSettings[cntModule][SWITCHSTART] = 0x00;
//		}

//		if (ModuleType[cntModule] == TELCO)
//		{
//			ConsoleModuleSettings[cntModule][TIMERMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][STARTLINEA] = 0x01;
//			ConsoleModuleSettings[cntModule][STARTLINEB] = 0x01;
//			ConsoleModuleSettings[cntModule][STARTMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][STARTFADER] = 0x00;
//			ConsoleModuleSettings[cntModule][STARTON] = 0x01;
//			ConsoleModuleSettings[cntModule][STARTPULS] = 0x00;
//			ConsoleModuleSettings[cntModule][DJMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][ANNMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][STUDIOMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][ONAIR1MIC] = 0x00;
//			ConsoleModuleSettings[cntModule][ONAIR2MIC] = 0x00;
//			ConsoleModuleSettings[cntModule][MICATLINEA] = 0x00;
//			ConsoleModuleSettings[cntModule][MICATLINEB] = 0x00;
//			ConsoleModuleSettings[cntModule][REMONMIC] = 0x00;
//			ConsoleModuleSettings[cntModule][REMONLINEA] = 0x00;
//			ConsoleModuleSettings[cntModule][REMONLINEB] = 0x00;
//			ConsoleModuleSettings[cntModule][SWITCHSTART] = 0x00;
		}

   	// Watchdog
  	PCON |= 0x10;
  	T3 = 0;
	}
	
	ModuleType[0] = 1;
	ModuleType[1] = 1;
	ModuleType[2] = 1;
	ModuleType[3] = 1;
	ModuleType[4] = 1;
	ModuleType[5] = 1;
	ModuleType[6] = 1;
	ModuleType[7] = 1;
				//
	//
	ModuleType[8] = 1;
	ModuleType[9] = 1;
	ModuleType[10] = 1;
	ModuleType[11] = 1;
	ModuleType[12] = 1;
	ModuleType[13] = 1;
	ModuleType[14] = 1;
	ModuleType[15] = 1;	
	
	ConsoleMasterSettings[0]=1;
	ConsoleMasterSettings[1]=1;
	ConsoleMasterSettings[2]=1;
	ConsoleMasterSettings[3]=1;
	ConsoleMasterSettings[4]=1;

	// Determine Startup setting
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		ConsoleFunctionData[cntModule] = 0x00;
		if (ConsoleModuleSettings[cntModule][POWERON])
		{
			ConsoleFunctionData[cntModule] |= ON_LED;
			if ((ConsoleModuleSettings[cntModule][STARTON]) && (!ConsoleModuleSettings[cntModule][STARTFADER]))
				StartControl(cntModule,1);

			if ((ModuleData[cntModule][FADERON]&0x80))						
			{ // !JB
				if  ((ConsoleModuleSettings[cntModule][STARTON]) && (ConsoleModuleSettings[cntModule][STARTFADER]))
					StartControl(cntModule,1);

				ConsoleFunctionData[cntModule] |= REMOTE_FUNCTION;
			}
			else
			{
				ConsoleFunctionData[cntModule] |= MUTE_FUNCTION;
			}
		}
		else
		{
			ConsoleFunctionData[cntModule] |= MUTE_FUNCTION;
		}
  	// Watchdog
  	PCON |= 0x10;
  	T3 = 0;
	}
	ConsoleFunctionData[16] = NOT_POWER_ON_CC;

	for (cntModule; cntModule<NROFMODULES; cntModule++)
	{
		TimerControl(cntModule);
  	// Watchdog
  	PCON |= 0x10;
  	T3 = 0;
	}

	//OnAir1/2 Control
	OnAirControl();

	//DJ-Control
	DJControl();

	//StudioMic Control
	StudioMicControl();

	//Cue Control
	CueControl();	

	SetConsoleData();


	// Determine Nr Of Modules and Telco's
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		switch (ModuleType[cntModule])
		{
			case TRIPLE:
			{
				// Triple Module
				NrOfTriples++;
			}
			break;
			case TELCO:
			{
				// Telco Module
				NrOfTelcos++;
			}
			break;
		}
  	// Watchdog
  	PCON |= 0x10;
  	T3 = 0;
	}

	/* Set display */
	SetLCD(0,"\x80");
	SetLCD(1,"   D&R Airlab   ");
	ShowConfig();

	Delay(50);
	P10 = 0; //read master
	P11 = 0; 
	P12 = 0; 
	P13 = 0; 
	P14 = 1; 
	if ((P5&0x43)==0x00)
	{
		ConsoleMasterSettings[4] = 0x01;
		SetLCD(0,"\xC0");
		SetLCD(1,"Security off    ");
	}

	Delay(50);
	SetLCD(0,"\x80");
	SetLCD(1,"                ");
	SetLCD(0,"\xC0");
	SetLCD(1,TextString[TextStringPtr]);

 	// Watchdog
 	PCON |= 0x10;
 	T3 = 0;

	EA=1;
	AA = 1;

	SetAddress(I2CAddressEEProm, 0x00);
	
	
//	while(1)
//	{
//		//S0BUF = EventBuffer[EventBufferPtrBottom];
//		
//		//S0BUF = 0x77;
//		
//		int t;
//		
//		for( t=0; t<5; t++)
//		{
//			S0BUF = ConsoleMasterSettings[t];
//			Delay(10);
//		}
//		
//		for( t=0; t<29; t++)
//		{
//			S0BUF = ConsoleModuleSettings[t][0];
//			Delay(10);
//		}	


//		WriteData(I2CAddressEEProm, Address, ConsoleMasterSettings, 5);
//		
//		while(1)
//			;
//			
//	}	

	
	
}

//=============================================================================================================
void main(void)
{
	char TextBuffer[9];

	Init();

	P10 = 0; //read master
	P11 = 0; 
	P12 = 0; 
	P13 = 0; 
	P14 = 1; 
	if ((P5&0x3C)==0x00)
	{
		unsigned char Stop=0;
		SetLCD(0,"\x80");
		SetLCD(1,"Test mode       ");
		SetLCD(0,"\xC0");
		SetLCD(1,"                ");

		while (!Stop)
		{
			unsigned char cnt;

			// Watchdog
			PCON |= 0x10;
			T3 = 0;
	

			for (cnt=0; cnt<NROFMODULES; cnt++)
			{
				if (cntStartPuls[cnt])
				{
					cntStartPuls[cnt]--;
					if (cntStartPuls[cnt]==0)
					{
						ConsoleFunctionData[cnt] &= START_FUNCTION^0xFF;		
						SendSerialData = 1;
					}
				}
			}
		
			if (SendSerialData)
			{
				SendSerialData = 0;
				SetConsoleData();
			}

			ReadConsole();
			if (EventBufferPtrTop != EventBufferPtrBottom)
			{
				unsigned int Event = EventBuffer[EventBufferPtrBottom++];
				unsigned char EventType = (Event&0x87) | ((Event&0x200)>>6);
				unsigned char Module = (Event>>3)&0x0F;
				unsigned char EventOn = !(Event&0x100);
				char TestString2[17] = "Off at Module xx";
				if (EventOn)
				{
					TestString2[0]=' ';
					TestString2[1]='O';
					TestString2[2]='n';
				}
				TestString2[14] = ((Module+1)/10)+0x30;
				TestString2[15] = ((Module+1)%10)+0x30;

				switch (EventType)
				{
					case EXTERNREMOTEIN:
					{
						char TestString1[17] = "'Ext. remote In'";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
						ConsoleFunctionData[Module] |= START_FUNCTION;		
						cntStartPuls[Module] = PULSWIDTH;
						ConsoleFunctionData[Module] ^= CUE_LED;
						ConsoleFunctionData[Module] ^= REMOTE_FUNCTION;
    				SetConsoleData();
					}												 
					break;
					case FADERON:
					{
						char TestString1[17] = "'Fader On'      ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
					}
					break;
					case PFL:
					{
						char TestString1[17] = "'CUE'           ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
					}
					break;
					case ON:
					{
						char TestString1[17] = "'ON'            ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
					}
					break;
					case LINEB:
					{
						char TestString1[17] = "'LINE B'        ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
					}
					break;
					case MIC_RING:
					{
						char TestString1[17] = "'MIC/RING'      ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
					}
					break;
					case ESCAPE:
					{
						char TestString1[17] = "'ESCAPE'        ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
						TestString2[3]=0;
					}
					break;
					case ENTER:
					{
						char TestString1[17] = "'ENTER'         ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
						TestString2[3]=0;
					}
					break;
					case RIGHT:
					{
						char TestString1[17] = "'RIGHT'         ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
						TestString2[3]=0;
					}
					break;
					case UP:
					{
						char TestString1[17] = "'UP'            ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
						TestString2[3]=0;
					}
					break;
					case DOWN:
					{
						char TestString1[17] = "'DOWN'          ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
						TestString2[3]=0;
					}
					break;
					case LEFT:
					{
						char TestString1[17] = "'LEFT'          ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
						TestString2[3]=0;
					}
					break;
					case CUERESET:
					{
						char TestString1[17] = "'CUE RESET'     ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
						TestString2[3]=0;
			 		}
					break;
					case SW_CC:
					{
						char TestString1[17] = "'Card Switch'   ";
						SetLCD(0,"\x80");
						SetLCD(1,TestString1);
						TestString2[3]=0;
					}
					break;
				}
				SetLCD(0,"\xC0");
				SetLCD(1,TestString2);
			}

			P10 = 0; //read master
			P11 = 0; 
			P12 = 0; 
			P13 = 0; 
			P14 = 1; 
			if ((P5&0x43)==0x00)
			{
				Stop = 1;

				SetLCD(0,"\x80");
				SetLCD(1,"                ");
				SetLCD(0,"\xC0");
				SetLCD(1,"                ");

				for (cnt=0; cnt<NROFMODULES; cnt++)
				{
					ConsoleFunctionData[cnt] &= START_FUNCTION^0xFF;		
					cntStartPuls[cnt] = 0;
					ConsoleFunctionData[cnt] &= CUE_LED^0xFF;
					ConsoleFunctionData[cnt] &= CUE_FUNCTION^0xFF;
				}
			}
		}
	}

	TextBuffer[0] = 0x30+(TimeAndDate.Hours/10);
	TextBuffer[1] = 0x30+(TimeAndDate.Hours%10);
	TextBuffer[2] = ':';
  TextBuffer[3] = 0x30+(TimeAndDate.Minutes/10);
	TextBuffer[4] = 0x30+(TimeAndDate.Minutes%10);
	TextBuffer[5] = ':';
	TextBuffer[6] = 0x30+(TimeAndDate.Seconds/10);
	TextBuffer[7] = 0x30+(TimeAndDate.Seconds%10);
/*	TextBuffer[8] = ':';
	TextBuffer[9] = 0x30+(TimeAndDate.Frames/10);
	TextBuffer[10] = 0x30+(TimeAndDate.Frames%10);
	TextBuffer[11] = 0;*/
	SetLCD(0,"\x84");
	TextBuffer[8] = 0;
	SetLCD(1,TextBuffer);

									{//Reload
										unsigned char cntByte, cntChannel;

										for (cntChannel=0; cntChannel<16; cntChannel++)
										{
  										if ((ModuleType[cntChannel]==0x01) || (ModuleType[cntChannel]==0x02))
											{
												SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MODULESETUPDATA;
												SerialBufferOut[SerialBufferOutPtrTop++] = cntChannel;
												for (cntByte=0; cntByte<29; cntByte++)
												{
													ConsoleModuleSettings[cntChannel][cntByte]&=0x01;
													SerialBufferOut[SerialBufferOutPtrTop++] =
													ConsoleModuleSettings[cntChannel][cntByte]&0x7F;
									 		  }
											}
											if ((SerialBufferOutPtrTop != SerialBufferOutPtrBottom) && SendSerialReady)
											{
												S0BUF = SerialBufferOut[SerialBufferOutPtrBottom++];
												SendSerialReady = 0;
											}
											while(!SendSerialReady);
										}
	
										SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MASTERSETUPDATA;
										SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
										for (cntByte=0; cntByte<5; cntByte++)
										{
											SerialBufferOut[SerialBufferOutPtrTop++] = ConsoleMasterSettings[cntByte]&0x7F;
										}
									}
									
						
									
	//SerialBufferOutPtrTop = 1;  // Dit schijnt belangrijk te zijn
									
	while(1)
	{
		unsigned char cnt;

		for (cnt=0; cnt<NROFMODULES; cnt++)
		{
			if (cntStartPuls[cnt])
			{
				cntStartPuls[cnt]--;
				if (cntStartPuls[cnt]==0)
				{
					ConsoleFunctionData[cnt] &= START_FUNCTION^0xFF;		
					SendSerialData = 1;
				}
			}

			if (cntStartFunction[cnt])
			{
				cntStartFunction[cnt]--;
				if (cntStartFunction[cnt]==0)
				{
					ConsoleFunctionData[cnt] &= START_FUNCTION^0xFF;
					SendSerialData = 1;
				}
			}
			if (cntCueFunction[cnt])
			{
				cntCueFunction[cnt]--;
				if (cntCueFunction[cnt]==0)
				{
					ConsoleFunctionData[cnt] |= CUE_FUNCTION;
					CueControl();
					SendSerialData = 1;
				}
			}
			if (cntMuteFunction[cnt])
			{
				cntMuteFunction[cnt]--;
				if (cntMuteFunction[cnt]==0)
				{
					char TelcoNr = GetTelcoNrFromModuleNr(cnt);

					ConsoleFunctionData[cnt] &= MUTE_FUNCTION^0xFF;

					if ((TelcoNr>=0) && (TelcoNr<4))
					{
						SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_MUTE;
						SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
						SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
					}

					SendSerialData = 1;
				}
			}
		}
		//---------------------------------------------------------------------------------------
		if (SendSerialData)
		{
			SendSerialData = 0;
			SetConsoleData();
		}
		

		if ((TimeAndDate.Seconds != OldTimeAndDate.Seconds) && (OperatingMode==NORMALMODE))
		{
			TextBuffer[0] = 0x30+(TimeAndDate.Hours/10);
			TextBuffer[1] = 0x30+(TimeAndDate.Hours%10);
			TextBuffer[2] = ':';
		  TextBuffer[3] = 0x30+(TimeAndDate.Minutes/10);
			TextBuffer[4] = 0x30+(TimeAndDate.Minutes%10);
			TextBuffer[5] = ':';
			TextBuffer[6] = 0x30+(TimeAndDate.Seconds/10);
			TextBuffer[7] = 0x30+(TimeAndDate.Seconds%10);
/*		TextBuffer[8] = ':';
			TextBuffer[9] = 0x30+(TimeAndDate.Frames/10);
			TextBuffer[10] = 0x30+(TimeAndDate.Frames%10);
			TextBuffer[11] = 0;*/
			TextBuffer[8] = 0;
			SetLCD(0,"\x84");
			SetLCD(1,TextBuffer);
			OldTimeAndDate.Seconds = TimeAndDate.Seconds;
		}

		// Watchdog
		PCON |= 0x10;
		T3 = 0;
		
		
		ReadConsole();
		
		
		if (EventBufferPtrTop != EventBufferPtrBottom)
		{
			//S0BUF = EventBuffer[EventBufferPtrBottom];    // Aha hier weet je nu welke schakelaar er is ingedrukt
			//DoEvent(EventBuffer[EventBufferPtrBottom]);	
			DoEvent(EventBuffer[EventBufferPtrBottom++]);				
			//S0BUF =EventBufferPtrTop;
			//S0BUF =EventBufferPtrBottom;
			//EventBufferPtrBottom++;			
		}

		//ConsoleMasterSettings[3]=1;  // is nodig zodat RS232 communicatie altijd mogelijk is
		
		if (ConsoleMasterSettings[3])
		{
			if (SerialBufferInPtrTop != SerialBufferInPtrBottom)
				DoSerial();

			if ((SerialBufferOutPtrTop != SerialBufferOutPtrBottom) && SendSerialReady)
			{
				S0BUF = SerialBufferOut[SerialBufferOutPtrBottom++];
				SendSerialReady = 0;
			}
		}
		else
		{
			SerialBufferInPtrTop = SerialBufferInPtrBottom = SerialBufferOutPtrTop = SerialBufferOutPtrBottom = 0;
		}
	}
}


//___________________________________________________________________________________________________________


void CueControl()
{
	unsigned char cntModule;
	unsigned char CueActive=0;

	ConsoleFunctionData[16] &= MASTER_CUE_ON_FUNCTION_LED^0xFF;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if ((ConsoleFunctionData[cntModule]&CUE_FUNCTION) && ((ModuleType[cntModule]==TRIPLE) || (ModuleType[cntModule]==TELCO)))
			CueActive = 1;
	}

	if (DirectorTBCUEBussActive != 0)
	{
		CueActive = 1;
	}

	if ((CueActive) && (ConsoleMasterSettings[CUETOCRM]))
		ConsoleFunctionData[16] |= MASTER_CUE_ON_FUNCTION_LED;

	//Check auto communication
	ConsoleFunctionData[16] &= ANNOUNCER_COMM_FUNCTION_LED^0xFF;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if (ConsoleFunctionData[cntModule]&CUE_LED)
		{
			if ((ModuleData[cntModule][MIC_RING]&0x80) && (ConsoleModuleSettings[cntModule][ANNMIC]) && (ConsoleFunctionData[cntModule]&MUTE_FUNCTION) && (ConsoleMasterSettings[AUTOCOM]))
			{
				ConsoleFunctionData[16] |= ANNOUNCER_COMM_FUNCTION_LED;
			}
		}
	}
	CheckTBMic();

	SendSerialData = 1;
}

void CheckTBMic()
{
	char TBByTelco = 0;
	char cntModule;

	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if (ModuleType[cntModule] == TELCO)
		{
			if (ConsoleFunctionData[cntModule]&CUE_FUNCTION)
			{
				TBByTelco = 1;
			}
		}
	}

	if ((ConsoleFunctionData[16]&ANNOUNCER_COMM_FUNCTION_LED) || (TBByTelco))
	{
		TBL_MUTE = 0x00;
	}
	else
	{
		TBL_MUTE = 0xFF;
	}
}


void StudioMicControl()
{
	unsigned char cntModule;
	unsigned char StudioMicActive=0;

	ConsoleFunctionData[16] &= STUDIOMUTE_FUNCTION_LED^0xFF;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if (((ConsoleModuleSettings[cntModule][STUDIOMIC]) || (ConsoleModuleSettings[cntModule][ANNMIC])) && ((ModuleType[cntModule]==TRIPLE) || (ModuleType[cntModule]==TELCO)))
		{
//v1.11
/*			if (!MIC_AND_LINE_MUTE)
			{
				if (ModuleData[cntModule][MIC_RING]&0x80)
				{
					if (((ModuleData[cntModule][FADERON]&0x80)) && ((ConsoleFunctionData[cntModule]&ON_LED)))
						StudioMicActive = 1; // !JB
				}
			}
			else
			{
				if (((ModuleData[cntModule][FADERON]&0x80)) && ((ConsoleFunctionData[cntModule]&ON_LED)))
					StudioMicActive = 1; // !JB
			}*/
			if ((ModuleData[cntModule][MIC_RING]&0x80) ||
				 ((!(ModuleData[cntModule][LINEB]&0x80)) && (ConsoleModuleSettings[cntModule][MICATLINEA])) ||
				 (((ModuleData[cntModule][LINEB]&0x80)) && (ConsoleModuleSettings[cntModule][MICATLINEB])))
			{
				if (((ModuleData[cntModule][FADERON]&0x80)) && ((ConsoleFunctionData[cntModule]&ON_LED)))
					StudioMicActive = 1; // !JB
			}
		}
	}

	if (StudioMicActive)
		ConsoleFunctionData[16] |= STUDIOMUTE_FUNCTION_LED;		

	if (ConsoleFunctionData[16]&STUDIOMUTE_FUNCTION_LED)
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_FUNCTION;
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_STUDIOMUTE;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
	}
	else
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_FUNCTION;
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_STUDIOMUTE;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
	}

	SendSerialData = 1;
}







void DJControl()
{
	unsigned char cntModule;
	unsigned char DJMicActive=0;

	ConsoleFunctionData[16] &= CRMMUTE_FUNCTION_LED^0xFF;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if ((ConsoleModuleSettings[cntModule][DJMIC]) && ((ModuleType[cntModule]==TRIPLE) || (ModuleType[cntModule]==TELCO)))
		{
//v1.11
/*			if (!MIC_AND_LINE_MUTE)
			{
				if (ModuleData[cntModule][MIC_RING]&0x80)
				{
					if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED) || (ConsoleFunctionData[cntModule]&CUE_FUNCTION))
						DJMicActive = 1; // !JB
				}
			}
			else
			{
				if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED) || (ConsoleFunctionData[cntModule]&CUE_FUNCTION))
					DJMicActive = 1; // !JB
			}
		}*/
//v1.15
			if ((ModuleData[cntModule][MIC_RING]&0x80) ||
				 ((!(ModuleData[cntModule][LINEB]&0x80)) && (ConsoleModuleSettings[cntModule][MICATLINEA])) ||
				 (((ModuleData[cntModule][LINEB]&0x80)) && (ConsoleModuleSettings[cntModule][MICATLINEB])))
			{
				if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED) || (ConsoleFunctionData[cntModule]&CUE_FUNCTION))
					DJMicActive = 1; // !JB
			}
		}
	}

	if (DJMicActive)
		ConsoleFunctionData[16] |= CRMMUTE_FUNCTION_LED;		

	if (ConsoleFunctionData[16]&CRMMUTE_FUNCTION_LED)
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_FUNCTION;
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_CRMMUTE;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
	}
	else
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_FUNCTION;
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_CRMMUTE;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
	}
	SendSerialData = 1;
}


void OnAirControl()
{
	unsigned char cntModule;
	unsigned char OnAir1=0;
	unsigned char OnAir2=0;

	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if  ((ModuleType[cntModule]==TRIPLE) || (ModuleType[cntModule]==TELCO))
		{
			if (ModuleData[cntModule][MIC_RING]&0x80)
			{
				if (ConsoleModuleSettings[cntModule][ONAIR1MIC])
					if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED))
						OnAir1 = 1;	 // !JB
				if (ConsoleModuleSettings[cntModule][ONAIR2MIC])
					if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED))
						OnAir2 = 1;	 // !JB
			}
			else if (!(ModuleData[cntModule][LINEB]&0x80))
			{
				if (ConsoleModuleSettings[cntModule][ONAIR1LINEA])
					if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED))
						OnAir1 = 1;	// !JB
				if (ConsoleModuleSettings[cntModule][ONAIR2LINEA])
					if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED))
						OnAir2 = 1;	// !JB
			}
			else if (ModuleData[cntModule][LINEB]&0x80)
			{
				if (ConsoleModuleSettings[cntModule][ONAIR1LINEB])
					if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED))
						OnAir1 = 1;	 // !JB
				if (ConsoleModuleSettings[cntModule][ONAIR2LINEB])
					if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED))
						OnAir2 = 1;	// !JB
			}
		}
	}
		
	ConsoleFunctionData[16] &= ONAIR1_FUNCTION^0xFF;
	ConsoleFunctionData[16] &= ONAIR2_FUNCTION^0xFF;
	if (OnAir1)
		ConsoleFunctionData[16] |= ONAIR1_FUNCTION;
	if (OnAir2)
		ConsoleFunctionData[16] |= ONAIR2_FUNCTION;

	if (ConsoleFunctionData[16]&ONAIR1_FUNCTION)
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_ONAIR1;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
	}
	else
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_ONAIR1;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
  }

	if (ConsoleFunctionData[16]&ONAIR2_FUNCTION)
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_ONAIR2;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
	}
	else
	{
	 	SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_ONAIR2;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00; 
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
	}
	SendSerialData = 1;
}


void StartControl(unsigned char Module, unsigned char On)
{
	unsigned char Status = 0;

	ConsoleFunctionData[Module] &= START_FUNCTION^0xFF;
	if (On)
	{
		if (ModuleData[Module][MIC_RING]&0x80)
		{
			if (ConsoleModuleSettings[Module][STARTMIC])
				ConsoleFunctionData[Module] |= START_FUNCTION;
		}
		else if (!(ModuleData[Module][LINEB]&0x80))
		{
			if (ConsoleModuleSettings[Module][STARTLINEA])
				ConsoleFunctionData[Module] |= START_FUNCTION;		
		}
		else if (ModuleData[Module][LINEB]&0x80)
		{
			if (ConsoleModuleSettings[Module][STARTLINEB])
				ConsoleFunctionData[Module] |= START_FUNCTION;		
		}
	}
	
	cntStartPuls[Module] = 0;
	if ((ConsoleFunctionData[Module]&START_FUNCTION) && (ConsoleModuleSettings[Module][STARTPULS]))
	{
		cntStartPuls[Module] = PULSWIDTH;
	}

	if (ConsoleFunctionData[Module]&START_FUNCTION)
	{
			SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_START;
			SerialBufferOut[SerialBufferOutPtrTop++] = Module;
			SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
	}
	else
	{
			SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_STOP;
			SerialBufferOut[SerialBufferOutPtrTop++] = Module;
			SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
	}
	SendSerialData = 1;
}



char GetModuleNrFromTelcoNr(char TelcoNr)
{
	char cntTelco = -1;
	char ModuleNr = -1;
	char cntModule;
	
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if (ModuleNr == -1)
		{
			if (ModuleType[cntModule] == TELCO)
			{
				cntTelco++;
			}

			if (cntTelco == TelcoNr)
			{
				ModuleNr = cntModule;
			}
		}
	}

	return ModuleNr;
}



void TimerControl(unsigned char Module)
{
	unsigned char SetTimerOn = 0;
	unsigned char cntModule;

	if (ModuleData[Module][MIC_RING]&0x80)
	{
		if (ConsoleModuleSettings[Module][TIMERMIC])
			SetTimerOn = 1;		
	}
	else if (!(ModuleData[Module][LINEB]&0x80))
	{
		if (ConsoleModuleSettings[Module][TIMERLINEA])
			SetTimerOn = 1;
	}
	else if (ModuleData[Module][LINEB]&0x80)
	{
		if (ConsoleModuleSettings[Module][TIMERLINEB])
			SetTimerOn = 1;
	}

	if (SetTimerOn)
	{
		if (((ModuleData[Module][FADERON]&0x80)) && ((ConsoleFunctionData[Module]&ON_LED))) 	 
		{// !JB
			TimeAndDate.Hours = 0;
			TimeAndDate.Minutes = 0;
			TimeAndDate.Seconds = 0;
			TimeAndDate.Frames = 0;
			TimerActive[Module] = 1;
		}
		else
		{
			TimerActive[Module] = 0;
		}
	}
	else
	{
		TimerActive[Module] = 0;
	}

	GlobalTimerActive = 0;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
			if (TimerActive[cntModule])
				GlobalTimerActive = 1;
	}
}


/***************************************************************
* SetConsoleData                                               *
***************************************************************/

void SetConsoleData()
{
	unsigned char cntByte, cntBit;  // byte   bits
	
	for (cntBit=0; cntBit<8; cntBit++)	// 8 bits
	{
		DATA1 = ConsoleFunctionData[16]&(0x80>>cntBit);   // AND de data met 1000 0000 . Data staat in :   ConsoleFunctionData[16]
		CLK1 = 1;																					// Klok hoog
		for (Wait=0; Wait<SET_SERIAL_WAIT; Wait++);				// Wacht 
		CLK1 = 0;																					// Klok laag
	}
	for (cntByte=0; cntByte<8; cntByte++)
	{
		for (cntBit=0; cntBit<6; cntBit++)								// 8 * 6 = 48
		{
			if ((OperatingMode==MODULEMODE) && ((0x01<<cntBit)&CUE_LED))   // Als betreffende module AND CUE_LED=1
			{
				DATA1 = ShowFunctionData[7-cntByte]&(0x01<<cntBit);						// ShowFunctionData
				DATA2 = ShowFunctionData[15-cntByte]&(0x01<<cntBit);
			}
			else
			{
				DATA1 = ConsoleFunctionData[7-cntByte]&(0x01<<cntBit);        // ConsoleFunctionData
				DATA2 = ConsoleFunctionData[15-cntByte]&(0x01<<cntBit);
			}
			CLK1 = 1;
			CLK2 = 1;
			for (Wait=0; Wait<SET_SERIAL_WAIT; Wait++);		// Wacht
			CLK1 = 0;
			CLK2 = 0;
		}
	}
	STRB1 = 0;
	for (Wait=0; Wait<SET_SERIAL_WAIT; Wait++);
  STRB1 = 1;	
	STRB2 = 0;
	for (Wait=0; Wait<SET_SERIAL_WAIT; Wait++);
	STRB2 = 1;
}




