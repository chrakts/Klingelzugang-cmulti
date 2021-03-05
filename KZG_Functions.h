/*
 * KZG_Functions.h
 *
 * Created: 20.02.2016 19:22:42
 *  Author: Christof
 */


#ifndef KZG_FUNCTIONS_H_
#define KZG_FUNCTIONS_H_

#include "Communication.h"
#include "ComReceiver.h"
#include "cmultiStandardCommands.h"

#define NUM_KZG_COMMANDS 9+CMULTI_STANDARD_NUM
#define NUM_KZG_INFORMATION 1

extern COMMAND knetCommands[NUM_KZG_COMMANDS];
extern INFORMATION knetInformation[NUM_KZG_INFORMATION];

void jobPirTrigger(ComReceiver *comRec, char function,char address,char job, void * pMem);

void jobfoundCard(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGotCardInfo(ComReceiver *comRec, char function,char address,char job, void * pMem);

/*
void jobGetRandom(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobNewRandom(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobClearAutoDoor(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobDoAutoDoor(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetCardKey(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobGetCardInfo(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobTryInfo(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobTryCode(ComReceiver *comRec, char function,char address,char job, void * pMem);
*/

#endif /* KZG_FUNCTIONS_H_ */
