/*
 * Klingel.cpp
 *
 * Created: 09.02.2016 20:24:18
 *  Author: Christof
 */ 

#include "Klingelzugang.h"

void init_klingel()
{
	PORTD_OUTCLR = TASTER_SINK;
	PORTD_INTCTRL = PORT_INT0LVL0_bm ; // Low-Level interrupt 0 for PORTA
	PORTD_INT0MASK = TASTER_PFORTE;
	PORTD_PIN2CTRL = PORT_ISC_FALLING_gc | PORT_OPC_PULLUP_gc | PORT_SRLEN_bm;
}

SIGNAL(PORTD_INT0_vect)
{
	_delay_ms(100);
	if( (PORTD_IN & TASTER_PFORTE)==0 )
	{
		ring_bel(1);
	}
}