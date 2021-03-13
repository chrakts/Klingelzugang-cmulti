/*
 * Klingel.cpp
 *
 * Created: 09.02.2016 20:24:18
 *  Author: Christof
 */

#include "Klingelzugang.h"

void init_klingel()
{
	TASTERLEDSETUP;
	TASTERINPUTSETUP;
	TASTERLED_ON;
	TASTER_PORT.INTCTRL  |= PORT_INT1LVL0_bm ; // Low-Level interrupt 0 for PORTA
	TASTER_PORT.INT1MASK |= TASTER_INPUT;
	TASTER_PORT.TASTER_INTPIN = PORT_ISC_FALLING_gc | PORT_OPC_PULLUP_gc | PORT_SRLEN_bm;
}

void jobKlingel(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  wakeup();
  auto_door_status = false;
  ring_bel(address);
}

void ring_bel(char klingel)
{
	if (klingel == '1')
  {
		KLINGEL0_START;
    LEDROT_ON;
  }
	else
  {
		KLINGEL1_START;
		LEDGELB_ON;
  }
  //my_random_timer.Make_New();
	MyTimers[TIMER_STOP_RING_BEL].value = 700;
	MyTimers[TIMER_STOP_RING_BEL].state = TM_START;
	cmulti.broadcast('K',klingel,'r');
}

void entprelltTaster(uint8_t test)
{
  if( (TASTER_PORT.IN & TASTER_INPUT)==0 )
	{
		ring_bel('1');
	}
}

SIGNAL(TASTER_INT_VEC)
{
	MyTimers[TIMER_ENTPRELLT_TASTER].state = TM_START;
}
