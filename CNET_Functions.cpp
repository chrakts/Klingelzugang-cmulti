/*
 * CNET_Functions.cpp
 *
 * Created: 06.05.2016 20:03:58
 *  Author: Christof
 */
#include "CNET_Functions.h"
#include "Klingelzugang.h"
#include "Klingel.h"
#include "ComReceiver.h"
#include "cmultiStandardCommands.h"
#include "Externals.h"
#include "../Secrets/secrets.h"

COMMAND cnetCommands[NUM_CNET_COMMANDS] =
{
  cmultiStandardCommands,
  {'L','G',CUSTOMER,UINT_16,1,jobSetLichtGrenzwerte},
  {'L','S',CUSTOMER,STRING,8,setLichtSet},
  {'X','s',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_RESET			'R'
  {'T','p',CUSTOMER,NOPARAMETER,0,numberPressed},
  {'P','p',CUSTOMER,NOPARAMETER,0,specialPressed},
  {'L','t',CUSTOMER,NOPARAMETER,0,lightToggle},
  {'M','p',CUSTOMER,NOPARAMETER,0,mailPressed},
  {'D','p',CUSTOMER,NOPARAMETER,0,doorToggle},
  {'K','r',CUSTOMER,NOPARAMETER,0,jobKlingel},
  {'P','t',CUSTOMER,NOPARAMETER,0,jobPir},
};


#define NUM_INFORMATION 1
INFORMATION cnetInformation[NUM_INFORMATION]=
{
  {"CQ",'C','1','l',FLOAT,1,(void*)&fHelligkeit,gotNewBrightness}
};

ComReceiver cnetCom(&cmulti,Node, cnetCommands,NUM_CNET_COMMANDS, cnetInformation,NUM_INFORMATION,NULL,NULL);

void jobPir(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  wakeup();

}

void mailPressed(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  wakeup();
}

void gotNewBrightness()
{
  calcAndSendLichtActualStatus();
}

void jobSetLichtGrenzwerte(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint16_t wert;
  wert = ((uint16_t *)pMem)[0];
  switch(address)
  {
    case 'g':
      iLichtgrenzwert = wert;
    break;
    case 'h':
      iLichtwertHysterese = wert;
    break;
  }
  cmulti.broadcastUInt16(wert,function,address,job);
}


