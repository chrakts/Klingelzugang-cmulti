/*
 * Global_Vars.cpp
 *
 * Created: 21.02.2016 08:32:15
 *  Author: Christof
 */

#define EXTERNALS_H_

#include "Klingelzugang.h"

const char *Node = "Kg";
const char *Bedienung = "ZB";

volatile TIMER MyTimers[MYTIMER_NUM]=	{	{TM_STOP,RESTART_NO,100,0,Beeper_Ready},
											{TM_STOP,RESTART_NO,100,0,NULL},
											{TM_STOP,RESTART_NO,125,0,LED_toggle},
											{TM_START,RESTART_NO,1200,0,goto_sleep},
											{TM_STOP,RESTART_NO,250,0,Stop_Open_Door},
											{TM_STOP,RESTART_NO,600,0,Stop_Ring_Bel},
											{TM_STOP,RESTART_YES,20,0,LED_rot_toggle},
                      {TM_STOP,RESTART_NO,250,0,BlockingEnde}	// Blockade nach falscher Eingabe

};

RandomTimer my_random_timer(&TCC1);

Encrypt encrypt((uint8_t*)key);

volatile uint8_t BlockingStatus = UNBLOCKED;
uint16_t BlockadeZeiten[BLOCKED_LAST] = {250,1000,1000,3000,5000,15000,60000};

volatile uint8_t do_sleep = 0;

volatile uint8_t auto_door_status = false;

Communication cmulti(CNET,Node,5,USE_BUSY_0);
ComReceiver cmultiRec(&cmulti,Node,cnetCommands,NUM_CNET_COMMANDS,cnetInformation,NUM_INFORMATION,NULL,NULL);

Communication kmulti(KNET,Node,5,USE_BUSY_1);


