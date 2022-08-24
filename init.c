/***************************************************************
* InitConsole                                             
***************************************************************/

//#include "reg552.h"
////#include "definitions.h"
//#include "Delay.h"

//#define	NORMALMODE					0

//unsigned char OperatingMode;

//void Init(void)
//{
//	unsigned int Address;
//	unsigned char cntModule;
//	unsigned char cntByte;
//	unsigned char ClearMem = 0;

//	EA = 0;

//	// Watchdog
//	PCON |= 0x10;
//	T3 = 0;

//  OperatingMode = NORMALMODE;
//	DirectorTBCUEBussActive = 0;

//	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
//	{
//		ConsoleFunctionData[cntModule] = MUTE_FUNCTION;
//		ShowFunctionData[cntModule] = 0x00;
//	}
//	ConsoleFunctionData[16] = NOT_POWER_ON_CC;
//	SetConsoleData();

//	TBL_MUTE = 0xFF;
//	CS_CC = 0x00;

//  CLK1 = 0;
//	CLK2 = 0;
//	STRB1 = 1;	
//	STRB2 = 1;

//	OldTimeAndDate.Hours = 0;
//	OldTimeAndDate.Minutes = 0;
//	OldTimeAndDate.Seconds = 0;
//	OldTimeAndDate.Frames = 0;
//	TimeAndDate.Hours = 0;
//	TimeAndDate.Minutes = 0;
//	TimeAndDate.Seconds = 0;
//	TimeAndDate.Frames = 0;
//	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
//	{
//		TimerActive[cntModule] = 0;
//		cntStartPuls[cntModule] = 0;
//		cntStartFunction[cntModule] = 0;
//		cntCueFunction[cntModule] = 0;
//		cntMuteFunction[cntModule] = 0;
//		StartActive[cntModule] = 0;
//		CUEByRemoteMIC[cntModule] = 0;
//	}
//	GlobalTimerActive = 0;
//	TextStringPtr = 0;

//	SerialBufferInPtrTop = 0;
//	SerialBufferInPtrBottom = 0;
//	SerialBufferOutPtrTop = 0;
//	SerialBufferOutPtrBottom = 0;
//	SerialCnt = 0;
//	SerialLength = 0;
//	SendSerialReady = 1;

//// 	I2CBufferInPtrTop = 0;
////	I2CBufferInPtrBottom = 0;
////	I2CBufferOutPtrTop = 0;
////	I2CBufferOutPtrBottom = 0;
////	I2CSendReady=1;
////	I2CMasterReciever = 0;
////	I2CError = 0;
////	CardInserted = 0;

//	SetupSIO1();

//	SetupLCD();

//	/* Set display */
//	SetLCD(1,"   D&R Airlab   ");
//	SetLCD(0,"\xC0");
//	SetLCD(1,"  Initializing  ");

//	NrOfTelcos = 0;
//	NrOfTriples = 0;

// 	// Watchdog
// 	PCON |= 0x10;
// 	T3 = 0;

//	EA=1;
//	AA=1;
//	
//	

//	// First Load all data from EEPROM (or Card?)
//	Address = 0x0000;
//  ReadData(I2CAddressEEProm, Address, &(ModuleType[0]), NROFMODULES);
//  Address += NROFMODULES;
//	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
//	{
//		ReadData(I2CAddressEEProm, Address, &(ConsoleModuleSettings[cntModule][0]), 29);
//	  Address += 29;

//  	// Watchdog
//  	PCON |= 0x10;
//  	T3 = 0;
//	}
//	ReadData(I2CAddressEEProm, Address, &(ConsoleMasterSettings[0]), 5);
//	
//	

//	EA=0;
//	AA = 0;

//	SetupSIO0();
//	SetupTimer0();

//	// Clear readbuffers
//  for (cntModule=0; cntModule<NROFMODULES; cntModule++)
//	{
//		for (cntByte=0; cntByte<8; cntByte++)
//		{
//			ModuleData[cntModule][cntByte]=ATTACK;
//		}
//	}
//	for (cntByte=0; cntByte<8; cntByte++)
//	{
//		MasterData[cntByte]=ATTACK;
//	}

//	Delay(10);

//	for (cntModule=0; cntModule<100; cntModule++)
//	{
//		ReadModuleSwitches(MODULE1_8);
//		ReadModuleSwitches(MODULE9_16);
//		ReadMasterSwitches();  // willem
//  	// Watchdog
//  	PCON |= 0x10;
//  	T3 = 0;
//	}
//	EventBufferPtrTop = 0;
//	EventBufferPtrBottom = 0;

//	// Determine Nr Of Modules and Telco's
//	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
//	{
//		unsigned char Type = 0;

//		if (ModuleData[cntModule][4]&0x80)
//			Type = 0x02;
//		if (ModuleData[cntModule][5]&0x80)
//			Type |= 0x01;

//		if (ModuleType[cntModule] != Type)
//		{
//			ModuleType[cntModule] = Type;
//			SetLCD(0,"\xC0");
//			SetLCD(1,"Config changed");
//			Delay(20);

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
//		}

//   	// Watchdog
//  	PCON |= 0x10;
//  	T3 = 0;
//	}

//	// Determine Startup setting
//	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
//	{
//		ConsoleFunctionData[cntModule] = 0x00;
//		if (ConsoleModuleSettings[cntModule][POWERON])
//		{
//			ConsoleFunctionData[cntModule] |= ON_LED;
//			if ((ConsoleModuleSettings[cntModule][STARTON]) && (!ConsoleModuleSettings[cntModule][STARTFADER]))
//				StartControl(cntModule,1);

//			if ((ModuleData[cntModule][FADERON]&0x80))						
//			{ // !JB
//				if  ((ConsoleModuleSettings[cntModule][STARTON]) && (ConsoleModuleSettings[cntModule][STARTFADER]))
//					StartControl(cntModule,1);

//				ConsoleFunctionData[cntModule] |= REMOTE_FUNCTION;
//			}
//			else
//			{
//				ConsoleFunctionData[cntModule] |= MUTE_FUNCTION;
//			}
//		}
//		else
//		{
//			ConsoleFunctionData[cntModule] |= MUTE_FUNCTION;
//		}
//  	// Watchdog
//  	PCON |= 0x10;
//  	T3 = 0;
//	}
//	ConsoleFunctionData[16] = NOT_POWER_ON_CC;

//	for (cntModule; cntModule<NROFMODULES; cntModule++)
//	{
//		TimerControl(cntModule);
//  	// Watchdog
//  	PCON |= 0x10;
//  	T3 = 0;
//	}

//	//OnAir1/2 Control
//	OnAirControl();

//	//DJ-Control
//	DJControl();

//	//StudioMic Control
//	StudioMicControl();

//	//Cue Control
//	CueControl();	

//	SetConsoleData();


//	// Determine Nr Of Modules and Telco's
//	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
//	{
//		switch (ModuleType[cntModule])
//		{
//			case TRIPLE:
//			{
//				// Triple Module
//				NrOfTriples++;
//			}
//			break;
//			case TELCO:
//			{
//				// Telco Module
//				NrOfTelcos++;
//			}
//			break;
//		}
//  	// Watchdog
//  	PCON |= 0x10;
//  	T3 = 0;
//	}

//	/* Set display */
//	SetLCD(0,"\x80");
//	SetLCD(1,"   D&R Airlab   ");
//	ShowConfig();

//	Delay(50);
//	P10 = 0; //read master
//	P11 = 0; 
//	P12 = 0; 
//	P13 = 0; 
//	P14 = 1; 
//	if ((P5&0x43)==0x00)
//	{
//		ConsoleMasterSettings[4] = 0x01;
//		SetLCD(0,"\xC0");
//		SetLCD(1,"Security off    ");
//	}

//	Delay(50);
//	SetLCD(0,"\x80");
//	SetLCD(1,"                ");
//	SetLCD(0,"\xC0");
//	SetLCD(1,TextString[TextStringPtr]);

// 	// Watchdog
// 	PCON |= 0x10;
// 	T3 = 0;

//	EA=1;
//	AA = 1;

//	SetAddress(I2CAddressEEProm, 0x00);
//}
