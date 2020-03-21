/*
 * Klingelzugang.cpp
 *
 * Created: 15.11.2015 17:39:41
 *  Author: Christof
 */

#include "Klingelzugang.h"

int main(void)
{
	init_clock(SYSCLK,PLL,true,CLOCK_CALIBRATION);
	init_io();
	//uart_init_0();
	//uart_init_1();  // Schnittstelle zum Bedienpanel

	init_mytimer();
	init_klingel();
  initReadMonitor();
  initBusyCounter();

	WDT_EnableAndSetTimeout(WDT_SHORT);

	LED_ROT_ON;

	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
	sei();

	cmulti.open(Serial::BAUD_57600,F_CPU);
  serKNET.open(Serial::BAUD_19200,F_CPU);

	WDT_Reset();
	_delay_ms(500);
	cmulti.sendInfo("Hallo von der Klingel","BR");
	do
	{
		rec_KNET();
		cnetCom.comStateMachine();
    cnetCom.doJob();
		if(be_master>0)
			be_master_fkt();
		WDT_Reset();
	} while (1);
}

void be_master_fkt()
{
uint8_t temp = false;

	WDT_Reset();
	MyTimers[TIMER_TIMEOUT].value = 1000;
	MyTimers[TIMER_TIMEOUT].state = TM_START;
	do
	{
		WDT_Reset();
		if ( (PORTC_IN & STREET_LET_MASTER) )
		{
			_delay_ms(10);						// soll Stoerungen abfangen
			if (PORTC_IN & STREET_LET_MASTER)
				temp = true;
		}
	}
	while(  (temp==false) && (MyTimers[TIMER_TIMEOUT].state!=TM_STOP)  );
	WDT_EnableAndSetTimeout(WDT_LONG);
	if (temp==true)
	{
		LED_ROT_OFF;
		LED_GELB_OFF;
		//send_answer(CNET,"be master",true);
		cmulti.sendInfo("be master","BR");
		do
		{
			cnetCom.comStateMachine();
			cnetCom.doJob();
			if (be_tunnel>0)
			{
				be_tunnel_fkt();
			}
		} while (be_master>0);
	}
	else
	{
		//send_answer(CNET,"no master",false);
		cmulti.sendInfo("no master","BR");
		LED_GRUEN_OFF;
	}
	//send_answer(CNET,"no more master",true);
	cmulti.sendInfo("no more master","BR");
	be_master = 0;
	WDT_EnableAndSetTimeout(WDT_SHORT);
}

void be_tunnel_fkt()
{
char z1;
char magic_word[]="ich habe fertig";
#define MAGIC_LENGTH 15
uint8_t zeiger = 0;
uint8_t ready = false;
uint8_t temp;

	//send_answer(CNET,"be tunnel",true);
	cmulti.sendInfo("be tunnel","BR");
	temp = MyTimers[TIMER_RECALL].value;
	MyTimers[TIMER_RECALL].value = 40;
	be_tunnel = 0;
	cmulti.input_flush();
	serKNET.input_flush();
	//input_flush( CNET );
	//input_flush( KNET );
	do
	{
		if (cmulti.getChar(z1))
		{
			WDT_Reset();
			serKNET.transmit(z1);
			//output(KNET,z1);
			if (z1==magic_word[zeiger])
			{
				zeiger++;
				if(zeiger == MAGIC_LENGTH)
					ready = true;
			}
			else
				zeiger = 0;
		}
		if (serKNET.getChar(z1))
		{
			cmulti.transmit(z1);
		}
	} while (!ready);
	MyTimers[TIMER_RECALL].value = temp;
}


void init_io()
{
	PORTA_DIRSET = 0xff;
//	PORTA_DIRCLR = ;
	PORTB_DIRSET = 0xff;
//	PORTB_DIRCLR = ;
	PORTC_DIRSET = RS485_0_TE_PIN | RS485_0_RE_PIN | RS485_0_TxD_PIN | STREET_WANT_MASTER;
	PORTC_DIRCLR = RS485_0_RxD_PIN | STREET_LET_MASTER | STREET_OUT0 | STREET_OUT_RING;
	PORTD_DIRSET =  DAC_PIN10_PIN | RS485_1_TERE_PIN | RS485_1_TxD_PIN | TASTER_SINK;
	PORTD_DIRCLR =  TASTER_PFORTE | HOUSE_OUT0 | HOUSE_OUT_RING | RS485_1_RxD_PIN;
	PORTE_DIRSET = 0xff;
	PORTE_OUTCLR = KLINGEL1_PIN | KLINGEL2_PIN;
//	PORTE_DIRCLR = ;

	PORTA_OUTSET = OEFFNER_PIN_L | LED_ROT_PIN | LED_GRUEN_PIN | LED_GELB_PIN;
	PORTC_OUTSET = RS485_0_RE_PIN;
}

