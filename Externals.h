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
#include "KZG_Functions.h"
#include "RandomTimer.h"
#include "encrypt.hpp"


extern const char *Node;
extern const char *Bedienung;

extern RandomTimer my_random_timer;



extern uint8_t key[16];
extern Encrypt encrypt;
extern uint8_t Actual_Random[2][16];

extern volatile uint8_t do_sleep;


extern uint8_t KeyList[KEY_NUM][KEY_LENGTH];
extern uint8_t InfoList[KEY_NUM][INFO_LENGTH];

extern Communication cmulti;
extern Communication kmulti;

extern ComReceiver cnetCom;
extern ComReceiver knetCom;

#endif /* EXTERNALS_H_ */
