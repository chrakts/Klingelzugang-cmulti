#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED

#include "Klingelzugang.h"

extern volatile uint8_t auto_door_status;

void open_door(uint8_t open);
void doorToggle(ComReceiver *comRec, char function,char address,char job, void * pMem);
void broadcastOpenDoorStatus();
#endif // DOOR_H_INCLUDED
