//#include "reg552.h"

///************************************************************************
//*       Interrupt on recieving I2C                                                                                                                      *
//************************************************************************/
//void Int_I2C(void) interrupt 5 using 3 
//{
//	unsigned char Status = S1STA;
//	unsigned char Data = S1DAT;
//	
//	
//		
//	I2CBufferInPtrTop = 0;
//	I2CBufferOutPtrTop = 0;
//	I2CBufferOutPtrBottom = 0;
//	I2CSendReady=1;
//	I2CMasterReciever = 0;
//	I2CError = 0;


//	I2CStatus = Status;

//	switch (Status)
//	{
//		case 0x08:
//		{	/*	A START Condition has been transmitted */
//			if (I2CBufferOutPtrTop != I2CBufferOutPtrBottom)
//			{
//				S1DAT = CurrentAddress = I2CAddressBufferOut[I2CBufferOutPtrBottom];
//				if (I2CMasterReciever)
//					I2CBufferOutPtrBottom++;
//			}
//			STO = 0;

//			if (I2CRepeatAddress != CurrentAddress)
//			{
//				I2CRepeat = 0;
//				I2CRepeatAddress = CurrentAddress;
//			}
//		}
//		break;
//		case 0x10:
//		{	/*	A repeated START Condition has been transmitted */
//			STO = 0;
//			S1DAT = CurrentAddress = I2CAddressBufferOut[I2CBufferOutPtrBottom];
//			if (I2CRepeatAddress != CurrentAddress)
//			{
//				I2CRepeat = 0;
//				I2CRepeatAddress = CurrentAddress;
//			}
//		}
//		break;
//		case 0x18:
//		{	/*	SLA+W has been transmitted; ACK has been recieved */
//			if (I2CBufferOutPtrTop != I2CBufferOutPtrBottom)
//			{
//				STA = 0;
//				STO = 0;
//				S1DAT = I2CDataBufferOut[I2CBufferOutPtrBottom];
//			}
//			else
//			{
//				STA = 0;
//				STO = 1;
//				AA = 1;
//				I2CSendReady = 1;
//			}
//			I2CRepeat = 0;
//		}
//		break;
//		case 0x20:
//		{	/*	SLA+W has been transmitted; NOT ACK has been recieved */
//			I2CRepeat++;
//			if (I2CRepeat > 10)
//			{
//				STA = 0;
//				STO = 1;
//				AA = 1;
// 				I2CSendReady = 1;
//				I2CError = 1;
//				while ((I2CBufferOutPtrBottom!=I2CBufferOutPtrTop) && (I2CRepeatAddress == I2CAddressBufferOut[I2CBufferOutPtrBottom]))
//				{
//					I2CBufferOutPtrBottom++;
//				}
//			}
//			else
//			{
//				STA = 1;
//				STO = 0;
//				AA = 1;
//			}
//		}
//		break;
//		case 0x28:
//		{	/*	Data byte in S1DAT has been transmitted; ACK has been recieved */
//			I2CBufferOutPtrBottom++;
//			if ((I2CBufferOutPtrTop != I2CBufferOutPtrBottom) && (I2CAddressBufferOut[I2CBufferOutPtrBottom] == CurrentAddress))
//			{
//				STA = 0;
//				STO = 0;
//				S1DAT = I2CDataBufferOut[I2CBufferOutPtrBottom];
//			}
//			else
//			{
//				STA = 0;
//				STO = 1;
//				AA = 1;
//				I2CSendReady = 1;
//			}
//			I2CRepeat = 0;
//		}
//		break;
//		case 0x30:
//		{	/*	Data byte in S1DAT has been transmitted; NOT ACK has been recieved */
//			STA = 1;
//			STO = 0;
//			I2CRepeat++;
//			if (I2CRepeat > 10)
//			{
//				while ((I2CBufferOutPtrBottom!=I2CBufferOutPtrTop) && (I2CRepeatAddress == I2CAddressBufferOut[I2CBufferOutPtrBottom]))
//				{
//					I2CBufferOutPtrBottom++;
//				}
//			}
//		}
//		break;
//		case 0x38:
//		{	/* Arbitration lost in SLA+R/W or databytes */
//			STA = 1;
//			STO = 0;
//			I2CRepeat++;
//			if (I2CRepeat > 10)
//			{
//				while ((I2CBufferOutPtrBottom!=I2CBufferOutPtrTop) && (I2CRepeatAddress == I2CAddressBufferOut[I2CBufferOutPtrBottom]))
//				{
//					I2CBufferOutPtrBottom++;
//				}
//  		}
//		}
//		break;
//		case 0x40:
//		{	/* SLA+R has been transmitted; ACK has been recieved */
//			STA = 0;
//			STO = 0;

//			I2CByteNr = 0;
//			if (I2CDataLength<2)
//				AA = 0;
//			else
//				AA = 1;
//		}
//		break;
//		case 0x48:
//		{	/* SLA+R has been transmitted; NOT ACK has been recieved */
//			STA = 0;
//			STO = 1;
//			AA = 1;
//			I2CSendReady = 1;
//		}
//		break;
//		case 0x50:
//		{	/* Data byte has been recieved; ACK has been returned */
//			I2CBuffer[I2CByteNr++] = Data;

//			STA = 0;
//			STO = 0;
//			if (I2CByteNr>(I2CDataLength-1))
//				AA = 0;
//			else
//				AA = 1;
//		}
//		break;
//		case 0x58:
//		{	/* Data byte has been recieved; NOT ACK has been returned */
//			unsigned char cnt = 0;

//			while (cnt < I2CByteNr)
//			{
//				I2CBufferIn[I2CBufferInPtrTop++] = I2CBuffer[cnt];
//				cnt++;
//			}
//			I2CBufferIn[I2CBufferInPtrTop++] = Data;

//			I2CByteNr = 0;
//			STA = 0;
//			STO = 1;
//			I2CSendReady = 1;
//		}
//		break;
//		case 0x60:
//		{	/* Own SLA+W has been revieved; ACK has been returned */
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//			I2CByteNr = 0;
//		}
//		break;
//		case 0x68:
//		{	/* Arbitration lost in SLA+R/W as master; Own SLA+W has been recieved, ACK returned */
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//			I2CByteNr = 0;
//		}
//		break;
//		case 0x70:
//		{	/* General call address (00H) has been recieved; ACK has been returned */
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//			I2CByteNr = 0;
//		}
//		break;
//		case 0x78:
//		{	/*	Arbitration lost in SLA+R/W as master; Generall call has been recieved; ACK has been returned */
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//			I2CByteNr = 0;
//		}
//		break;
//		case 0x80:
//		{	/* Previously addressed with own SLV adress; Data has been received; ACK has been returned */
//			switch (I2CByteNr)
//			{
//				case 0:
//				{
//					I2CFromAddress = Data;
//					I2CByteNr++;
//				}
//				break;
//				case 1:
//				{
//					I2CItem = Data;
//					switch (I2CItem)
//					{
//						default:
//						{
//							I2CLength = 3;
//						}
//					}
//					I2CByteNr++;
//				}
//				break;
//				case 2:
//				{
//					I2CModule = Data;
//					I2CByteNr++;
//				}
//				break;
//				default:
//				{
//					I2CBuffer[I2CByteNr++] = Data;
//					if (I2CByteNr > I2CLength)
//					{
//						unsigned char cnt = 3;
//						I2CBufferIn[I2CBufferInPtrTop++] = I2CFromAddress;
//						I2CBufferIn[I2CBufferInPtrTop++] = I2CItem;
//						I2CBufferIn[I2CBufferInPtrTop++] = I2CModule;
//						while (cnt < I2CByteNr)
//						{
//							I2CBufferIn[I2CBufferInPtrTop++] = I2CBuffer[cnt];
//							cnt++;
//						}
//						I2CByteNr = 0;
//					}
//				}
//				break;
//			}
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//		}
//		break;
//		case 0x88:
//		{	/* Previously addressed with own SLA; Data has been received; NOT ACK has been returned */
//			STA = 0;
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;

//		}
//		break;
//		case 0x90:
//		{	/*	Previously addressed with General Call;Data byte has been recieved; ACK has been recieved */
//			switch (I2CByteNr)
//			{
//				case 0:
//				{
//					I2CFromAddress = Data;
//					I2CByteNr++;
//				}
//				break;
//				case 1:
//				{
//					I2CItem = Data;
//					switch (I2CItem)
//					{
//						default:
//						{
//							I2CLength = 3;
//						}
//					}
//					I2CByteNr++;
//				}
//				break;
//				case 2:
//				{
//					I2CModule = Data;
//					I2CByteNr++;
//				}
//				break;
//				default:
//				{
//					I2CBuffer[I2CByteNr++] = Data;
//					if (I2CByteNr > I2CLength) 
//					{
//						unsigned char cnt = 3;
//						I2CBufferIn[I2CBufferInPtrTop++] = I2CFromAddress;
//						I2CBufferIn[I2CBufferInPtrTop++] = I2CItem;
//						I2CBufferIn[I2CBufferInPtrTop++] = I2CModule;
//						while (cnt < I2CByteNr)
//						{
//							I2CBufferIn[I2CBufferInPtrTop++] = I2CBuffer[cnt];
//							cnt++;
//						}
//					}
//					I2CByteNr = 0;
//				}
//				break;
//			}
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//		}
//		break;
//		case 0x98:
//		{	/* Previously addressed with General Call;Data byte has been recieved; NOT ACK has been returned */
//			STA = 0;
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//		}
//		break;
//		case 0xA0:
//		{	/*	A STOP condition or repeated START condition has been recieved while still addressed as SLV/REC or SLV/TRX */
//			STA = 0;
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//		}
//		break;
//		case 0xA8:
//		{	/*	Own SLA+R has been recieved; ACK has been returned */
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//		}
//		break;
//		case 0xB0:
//		{	/*	Arbitration lost in SLA+R/W as master; own SLA+R has been recieved; ACK has been returned */
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//		}
//		break;
//		case 0xB8:
//		{	/*	Data byte in S1DAT has been transmitted; ACK has been recieved */
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//		}
//		break;
//		case 0xC0:
//		{	/*	Data byte in S1DAT has been transmitted; NOT ACK has been recieved */
//			STA = 0;
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//		}
//		break;
//		case 0xC8:
//		{	/* Last data byte in S1DAT has been transmitted (AA=0); ACK has been recieved */
//			STA = 0;
//			STO = 0;
//			AA = 1;
//			I2CSendReady = 1;
//		}
//		break;
//		case 0xF8:
//		{	/* No relevant state information available; S1 = 0 */
//		}
//		break;
//		case 0x00:
//		{	/* Bus error */
//			STA = 0;
//			STO = 1;
//			I2CSendReady = 1;
//		}
//		break;
//	}
//	SI = 0;
//}