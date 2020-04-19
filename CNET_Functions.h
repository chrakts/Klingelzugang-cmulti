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

/*
#define NODE			'Z'

#define FUNCTION_SYSTEM		'S'
#define JOB_BL_ATTENTION	'A'
#define JOB_BL_START		'B'
#define JOB_RESET_ACT		'r'
#define JOB_RESET			'R'
#define JOB_DC_ATTENTION	'D'			// Direkten Channel zum rueckwaerigen Geraet
#define JOB_DC_START		'C'
#define JOB_BE_MASTER		'M'
#define JOB_RELEASE_MASTER	'm'
#define JOB_BE_TUNNEL		'T'
#define JOB_RELEASE_TUNNEL	't'

#define FUNCTION_CODE		'C'
#define JOB_READ_CODE		'r'
#define JOB_WRITE_CODE		'w'
#define JOB_DELETE_CODE		'd'
#define JOB_DELETE_CODE_ALL	'D'
#define JOB_CODE_GET_NUM	'n'

#define FUNCTION_CARD		'D'
#define JOB_CARD_GET_NUM	'n'
#define JOB_READ_KEY		'R'
#define JOB_READ_INFO		'r'
#define JOB_WRITE_KEY_INFO_RND	'W'



#define FUNCTION_BOOT	'B'
#define DO_BOOTLOAD 	's'

#define JOB_ERROR		'!'
*/


void setStatusLichtKlein(ComReceiver *comRec, char function,char address,char job, void * pMem);
void jobSetLichtGrenzwert(ComReceiver *comRec, char function,char address,char job, void * pMem);
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
