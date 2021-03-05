/*
 * CNET_Functions.h
 *
 * Created: 06.05.2016 20:04:20
 *  Author: Christof
 */


#ifndef CNET_FUNCTIONS_H_
#define CNET_FUNCTIONS_H_

#include "Communication.h"
#include "ComReceiver.h"
#include "cmultiStandardCommands.h"

extern COMMAND cnetCommands[];
extern INFORMATION cnetInformation[];

#define NUM_CNET_COMMANDS 9+CMULTI_STANDARD_NUM
#define NUM_INFORMATION 1


void jobSetLichtGrenzwerte(ComReceiver *comRec, char function,char address,char job, void * pMem);
void mailPressed(ComReceiver *comRec, char function,char address,char job, void * pMem);

void jobGetCTemperatureSensor(ComReceiver *output, char function,char address,char job, void * pMem);
void jobGetCHumiditySensor(ComReceiver *output, char function,char address,char job, void * pMem);
void jobGetCAbsHumiditySensor(ComReceiver *output, char function,char address,char job, void * pMem);
void jobGetCDewPointSensor(ComReceiver *output, char function,char address,char job, void * pMem);
void jobGetCompilationDate(ComReceiver *output, char function,char address,char job, void * pMem);
void jobGetCompilationTime(ComReceiver *output, char function,char address,char job, void * pMem);
void jobSetSecurityKey(ComReceiver *output, char function,char address,char job, void * pMem);
void jobGetSecurityKey(ComReceiver *output, char function,char address,char job, void * pMem);
void jobGetFreeMemory(ComReceiver *output, char function,char address,char job, void * pMem);
void jobGetMeasureRate(ComReceiver *output, char function,char address,char job, void * pMem);
void jobSetMeasureRate(ComReceiver *output, char function,char address,char job, void * pMem);
void jobGetAverageRate(ComReceiver *output, char function,char address,char job, void * pMem);
void jobSetAverageRate(ComReceiver *output, char function,char address,char job, void * pMem);
void jobGotExternalTemperature(ComReceiver *output, char function,char address,char job, void * pMem);

void startBootloader(ComReceiver *comRec, char function,char address,char job, void * pMem);
void setBootloaderAttention(ComReceiver *comRec, char function,char address,char job, void * pMem);

void gotNewBrightness();



#endif /* CNET_FUNCTIONS_H_ */
