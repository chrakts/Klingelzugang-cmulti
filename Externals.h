/*
 * Externals.h
 *
 * Created: 21.02.2016 08:14:49
 *  Author: Christof
 */


#ifndef EXTERNALS_H_
#define EXTERNALS_H_

#include "myConstants.h"
#include "Communication.h"
#include "Klingelzugang.h"
#include "ComReceiver.h"
#include "CNET_Functions.h"


extern const char *Node;

extern char Compilation_Date[];
extern char Compilation_Time[];

volatile extern TIMER MyTimers[MYTIMER_NUM];

extern RandomTimer my_random_timer;
extern const char *fehler_text[];

extern uint8_t key[16];
extern Encrypt encrypt;
extern uint8_t Actual_Random[16];

extern volatile uint8_t do_sleep;
extern volatile uint8_t auto_door_status;

extern uint8_t be_master;
extern uint8_t be_tunnel;

extern char CodeList[INFO_NUM][5] ;
extern uint8_t KeyList[KEY_NUM][KEY_LENGTH];
extern uint8_t InfoList[KEY_NUM][INFO_LENGTH];

extern Communication cmulti;
extern Serial serKNET;
extern ComReceiver cnetCom;

#endif /* EXTERNALS_H_ */
