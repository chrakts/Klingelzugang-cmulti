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

RandomTimer my_random_timer(&TCC1);

Encrypt encrypt((uint8_t*)key);

volatile uint8_t do_sleep = 0;

enum{REPORT_FIRST,REPORT_LICHTKLEINACTUAL,REPORT_LICHTKLEINSET,REPORT_LICHTGROSSACTUAL,REPORT_LICHTGROSSSET,REPORT_LICHTGRENZWERT,REPORT_LICHTHYSTERESE,REPORT_DOOROPENSTATUS,REPORT_LAST};

volatile bool doNextReport=false;
volatile uint8_t toReport=REPORT_FIRST;

Communication kmulti(KNET,Node,5,USE_BUSY_1);
ComReceiver kmultiRec(&kmulti,Node,knetCommands,NUM_KZG_COMMANDS,knetInformation,NUM_KZG_INFORMATION,NULL,NULL);

Communication cmulti(CNET,Node,5,USE_BUSY_0);
ComReceiver cmultiRec(&cmulti,Node,cnetCommands,NUM_CNET_COMMANDS,cnetInformation,NUM_INFORMATION,NULL,NULL);



