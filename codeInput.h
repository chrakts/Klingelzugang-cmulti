#ifndef CODEINPUT_H_INCLUDED
#define CODEINPUT_H_INCLUDED

#include "Klingelzugang.h"

extern volatile uint8_t inputStatus;
extern volatile uint8_t BlockingStatus;
extern uint16_t BlockadeZeiten[];

enum{NO_INPUT,INPUT_1,INPUT_2,INPUT_3,INPUT_READY,START_SPECIAL,SPECIAL_1,SPECIAL_2,SPECIAL_3,SPECIAL_4,SPECIAL_5,SPECIAL_READY,INPUT_BLOCKED,DOOR_OPENING,INPUT_SLEEP};
enum{UNBLOCKED=0,BLOCKED0,BLOCKED1,BLOCKED2,BLOCKED3,BLOCKED4,BLOCKED5,BLOCKED_LAST};

void numberPressed(ComReceiver *comRec, char function,char address,char job, void * pMem);
void specialPressed(ComReceiver *comRec, char function,char address,char job, void * pMem);
uint8_t checkCodeInput(char *toTest);
uint8_t checkSpecialCodeInput(char *toTest);
void make_blocking(uint8_t reset);

#endif // CODEINPUT_H_INCLUDED
