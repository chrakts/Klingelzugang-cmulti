/*
 * KZG_Functions.cpp
 *
 * Created: 20.02.2016 18:53:31
 *  Author: Christof
 */

#include "Klingelzugang.h"
#include "Klingel.h"
#include "ComReceiver.h"
#include "Externals.h"
#include "../Secrets/secrets.h"

COMMAND knetCommands[NUM_KZG_COMMANDS] =
{
  cmultiStandardCommands,
  {'T','p',CUSTOMER,NOPARAMETER,0,numberPressed},
  {'P','p',CUSTOMER,NOPARAMETER,0,specialPressed},
  {'L','t',CUSTOMER,NOPARAMETER,0,lightToggle},
  {'M','p',CUSTOMER,NOPARAMETER,0,mailPressed},
  {'D','p',CUSTOMER,NOPARAMETER,0,doorToggle},
  {'K','r',CUSTOMER,NOPARAMETER,0,jobKlingel},
  {'P','t',CUSTOMER,NOPARAMETER,0,jobPirTrigger},
  {'C','f',CUSTOMER,NOPARAMETER,0,jobfoundCard},         // JOB_GET_INFO
  {'C','c',CUSTOMER,NOPARAMETER,0,jobWriteReady},         // JOB_GET_INFO
  {'C','i',CUSTOMER,BYTEARRAY,INFO_LENGTH,jobGotCardInfo},         // JOB_GET_INFO
  {'C','s',CUSTOMER,NOPARAMETER,0,jobGetKeyForClear},         // JOB_GET_INFO
};

INFORMATION knetInformation[NUM_KZG_INFORMATION]=
{
  {"ZB",'C','1','t',FLOAT,1,(void*)&fTemperaturZB,NULL}
};

ComReceiver knetCom(&kmulti,Node, knetCommands,NUM_KZG_COMMANDS, knetInformation,NUM_KZG_INFORMATION,NULL,NULL);


void jobWriteReady(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  inputStatus = NO_INPUT;
}

void jobGetKeyForClear(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t tosend[KEY_LENGTH],i;

  inputStatus = DELETE_CARD_WAITING;
  uint8_t keynum = (uint8_t) address - '0';
	if (keynum<KEY_NUM)
	{
		for (i=0;i<KEY_LENGTH;i++)
		{
			tosend[i] = eeprom_read_byte(&KeyList[keynum][i]);
		}
    comRec->Getoutput()->sendStandardByteArray(tosend,KEY_LENGTH,Bedienung,'C',address,'d','T');
  }
}

void jobGotCardInfo(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t i;
  uint8_t keynum = (uint8_t) address - '0';
  uint8_t *info = (uint8_t*) pMem;
  bool valid = true;
  if(keynum<INFO_NUM)
  {
    for (i=0;i<INFO_LENGTH;i++)
    {
      if( info[i] != eeprom_read_byte(&InfoList[keynum][i]) )
      {
        valid = false;
        break;
      }
    }
  }
  else
    valid = false;
  open_door(valid);
  comRec->Getoutput()->sendCommand(Bedienung,'C',address,'r'); // release Card
}


void jobfoundCard(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t tosend[KEY_LENGTH],i;
  uint8_t keynum = (uint8_t) address - '0';
	if (keynum<KEY_NUM)
	{
		for (i=0;i<KEY_LENGTH;i++)
		{
			tosend[i] = eeprom_read_byte(&KeyList[keynum][i]);
		}
    comRec->Getoutput()->sendStandardByteArray(tosend,KEY_LENGTH,Bedienung,'C',address,'k','T');
  }

}


void jobPirTrigger(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  wakeup();
  calcAndSendLichtActualStatus();
  sendSignalLamps(false);
}

void jobGetRandom(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  uint8_t adr = (uint8_t) address - 48;
  if(adr<2)
  {
    LEDROT_OFF;
    my_random_timer.Make_New();
    my_random_timer.GetRandom(Actual_Random[adr]);
    WDT_Reset();
    comRec->Getoutput()->setEncryption();
    comRec->Getoutput()->sendStandardByteArray(Actual_Random[adr],16,Bedienung,'R',address,'s','T');
    WDT_Reset();
    //encrypt.Output_Encrypt(&serKNET,'B','R','S',Actual_Random[address]); // Zielangaben sind noch unklar
  }
}

void jobNewRandom(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  my_random_timer.Make_New();
}
