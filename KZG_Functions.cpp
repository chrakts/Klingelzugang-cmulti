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
/*
  {'R','g',CUSTOMER,NOPARAMETER,0,jobGetRandom},      // JOB_GET_RANDOM
  {'R','n',CUSTOMER,NOPARAMETER,0,jobNewRandom},      // JOB_NEW_RANDOM
  {'C','K',CUSTOMER,BYTEARRAY,16,jobGetCardKey},          // JOB_GET_KEY
  {'C','I',CUSTOMER,BYTEARRAY,16,jobGetCardInfo},         // JOB_GET_INFO
  {'D','t',CUSTOMER,BYTEARRAY,16,jobTryInfo},             // JOB_TRY_INFO
  {'C','t',CUSTOMER,BYTEARRAY,4,jobTryCode},             // JOB_TRY_CODE
*/
  {'C','f',CUSTOMER,NOPARAMETER,0,jobfoundCard},         // JOB_GET_INFO
  {'C','i',CUSTOMER,BYTEARRAY,INFO_LENGTH,jobGotCardInfo},         // JOB_GET_INFO
};

INFORMATION knetInformation[NUM_KZG_INFORMATION]=
{
  {"ZB",'C','1','t',FLOAT,1,(void*)&fTemperaturZB,NULL}
};

ComReceiver knetCom(&kmulti,Node, knetCommands,NUM_KZG_COMMANDS, knetInformation,NUM_KZG_INFORMATION,NULL,NULL);


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

/*
// Sendet den Key der Karte mit der Nummer keynum. Die ersten Stellen enthalten den Key, der Rest ActualRandom
void jobGetCardKey(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t tosend[KEY_LENGTH];
uint8_t i,keynum;

  uint8_t adr = (uint8_t) address - 48;
  keynum = ( (uint8_t*)pMem )[0];
	if (keynum<KEY_NUM)
	{
		for (i=0;i<KEY_LENGTH;i++)
		{
			tosend[i] = eeprom_read_byte(&KeyList[keynum][i]);
		}
    comRec->Getoutput()->setEncryption(Actual_Random[adr]);
    comRec->Getoutput()->sendStandardByteArray(tosend,KEY_LENGTH,Bedienung,'C',address,'k','T');
  }
}

// Sendet den Info der Karte mit der Nummer keynum. Die ersten Stellen enthalten den Info, der Rest ActualRandom
void jobGetCardInfo(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	uint8_t tosend[INFO_LENGTH];
	uint8_t i,infonum;

  uint8_t adr = (uint8_t) address - 48;
  infonum = ( (uint8_t*)pMem )[0];
  if (infonum<INFO_NUM)
	{
		for (i=0;i<INFO_LENGTH;i++)
		{
			tosend[i] =  eeprom_read_byte(&InfoList[infonum][i]);
		}
    comRec->Getoutput()->setEncryption(Actual_Random[adr]);
    comRec->Getoutput()->sendStandardByteArray(tosend,INFO_LENGTH,Bedienung,'C',address,'i','T');
	}
}

void jobTryInfo(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t i;

  my_random_timer.Make_New();
  auto_door_status = false;
  uint8_t adr = (uint8_t) address - 48;
  for(i=0;i<16;i++)
    ((uint8_t*)pMem)[i] = ((uint8_t*)pMem)[i] ^ Actual_Random[adr][i];
  open_door(check_info((uint8_t*)pMem) );
}

void jobTryCode(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t i,j,right;

  my_random_timer.Make_New();
  auto_door_status = false;
  uint8_t adr = (uint8_t) address - 48;
  for(i=0;i<16;i++)
    ((uint8_t*)pMem)[i] = ((uint8_t*)pMem)[i] ^ Actual_Random[adr][i];

  right = true;
  j=0;
  do
  {
    i=0;
    do
    {
      if (  (( (uint8_t*)pMem)[i] )!=eeprom_read_byte((uint8_t*)&CodeList[j][i])  )
        right = false;
      else
        right = true;
      i++;
    }while( (i<4) && (right) );
    j++;
  }while( (j<INFO_NUM) && (right==false) );
  open_door(right);
}
*/
