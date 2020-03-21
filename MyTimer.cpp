/*
 * MyTimer.cpp 
 *
 * Created: 11.02.2016 20:19:03
 *  Author: Christof
 */ 

#include "Klingelzugang.h"

void init_mytimer(void)
{
	CLK.RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;
	
	do {
		;/* Wait until RTC is not busy. */
	} while ( RTC.STATUS & RTC_SYNCBUSY_bm );
	
	RTC.PER = 1;
	RTC.CNT = 0;
	RTC.COMP = 0; 
	RTC.CTRL = 1; // Teiler 1
	RTC.INTCTRL	= RTC_OVFINTLVL_LO_gc;
}

SIGNAL(RTC_OVF_vect)
{
uint8_t i;
//	LED_KLINGEL_TOGGLE;
	for (i=0;i<MYTIMER_NUM;i++)
	{
		switch(MyTimers[i].state )
		{
			case TM_STOP:
			break;
			case TM_RUN:
				MyTimers[i].actual--;
				if (MyTimers[i].actual==0)
				{
					if (MyTimers[i].OnReady!=NULL)
						MyTimers[i].OnReady(3);
					
					if (MyTimers[i].restart == RESTART_YES)
						MyTimers[i].state = TM_START;
					else
						MyTimers[i].state = TM_STOP;
				}
			break;			
			case TM_START:
				MyTimers[i].actual = MyTimers[i].value;
				MyTimers[i].state = TM_RUN;
			break;

			case TM_RESET:
				MyTimers[i].state = TM_START;
			break;
		}
	}
}

void Beeper_Ready(uint8_t test)
{
	;
}

void LED_toggle(uint8_t test)
{
	LED_GELB_TOGGLE;
}

void LED_rot_toggle(uint8_t test)
{
	LED_ROT_TOGGLE;
}

void Stop_Open_Door(uint8_t test)
{
	LED_GRUEN_OFF;
	SCHLIESSEN_1;
	SCHLIESSEN_2;
}

void Stop_Ring_Bel(uint8_t test)
{
	LED_ROT_OFF;
	KLINGEL_STOP;
}

void goto_sleep(uint8_t test)
{
	do_sleep = 1;
}
