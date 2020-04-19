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

	init_mytimer();
	init_klingel();
	//WDT_EnableAndSetTimeout(WDT_SHORT);

	LED_ROT_ON;

	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
	sei();

	cmulti.open(Serial::BAUD_57600,F_CPU);
	kmulti.open(Serial::BAUD_57600,F_CPU);
  //serKNET.open(Serial::BAUD_19200,F_CPU);
  cmulti.encryptSetKey(key);
  kmulti.encryptSetKey(key);

	WDT_Reset();
	//_delay_ms(500);
	uint8_t wert[1];
	wert[0] = (RST.STATUS);
	kmulti.sendByteArray(wert,1,BROADCAST,'S','S','0','R');
	kmulti.sendInfo("Klingel zur Bedienung Neu","BR");
  broadcastOpenDoorStatus();
	do
	{
		//rec_KNET();
		knetCom.comStateMachine();
    knetCom.doJob();
		cnetCom.comStateMachine();
    cnetCom.doJob();
//		if(be_master>0)
//			be_master_fkt();
		WDT_Reset();
	} while (1);
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
	PORTE_OUTCLR = KLINGEL0_PIN | KLINGEL1_PIN;
//	PORTE_DIRCLR = ;

	PORTA_OUTSET = OEFFNER_PIN_L | LED_ROT_PIN | LED_GRUEN_PIN | LED_GELB_PIN;
	PORTC_OUTSET = RS485_0_RE_PIN;
}

void broadcastOpenDoorStatus()
{
  kmulti.broadcastUInt8(auto_door_status,'D','0','S');
#pragma message(Reminder "cmulti noch aktivieren")
  //cmulti.broadcastUInt8(auto_door_status,'D','0','S');
}
