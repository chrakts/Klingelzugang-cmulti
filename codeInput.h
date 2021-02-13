#ifndef CODEINPUT_H_INCLUDED
#define CODEINPUT_H_INCLUDED

#include "Klingelzugang.h"

void numberPressed(ComReceiver *comRec, char function,char address,char job, void * pMem);
void specialPressed(ComReceiver *comRec, char function,char address,char job, void * pMem);

#endif // CODEINPUT_H_INCLUDED
