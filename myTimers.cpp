/*
 * MyTimer.cpp
 *
 * Created: 11.02.2016 20:19:03
 *  Author: Christof
 */

#include "Klingelzugang.h"


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
}

void Stop_Ring_Bel(uint8_t test)
{
	LEDROT_OFF;
	LEDGRUEN_OFF;
	KLINGEL_STOP;
}

void goto_sleep(uint8_t test)
{
	do_sleep = 1;
}

void BlockingEnde(uint8_t test)
{
  BlockingStatus = UNBLOCKED;
  MyTimers[TIMER_BLOCKING_END].state = TM_STOP;
}
