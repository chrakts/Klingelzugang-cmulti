/*
 * Klingelzugang.cpp
 *
 * Created: 15.11.2015 17:39:41
 *  Author: Christof
 */

#include "Klingelzugang.h"
#include "report.h"

void init_io()
{
  init_clock(SYSCLK,PLL,true,CLOCK_CALIBRATION);

#ifdef MAIN_BUILT
	PORTA_DIRSET = 0xff;
	PORTB_DIRSET = 0xff;
	PORTC_DIRSET = RS485_0_TE_PIN | RS485_0_RE_PIN | RS485_0_TxD_PIN | STREET_WANT_MASTER;
	PORTC_DIRCLR = RS485_0_RxD_PIN | STREET_LET_MASTER | STREET_OUT0 | STREET_OUT_RING;
	PORTD_DIRSET =  DAC_PIN10_PIN | RS485_1_RE_PIN | RS485_1_TE_PIN | RS485_1_TxD_PIN | TASTER_SINK;
	PORTD_DIRCLR =  TASTER_PFORTE | HOUSE_OUT0 | RS485_1_RxD_PIN;
	PORTE_DIRSET = 0xff;
	PORTE_OUTCLR = KLINGEL0_PIN | KLINGEL1_PIN;
//	PORTE_DIRCLR = ;
	PORTA_OUTSET = OEFFNER_PIN_L | LEDROT_PIN | LEDGRUEN_PIN | LEDGELB_PIN;
	PORTC_OUTSET = RS485_0_RE_PIN;
#endif // MAIN_BUILT

#ifdef PLUG_BUILT

	PORTA_DIRSET = PIN2_bm | PIN3_bm | PIN4_bm;
	PORTA_OUTSET = 0xff;

	PORTB_DIRSET = 0xff;

	PORTC_DIRSET = PIN0_bm | PIN3_bm | PIN4_bm | PIN5_bm;

	PORTD_DIRSET = PIN0_bm | PIN1_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN7_bm;
	PORTD_DIRCLR = PIN2_bm | PIN6_bm;
	PORTD_OUTCLR = PIN4_bm | PIN5_bm;

	PORTE_DIRSET = 0xff;
#endif // PLUG_BUILT


	uint8_t i;
	for(i=0;i<20;i++)
	{
		LEDGRUEN_TOGGLE;
		_delay_ms(50);
	}
  LEDGRUEN_OFF;
	cmulti.open(Serial::BAUD_57600,F_CPU);
	kmulti.open(Serial::BAUD_57600,F_CPU);
	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
	sei();
}


int main(void)
{
	init_io();
  cmulti.broadcastUInt8((uint8_t) RST.STATUS,'S','0','R');
  readEEData();
	init_mytimer();
	init_klingel();


	WDT_Reset();
  kmulti.sendInfo("Hallo an Bedienung","PP");
  cmulti.broadcastUInt8((uint8_t) RST.STATUS,'S','0','R');
  cmulti.broadcastString("Hallo an CNET",'H','h','h');
  MyTimers[TIMER_NEXT_REPORT].state = TM_START;
	WDT_EnableAndSetTimeout(WDT_SHORT);
	do
	{
    knetCom.comStateMachine();
    knetCom.doJob();
		cnetCom.comStateMachine();
    cnetCom.doJob();
    sendSignalLamps(false);
    doReport();
    writeEEData();
		WDT_Reset();

	} while (1);
}

void doReport()
{
  if(doNextReport)
  {
    toReport++;
    switch(toReport)
    {
      case REPORT_LICHTKLEINACTUAL:
        broadcastLichtActualStatus('1');
      break;
      case REPORT_LICHTKLEINSET:
        broadcastLichtSetStatus('1');
      break;
      case REPORT_LICHTGROSSACTUAL:
        broadcastLichtActualStatus('2');
      break;
      case REPORT_LICHTGROSSSET:
        broadcastLichtSetStatus('2');
      break;
      case REPORT_LICHTGRENZWERT:
        cmulti.broadcastUInt16(iLichtgrenzwert,'L','g','G');
      break;
      case REPORT_LICHTHYSTERESE:
        cmulti.broadcastUInt16(iLichtwertHysterese,'L','h','G');
      break;
      case REPORT_TEMPERATURZB:
        cmulti.broadcastFloat(fTemperaturZB,'C','1','t');
      break;
      case REPORT_DOOROPENSTATUS:
        broadcastOpenDoorStatus();
      break;
      case REPORT_STATUSLEDS:
        sendSignalLamps(true);
      break;
      case REPORT_LAST:
        toReport = REPORT_FIRST;
      break;
    }
    doNextReport = false;
  }
}

void wakeup()
{
  if(inputStatus == INPUT_SLEEP)
  {
    inputStatus = NO_INPUT;
  }
  MyTimers[TIMER_SLEEP].state = TM_START;
}


