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
extern const char *Bedienung;

volatile extern TIMER MyTimers[MYTIMER_NUM];

extern RandomTimer my_random_timer;

extern volatile float fHelligkeit;
extern volatile uint16_t iLichtgrenzwert,iLichtwertHysterese;
extern volatile uint8_t iLichtKleinStatus;

extern volatile uint8_t BlockingStatus;
extern uint16_t BlockadeZeiten[];

extern uint8_t key[16];
extern Encrypt encrypt;
extern uint8_t Actual_Random[2][16];

extern volatile uint8_t do_sleep;
extern volatile uint8_t auto_door_status;

extern char CodeList[INFO_NUM][5] ;
extern uint8_t KeyList[KEY_NUM][KEY_LENGTH];
extern uint8_t InfoList[KEY_NUM][INFO_LENGTH];

extern Communication cmulti;
extern Communication kmulti;

extern ComReceiver cnetCom;
extern ComReceiver knetCom;

#endif /* EXTERNALS_H_ */
