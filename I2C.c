#include "reg552.h"
#include "Delay.h"
#include "LCD.h"
#include "airlabbrm-willem.h"

unsigned char I2CBufferIn[256];
unsigned char I2CStatus;
unsigned char CurrentAddress;

unsigned char I2CBuffer[256];
unsigned char I2CLength;
unsigned char I2CDataLength;
unsigned char I2CRepeat;
unsigned char I2CRepeatAddress;
unsigned char I2CByteNr;
unsigned char I2CFromAddress;
unsigned char I2CItem;
unsigned char I2CModule;

unsigned char I2CMasterReciever;
unsigned char I2CBufferOutPtrTop;
unsigned char I2CBufferInPtrTop;
unsigned char I2CBufferInPtrBottom;
unsigned char I2CBufferOutPtrBottom;
unsigned char I2CSendReady;
unsigned char I2CError;

unsigned char I2CAddressBufferOut[256];
unsigned char I2CDataBufferOut[256];





//#ifndef __I2C_H__
//#define __I2C_H__
//	I2CBufferInPtrTop = 0;
//	I2CBufferInPtrBottom = 0;
//	I2CBufferOutPtrTop = 0;
//	I2CBufferOutPtrBottom = 0;
//	I2CSendReady=1;
//	I2CMasterReciever = 0;
//	I2CError = 0;
//	CardInserted = 0;
//#endif

//void SetAddress(unsigned char I2CAddress, unsigned int StartAddress);
//void SetupSIO1();

/***************************************************************
* Setup_I2C                                                    *
***************************************************************/
//	EA = 0;  // EA=IE0....7 (Interrupt Enable) Register  EA 	IE.7 	It disables all interrupts. When EA = 0 no interrupt will be acknowledged and EA = 1 enables the interrupt individually.
						//- 	IE.6 	Reserved for future use.
						//- 	IE.5 	Reserved for future use.
						//ES 	IE.4 	Enables/disables serial port interrupt.
						//ET1 	IE.3 	Enables/disables timer1 overflow interrupt.
						//EX1 	IE.2 	Enables/disables external interrupt1.
						//ET0 	IE.1 	Enables/disables timer0 overflow interrupt.
						//EX0 	IE.0 	Enables/disables external interrupt0.


void SetupSIO1()
{
	
	I2CBufferInPtrTop = 0;
	I2CBufferInPtrBottom = 0;
	I2CBufferOutPtrTop = 0;
	I2CBufferOutPtrBottom = 0;
	I2CSendReady=1;
	I2CMasterReciever = 0;
	I2CError = 0;
	CardInserted = 0;
	
////		S1ADR = 0xFF;  // Adressregister

////	P16 = 1;		// set I2Cports // SCL (P1.6): Serial port clock line I2 C-bus.
////	P17 = 1;                     // SDA (P1.7): Serial port data line I2 C-bus.

////	SI = 0;		// reset Serial Interrupt Flag   
////	AA = 0;		// disable masterinterrupt    ( = Assert Acknowledge Flag )     

////	CR2 = 1;                // set bit-rate for I2C to 200kH   ( = clock Rate Bits )
////	CR1 = 1;								// set bit-rate for I2C to 200kH   ( = clock Rate Bits )
////	CR0 = 0;								// set bit-rate for I2C to 200kH   ( = clock Rate Bits )

////	ENS1 = 1;		// enable SIO1      ( activate the SIO1 Logic )              

////	STA = 0;		// no START flag            
////	STO = 0;    // no STOP bit

////	ES1 = 1;    // IEN0*# Interrupt enable 0 A8H EA EAD ES1 ES0 ET1 EX1 ET0 EX0 00H
////	PS1 = 1;    // IP0*# Interrupt priority 0 B8H – PAD PS1 PS0 PT1 PX1 PT0 PX0 x0000000B
	
	
	S1ADR = 0xFF;  // Adressregister  ,8 - Bit Special Funktion Register kann mit einer 7 Bit langen Slave Adresse geladen werden
	// ADDRESS R EGISTER (S1ADR)
  // This 8-bit register may be loaded with the 7-bit slave address to which the controller will respond when programmed as
  // a slave receiver/transmitter

	P16 = 1;		// set I2Cports // SCL (P1.6): Serial port clock line I2 C-bus.
	P17 = 1;                    // SDA (P1.7): Serial port data line I2 C-bus.

	SI = 0;		// reset Serial Interrupt Flag 
							/*	
							This flag is set and an interrupt request is generated, after any of the
							following events occur:
							• A START condition is generated in master mode.
							• The own slave address has been received during AA = 1.
							• The general call address has been received while GC (bit S1ADR.0) and AA = 1.
							• A data byte has been received or transmitted in master mode (even if arbitration is lost).
							• A data byte has been received or transmitted as selected slave.
							• A STOP or START condition is received as selected slave receiver or transmitter.
							While the SI flag is set, SCL remains LOW and the serial transfer is suspended. SI must be
							reset by software.
							*/



	
	AA = 0;		// disable masterinterrupt    ( = Assert Acknowledge Flag ) 

							/*
							When this bit is set, an acknowledge is returned after any one of the
							following conditions:
							• Own slave address is received.
							• General call address is received; GC (bit S1ADR.0) = 1.
							• A data byte is received, while the device is programmed to be a master receiver.
							• A data byte is received. while the device is a selected slave receiver.
							When the bit is reset, no acknowledge is returned. Consequently, no interrupt is requested when
							the own address or general call address is received.
							*/	

	CR2 = 1;                // set bit-rate for I2C to 200kH   ( = clock Rate Bits )
	CR1 = 1;								// set bit-rate for I2C to 200kH   ( = clock Rate Bits )
	CR0 = 0;								// set bit-rate for I2C to 200kH   ( = clock Rate Bits )
/*	

CR2 	CR1 	CR0 			BIT RATE (kbits/s) at fclk
										  12 MHz 		16 MHz
1 			0			0				50 				66.7
1				0			1				3.75 			5
1				1			0				75				100
1				1			1				100				 -
0				0			0				200  			266.7
0				0			1				7.5  			10
0				1			0				300  			400(1)
0				1			1				400  			 -
*/

	ENS1 = 1;		// enable SIO1      ( activate the SIO1 Logic  , ENS1 = 1: serial I/O enabled)              

	STA = 0;		// no START flag    //When this bit is set in slave mode, the hardware checks the I2C-bus and generates
																	//a START condition if the bus is free or after the bus becomes free. If the device operates in
																	//master mode it will generate a repeated START condition 

																	
	STO = 0;    // no STOP bit  //If this bit is set in a master mode a STOP condition is generated. A STOP condition
                              //detected on the I2C-bus clears this bit. This bit may also be set in slave mode in order to recover
                              //from an error condition. In this case no STOP condition is generated to the I2 C-bus, but the hard
                              //ware releases the SDA and SCL lines and switches to the not selected receiver mode. The
                              //STOP flag is cleared by the hardware

	ES1 = 1;    // Enable SIO1 (I2 C-bus) interrupt.
	PS1 = 1;    // SIO1 (I2C-bus) interrupt priority level
}


void SetAddress(unsigned char I2CAddress, unsigned int StartAddress)
{
	

	
	unsigned char Address = I2CAddress | ((StartAddress>>7)&0x0E);
	
	
		
	//	I2CBufferInPtrTop = 0;
	//I2CBufferInPtrBottom = 0;
	I2CBufferOutPtrTop = 0;
	I2CBufferOutPtrBottom = 0;
	I2CSendReady=1;
	//I2CMasterReciever = 0;
	I2CError = 0;
	CardInserted = 0;

	I2CAddressBufferOut[I2CBufferOutPtrTop] = Address;
	I2CDataBufferOut[I2CBufferOutPtrTop++] = StartAddress&0xFF;  // Hier wordt gezorgd dat we te maken hebben met 1 byte
	
	EA = 0;  // EA=IE0....7 (Interrupt Enable) Register  EA 	IE.7 	It disables all interrupts. When EA = 0 no interrupt will be acknowledged and EA = 1 enables the interrupt individually.
						//- 	IE.6 	Reserved for future use.
						//- 	IE.5 	Reserved for future use.
						//ES 	IE.4 	Enables/disables serial port interrupt.
						//ET1 	IE.3 	Enables/disables timer1 overflow interrupt.
						//EX1 	IE.2 	Enables/disables external interrupt1.
						//ET0 	IE.1 	Enables/disables timer0 overflow interrupt.
						//EX0 	IE.0 	Enables/disables external interrupt0.
	
  if ((I2CBufferOutPtrTop != I2CBufferOutPtrBottom) && I2CSendReady)
	{
		I2CSendReady=0;
		EA = 1;
	  STA = 0;		// no START flag    //When this bit is set in slave mode, the hardware checks the I2C-bus and generates
																	//a START condition if the bus is free or after the bus becomes free. If the device operates in
																	//master mode it will generate a repeated START condition 
	}
	EA = 1;
	
	while (!I2CSendReady);   // Wacht lus 
	Delay(1);
  
	
	
	// LCD info ----------------------------------------------------------
	if (I2CError)
	{
		SetLCD(0,"\xC0");
		if (I2CAddress==I2CAddressSmartCard)
		{
			CardInserted = 0;
			//SerialBufferOut[SerialBufferOutPtrTop++] = CARD_INSERTED;
			//SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
			//SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
		}
		else
			SetLCD(1,"No EEPROM comm. ");
	//--------------------------------------------------------------------	
	
		I2CError = 0;		
	}
}


/***************************************************************
* READ_DATA_I2C                                                    *
***************************************************************/
void ReadData(unsigned char I2CAddress, unsigned int StartAddress, unsigned char *Buffer, unsigned char NrOfBytes)
{
	unsigned char cnt;
	unsigned char Address = I2CAddress | ((StartAddress>>7)&0x0E) |0x01;
	
	
		
		I2CBufferInPtrTop = 0;
	I2CBufferInPtrBottom = 0;
	I2CBufferOutPtrTop = 0;
	//I2CBufferOutPtrBottom = 0;
	I2CSendReady=1;
	I2CMasterReciever = 0;
	//I2CError = 0;
	//CardInserted = 0;

	SetAddress(I2CAddress, StartAddress);

	I2CMasterReciever = 1;
	I2CDataLength = NrOfBytes;
	I2CAddressBufferOut[I2CBufferOutPtrTop++] = Address;
	EA = 0;

  if (I2CSendReady)
	{
		I2CSendReady=0;
		EA = 1;
		STA = 1;  // ????
	}
	EA = 1;

	while(!I2CSendReady);
	I2CMasterReciever = 0;

	cnt = 0;
	while ((I2CBufferInPtrTop != I2CBufferInPtrBottom))
	{
		unsigned char Dummy = I2CBufferIn[I2CBufferInPtrBottom++];
		if (cnt<NrOfBytes)
			Buffer[cnt++] = Dummy;
	}
}


/***************************************************************
* WRITE_DATA_I2C                                                    *
***************************************************************/
void WriteData(unsigned char I2CAddress, unsigned int StartAddress, unsigned char *Buffer, unsigned char NrOfBytes)
{
	unsigned char cntByte = 0;
	unsigned char Address = I2CAddress | ((StartAddress>>7)&0x0E);    // Device adres en StartAdres samen gevoegd ( groter adres bereik )

	while (cntByte<NrOfBytes)
	{
		// Watchdog
		PCON |= 0x10;
		T3 = 0;

		I2CAddressBufferOut[I2CBufferOutPtrTop] = Address;
		I2CDataBufferOut[I2CBufferOutPtrTop++] = StartAddress&0xFF;   // Pak alleen de rechter byte
	 	I2CAddressBufferOut[I2CBufferOutPtrTop] = Address;
	    I2CDataBufferOut[I2CBufferOutPtrTop++] = Buffer[cntByte];
		StartAddress++;
		cntByte++;

		while ((cntByte<NrOfBytes) && (StartAddress&0x000F))
		{
	  	I2CAddressBufferOut[I2CBufferOutPtrTop] = Address;
		  I2CDataBufferOut[I2CBufferOutPtrTop++] = Buffer[cntByte];
			StartAddress++;
			cntByte++;
		}

		EA = 0;                      // 8051 Global Interrupt
		
	    if ((I2CBufferOutPtrTop != I2CBufferOutPtrBottom) && I2CSendReady)  // Wat gebeurd hier ? ( oke als ze ongelijk zijn )
		{
			I2CSendReady=0;
			EA = 1;                  // 8051 Global Interrupt
			STA = 1;                 // Generate Start or Repeated start
		}
		EA = 1;                      // 8051 Global Interrupt

		while (!I2CSendReady);       // Wacht op hardware = Ready

		Address = I2CAddress | ((StartAddress>>7)&0x0E);

		Delay(1);
	}
}



/************************************************************************
*       Interrupt on recieving I2C                                                                                                                      *
************************************************************************/
void Int_I2C(void) interrupt 5 using 3 
{
	unsigned char Status = S1STA;   // S1STAT ( dem SIO1 Status Register )
	unsigned char Data = S1DAT;   // S1DAT ( dem SIO1 Daten Register )
	
	
		
		I2CBufferInPtrTop = 0;
	//I2CBufferInPtrBottom = 0;
	I2CBufferOutPtrTop = 0;
	I2CBufferOutPtrBottom = 0;
	I2CSendReady=1;
	I2CMasterReciever = 0;
	I2CError = 0;
	//CardInserted = 0;

	I2CStatus = Status;

	switch (Status)
	{
		case 0x08:
		{	/*	A START Condition has been transmitted */
			if (I2CBufferOutPtrTop != I2CBufferOutPtrBottom)
			{
				S1DAT = CurrentAddress = I2CAddressBufferOut[I2CBufferOutPtrBottom];
				if (I2CMasterReciever)
					I2CBufferOutPtrBottom++;
			}
			STO = 0;

			if (I2CRepeatAddress != CurrentAddress)
			{
				I2CRepeat = 0;
				I2CRepeatAddress = CurrentAddress;
			}
		}
		break;
		case 0x10:
		{	/*	A repeated START Condition has been transmitted */
			STO = 0;
			S1DAT = CurrentAddress = I2CAddressBufferOut[I2CBufferOutPtrBottom];
			if (I2CRepeatAddress != CurrentAddress)
			{
				I2CRepeat = 0;
				I2CRepeatAddress = CurrentAddress;
			}
		}
		break;
		case 0x18:
		{	/*	SLA+W has been transmitted; ACK has been recieved */
			if (I2CBufferOutPtrTop != I2CBufferOutPtrBottom)
			{
				STA = 0;
				STO = 0;
				S1DAT = I2CDataBufferOut[I2CBufferOutPtrBottom];
			}
			else
			{
				STA = 0;
				STO = 1;
				AA = 1;
				I2CSendReady = 1;
			}
			I2CRepeat = 0;
		}
		break;
		case 0x20:
		{	/*	SLA+W has been transmitted; NOT ACK has been recieved */
			I2CRepeat++;
			if (I2CRepeat > 10)
			{
				STA = 0;
				STO = 1;
				AA = 1;
 				I2CSendReady = 1;
				I2CError = 1;
				while ((I2CBufferOutPtrBottom!=I2CBufferOutPtrTop) && (I2CRepeatAddress == I2CAddressBufferOut[I2CBufferOutPtrBottom]))
				{
					I2CBufferOutPtrBottom++;
				}
			}
			else
			{
				STA = 1;
				STO = 0;
				AA = 1;
			}
		}
		break;
		case 0x28:
		{	/*	Data byte in S1DAT has been transmitted; ACK has been recieved */
			I2CBufferOutPtrBottom++;
			if ((I2CBufferOutPtrTop != I2CBufferOutPtrBottom) && (I2CAddressBufferOut[I2CBufferOutPtrBottom] == CurrentAddress))
			{
				STA = 0;
				STO = 0;
				S1DAT = I2CDataBufferOut[I2CBufferOutPtrBottom];
			}
			else
			{
				STA = 0;
				STO = 1;
				AA = 1;
				I2CSendReady = 1;
			}
			I2CRepeat = 0;
		}
		break;
		case 0x30:
		{	/*	Data byte in S1DAT has been transmitted; NOT ACK has been recieved */
			STA = 1;
			STO = 0;
			I2CRepeat++;
			if (I2CRepeat > 10)
			{
				while ((I2CBufferOutPtrBottom!=I2CBufferOutPtrTop) && (I2CRepeatAddress == I2CAddressBufferOut[I2CBufferOutPtrBottom]))
				{
					I2CBufferOutPtrBottom++;
				}
			}
		}
		break;
		case 0x38:
		{	/* Arbitration lost in SLA+R/W or databytes */
			STA = 1;
			STO = 0;
			I2CRepeat++;
			if (I2CRepeat > 10)
			{
				while ((I2CBufferOutPtrBottom!=I2CBufferOutPtrTop) && (I2CRepeatAddress == I2CAddressBufferOut[I2CBufferOutPtrBottom]))
				{
					I2CBufferOutPtrBottom++;
				}
  		}
		}
		break;
		case 0x40:
		{	/* SLA+R has been transmitted; ACK has been recieved */
			STA = 0;
			STO = 0;

			I2CByteNr = 0;
			if (I2CDataLength<2)
				AA = 0;
			else
				AA = 1;
		}
		break;
		case 0x48:
		{	/* SLA+R has been transmitted; NOT ACK has been recieved */
			STA = 0;
			STO = 1;
			AA = 1;
			I2CSendReady = 1;
		}
		break;
		case 0x50:
		{	/* Data byte has been recieved; ACK has been returned */
			I2CBuffer[I2CByteNr++] = Data;

			STA = 0;
			STO = 0;
			if (I2CByteNr>(I2CDataLength-1))
				AA = 0;
			else
				AA = 1;
		}
		break;
		case 0x58:
		{	/* Data byte has been recieved; NOT ACK has been returned */
			unsigned char cnt = 0;

			while (cnt < I2CByteNr)
			{
				I2CBufferIn[I2CBufferInPtrTop++] = I2CBuffer[cnt];
				cnt++;
			}
			I2CBufferIn[I2CBufferInPtrTop++] = Data;

			I2CByteNr = 0;
			STA = 0;
			STO = 1;
			I2CSendReady = 1;
		}
		break;
		case 0x60:
		{	/* Own SLA+W has been revieved; ACK has been returned */
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
			I2CByteNr = 0;
		}
		break;
		case 0x68:
		{	/* Arbitration lost in SLA+R/W as master; Own SLA+W has been recieved, ACK returned */
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
			I2CByteNr = 0;
		}
		break;
		case 0x70:
		{	/* General call address (00H) has been recieved; ACK has been returned */
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
			I2CByteNr = 0;
		}
		break;
		case 0x78:
		{	/*	Arbitration lost in SLA+R/W as master; Generall call has been recieved; ACK has been returned */
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
			I2CByteNr = 0;
		}
		break;
		case 0x80:
		{	/* Previously addressed with own SLV adress; Data has been received; ACK has been returned */
			switch (I2CByteNr)
			{
				case 0:
				{
					I2CFromAddress = Data;
					I2CByteNr++;
				}
				break;
				case 1:
				{
					I2CItem = Data;
					switch (I2CItem)
					{
						default:
						{
							I2CLength = 3;
						}
					}
					I2CByteNr++;
				}
				break;
				case 2:
				{
					I2CModule = Data;
					I2CByteNr++;
				}
				break;
				default:
				{
					I2CBuffer[I2CByteNr++] = Data;
					if (I2CByteNr > I2CLength)
					{
						unsigned char cnt = 3;
						I2CBufferIn[I2CBufferInPtrTop++] = I2CFromAddress;
						I2CBufferIn[I2CBufferInPtrTop++] = I2CItem;
						I2CBufferIn[I2CBufferInPtrTop++] = I2CModule;
						while (cnt < I2CByteNr)
						{
							I2CBufferIn[I2CBufferInPtrTop++] = I2CBuffer[cnt];
							cnt++;
						}
						I2CByteNr = 0;
					}
				}
				break;
			}
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
		}
		break;
		case 0x88:
		{	/* Previously addressed with own SLA; Data has been received; NOT ACK has been returned */
			STA = 0;
			STO = 0;
			AA = 1;
			I2CSendReady = 1;

		}
		break;
		case 0x90:
		{	/*	Previously addressed with General Call;Data byte has been recieved; ACK has been recieved */
			switch (I2CByteNr)
			{
				case 0:
				{
					I2CFromAddress = Data;
					I2CByteNr++;
				}
				break;
				case 1:
				{
					I2CItem = Data;
					switch (I2CItem)
					{
						default:
						{
							I2CLength = 3;
						}
					}
					I2CByteNr++;
				}
				break;
				case 2:
				{
					I2CModule = Data;
					I2CByteNr++;
				}
				break;
				default:
				{
					I2CBuffer[I2CByteNr++] = Data;
					if (I2CByteNr > I2CLength) 
					{
						unsigned char cnt = 3;
						I2CBufferIn[I2CBufferInPtrTop++] = I2CFromAddress;
						I2CBufferIn[I2CBufferInPtrTop++] = I2CItem;
						I2CBufferIn[I2CBufferInPtrTop++] = I2CModule;
						while (cnt < I2CByteNr)
						{
							I2CBufferIn[I2CBufferInPtrTop++] = I2CBuffer[cnt];
							cnt++;
						}
					}
					I2CByteNr = 0;
				}
				break;
			}
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
		}
		break;
		case 0x98:
		{	/* Previously addressed with General Call;Data byte has been recieved; NOT ACK has been returned */
			STA = 0;
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
		}
		break;
		case 0xA0:
		{	/*	A STOP condition or repeated START condition has been recieved while still addressed as SLV/REC or SLV/TRX */
			STA = 0;
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
		}
		break;
		case 0xA8:
		{	/*	Own SLA+R has been recieved; ACK has been returned */
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
		}
		break;
		case 0xB0:
		{	/*	Arbitration lost in SLA+R/W as master; own SLA+R has been recieved; ACK has been returned */
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
		}
		break;
		case 0xB8:
		{	/*	Data byte in S1DAT has been transmitted; ACK has been recieved */
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
		}
		break;
		case 0xC0:
		{	/*	Data byte in S1DAT has been transmitted; NOT ACK has been recieved */
			STA = 0;
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
		}
		break;
		case 0xC8:
		{	/* Last data byte in S1DAT has been transmitted (AA=0); ACK has been recieved */
			STA = 0;
			STO = 0;
			AA = 1;
			I2CSendReady = 1;
		}
		break;
		case 0xF8:
		{	/* No relevant state information available; S1 = 0 */
		}
		break;
		case 0x00:
		{	/* Bus error */
			STA = 0;
			STO = 1;
			I2CSendReady = 1;
		}
		break;
	}
	SI = 0;
}
