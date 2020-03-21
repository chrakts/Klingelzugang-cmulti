/*
 * Global_Vars.cpp
 *
 * Created: 21.02.2016 08:32:15
 *  Author: Christof
 */

#define EXTERNALS_H_

#include "Klingelzugang.h"

const char *Node = "Kg";

char Compilation_Date[] = __DATE__;
char Compilation_Time[] = __TIME__;


volatile TIMER MyTimers[MYTIMER_NUM]=	{	{TM_STOP,RESTART_NO,100,0,Beeper_Ready},
											{TM_STOP,RESTART_NO,100,0,NULL},
											{TM_STOP,RESTART_NO,125,0,LED_toggle},
											{TM_START,RESTART_NO,1200,0,goto_sleep},
											{TM_STOP,RESTART_NO,250,0,Stop_Open_Door},
											{TM_STOP,RESTART_NO,600,0,Stop_Ring_Bel},
											{TM_STOP,RESTART_YES,20,0,LED_rot_toggle}
};

RandomTimer my_random_timer(&TCC1);

const char *fehler_text[]={"Speicherfehler","Parameterfehler","keine SMS","kein Handy","Antwort falsch",
"Job Fehler","keine Uebertragung"};


Encrypt encrypt((uint8_t*)key);


volatile uint8_t do_sleep = 0;

volatile uint8_t auto_door_status = false;


uint8_t be_master = 0;
uint8_t be_tunnel = 0;

char Code[5]="----";
/* Global variables for TWI */
TWI_Master_t twiC_Master;    /*!< TWI master module. */
TWI_Master_t twiE_Master;    /*!< TWI master module. */

/*! Buffer with test data to send for TWI*/
uint8_t sendBuffer[NUM_BYTES] = {0x55, 0xAA, 0xF0, 0x0F, 0xB0, 0x0B, 0xDE, 0xAD};

Communication cmulti(CNET,Node,5);
Serial serKNET(KNET);
