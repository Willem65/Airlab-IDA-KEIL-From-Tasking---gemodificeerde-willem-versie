
#include "reg552.h"
#include "ConsoleTimer.h"

unsigned char GlobalTimerActive;
unsigned char Count;

struct TimeAndDateStruct OldTimeAndDate, TimeAndDate;


void SetupTimer0()
{
	 // Timer 0; 16 bits counter
	 TMOD	= (TMOD & 0xF0) | 0x01;
	 
	 TH0  = 0xDB;	/* set prescaler timer0     		*/
	 TL0  = 0xFF;	/* 11059200/12 tick per second	*/
								/* = 0x2400;	count to 100;			*/
								/* 0xFFFF-0x2400 = DBFF					*/
	 PT0  = 0;		/* low priority             */
	 ET0  = 1;		/* enable int. timer0       */
	 TR0  = 1;
}



void Timer0(void) interrupt 1 using 1 
{
	 TH0  = 0xDB;	/* set prescaler timer0     		*/
	 TL0  = 0xFF;	/* 11059200/12 tick per second	*/
								/* = 0x2400;	count to 100;			*/
								/* 0xFFFF-0x2400 = DBFF					*/

	 Count++;
	 if (Count>9) //Was before 10.
		 Count = 0;

	 if ((Count==0x00) && (GlobalTimerActive))
	 {
			TimeAndDate.Frames+=10;
			if (TimeAndDate.Frames>90)
			{
				TimeAndDate.Frames = 0;
				TimeAndDate.Seconds++;
				if (TimeAndDate.Seconds>59)
				{
					TimeAndDate.Seconds = 0;
					TimeAndDate.Minutes++;
					if (TimeAndDate.Minutes>59)
					{
						TimeAndDate.Minutes = 0;
						TimeAndDate.Hours++;
						if (TimeAndDate.Hours>23)
						{
							TimeAndDate.Hours = 0;
						}
					}
				}
		 }
	}
}