/*
 * MyTimer.cpp
 *
 * Created: 11.02.2016 20:19:03
 *  Author: Christof
 */

#include "Klingelzugang.h"

volatile TIMER MyTimers[MYTIMER_NUM]=	{	{TM_STOP,RESTART_NO,100,0,Beeper_Ready},
											{TM_START,RESTART_YES,100,0,nextReport},
											{TM_STOP,RESTART_NO,125,0,LED_toggle},
											{TM_START,RESTART_NO,1200,0,goto_sleep},
											{TM_STOP,RESTART_NO,250,0,Stop_Open_Door},
											{TM_STOP,RESTART_NO,600,0,Stop_Ring_Bel},
											{TM_STOP,RESTART_YES,20,0,LED_rot_toggle},
                      {TM_STOP,RESTART_NO,250,0,BlockingEnde},	// Blockade nach falscher Eingabe
                      {TM_STOP,RESTART_NO,5000,0,nowSaveEEProm}
};

void nextReport(uint8_t test)
{
  doNextReport=true;
}

void Beeper_Ready(uint8_t test)
{
	;
}

void LED_toggle(uint8_t test)
{
	LEDGELB_TOGGLE;
}

void LED_rot_toggle(uint8_t test)
{
	LEDROT_TOGGLE;
}

void Stop_Open_Door(uint8_t test)
{
	LEDGRUEN_OFF;
	SCHLIESSEN_1;
	SCHLIESSEN_2;
	inputStatus = NO_INPUT;
}

void Stop_Ring_Bel(uint8_t test)
{
	LEDROT_OFF;
	LEDGRUEN_OFF;
	KLINGEL_STOP;
}

void goto_sleep(uint8_t test)
{
  if( (inputStatus>INPUT_BLOCKED) || (inputStatus<INPUT_BLOCKED1) )
  {
    do_sleep = 1;
    inputStatus = INPUT_SLEEP;
    MyTimers[TIMER_SLEEP].state = TM_STOP;
  }
  else
    MyTimers[TIMER_SLEEP].state = TM_START;
}

void BlockingEnde(uint8_t test)
{
  //BlockingStatus = UNBLOCKED;
  if(inputStatus==INPUT_BLOCKED1)
  {
    inputStatus = NO_INPUT;
    MyTimers[TIMER_BLOCKING_END].state = TM_STOP;
  }
  else
  {
    inputStatus--;
    MyTimers[TIMER_BLOCKING_END].state = TM_START;
  }
}

