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

//const char *fehler_text[]={"Speicherfehler","Parameterfehler","keine SMS","kein Handy","Antwort falsch",
//"Job Fehler","keine Uebertragung"};


/*
uint8_t rec_state_KNET=RCST_WAIT;
uint8_t function_KNET=0;
uint8_t job_KNET=0;

char *parameter_text_KNET=NULL;
uint8_t parameter_text_length_KNET;
uint8_t parameter_text_pointer_KNET;*/

/*uint8_t bootloader_attention;		// nur wenn true, dann darf Bootloader gestartet werden.
uint8_t reset_attention;			// nur wenn true, dann darf Reset ausgeloest werden.

void (*bootloader)( void ) = (void (*)(void)) (BOOT_SECTION_START/2);       // Set up function pointer
void (*reset)( void ) = (void (*)(void)) 0x0000;       // Set up function pointer*/


#define NUM_KZG_COMMANDS 20

COMMAND knetCommands[NUM_KZG_COMMANDS] =
{
  {'S','K',CUSTOMER,STRING,16,jobSetSecurityKey},
  {'S','k',CUSTOMER,NOPARAMETER,0,jobGetSecurityKey},
  {'S','C',DEVELOPMENT,NOPARAMETER,0,jobGetCompilationDate},
  {'S','T',DEVELOPMENT,NOPARAMETER,0,jobGetCompilationTime},
  {'S','m',PRODUCTION,NOPARAMETER,0,jobGetFreeMemory},
  {'R','g',CUSTOMER,NOPARAMETER,0,jobGetRandom},      // JOB_GET_RANDOM
  {'R','n',CUSTOMER,NOPARAMETER,0,jobNewRandom},      // JOB_NEW_RANDOM
  {'D','c',CUSTOMER,NOPARAMETER,0,jobClearAutoDoor},  // JOB_AUTO_DOOR_OFF
  {'D','A',CUSTOMER,NOPARAMETER,0,jobDoAutoDoor},     // JOB_AUTO_DOOR
  {'K','r',CUSTOMER,NOPARAMETER,0,jobKlingel},        // JOB_KLINGEL
  {'C','K',CUSTOMER,BYTEARRAY,16,jobGetCardKey},          // JOB_GET_KEY
  {'C','I',CUSTOMER,BYTEARRAY,16,jobGetCardInfo},         // JOB_GET_INFO
  {'D','t',CUSTOMER,BYTEARRAY,16,jobTryInfo},             // JOB_TRY_INFO
  {'C','t',CUSTOMER,BYTEARRAY,4,jobTryCode},             // JOB_TRY_CODE
  {'P','t',CUSTOMER,NOPARAMETER,0,jobPirTrigger},         //
  {'L','N',CUSTOMER,NOPARAMETER,0,jobNextLichtStatus},    // JOB_RESET			'R'
  {'M','A',PRODUCTION,STRING,16,setBootloaderAttention},    //JOB_BL_ATTENTION
  {'M','B',PRODUCTION,NOPARAMETER,0,startBootloader},    // JOB_BL_START		'B'
  {'M','T',CUSTOMER,NOPARAMETER,0,NULL},
  {'M','t',CUSTOMER,NOPARAMETER,0,NULL},
};

#define NUM_KZG_INFORMATION 1

INFORMATION knetInformation[NUM_KZG_INFORMATION]=
{
  {"CQ",'C','1','l',FLOAT,1,(void*)&fHelligkeit,NULL}
};

ComReceiver knetCom(&kmulti,Node, knetCommands,NUM_KZG_COMMANDS, knetInformation,NUM_KZG_INFORMATION,NULL,NULL);


void jobNextLichtStatus(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  my_random_timer.Make_New();
/*
  if(iLichtKleinStatus<2)
    iLichtKleinStatus++;
  else
    iLichtKleinStatus=0;
  kmulti.broadcastUInt8(iLichtKleinStatus,'L','1','s');
  cmulti.broadcastUInt8(iLichtKleinStatus,'L','1','s');*/
}

void jobPirTrigger(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  my_random_timer.Make_New();
//  cmulti.broadcastUInt8(iLichtKleinStatus,function,address,job);
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

void jobClearAutoDoor(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  auto_door_status = false;
  broadcastOpenDoorStatus();
  my_random_timer.Make_New();
}

void jobDoAutoDoor(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  my_random_timer.Make_New();
  if (auto_door_status==true)
  {
    open_door(true);
  }
  else
  {
    if (MyTimers[TIMER_STOP_DOOR].state == TM_RUN)
    {
      //send_answer(&serKNET,"enabled",true);
      auto_door_status = true;
    }
    else
    {
      //send_answer(&serKNET,"nope",false);
      auto_door_status = false;
    }
    broadcastOpenDoorStatus();
  }
}



/* Sendet den Key der Karte mit der Nummer keynum. Die ersten Stellen enthalten den Key, der Rest ActualRandom */
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

/* Sendet den Info der Karte mit der Nummer keynum. Die ersten Stellen enthalten den Info, der Rest ActualRandom */
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



/* Prüft auf richtigen Key: Information: 1./2. Byte Kartennummer dezimal, 3. .. Info, Rest ActualRandom */
uint8_t check_info(uint8_t *data)
{
uint8_t cardnum,i;
uint8_t info_ok = false;
	cardnum = data[0];
	if (cardnum<INFO_NUM)
	{
		i=0;
		do
		{
			if (data[i+1] == eeprom_read_byte(&InfoList[cardnum][i]))
				info_ok = true;
			else
				info_ok = false;
			i++;
		} while ( info_ok && (i<INFO_LENGTH) );
		return(info_ok);
	}
	else
		return(false);
}

uint8_t recode_data(char _node, char _function, char _job, char *original)
{
CRC_Calc cc;
uint8_t i;
	cc.Data('\\');
	cc.Data('>');
	cc.Data(_node);
	cc.Data(_function);
	cc.Data(_job);
	if (cc.Test_CRC(original,original+32,32))
	{
		for (i=0;i<16;i++)
		{
			original[i] = ((original[2*i]-65)<<4 ) | ((original[2*i+1]-65) & 0x0f );
		}
		encrypt.Decrypt_Data((uint8_t*)original);
		encrypt.Get_Data((uint8_t*)original);
		return(true);
	}
	else
		return(false);
}

void open_door(uint8_t open)
{
	if (open)
	{
		MyTimers[TIMER_ROT_TOGGLE].state = TM_STOP;
		LEDGRUEN_ON;
		OEFFNER_1;
		OEFFNER_2;
		MyTimers[TIMER_STOP_DOOR].value = 700;
		MyTimers[TIMER_STOP_DOOR].state = TM_START;
		LEDROT_OFF;
		make_blocking(true);
		kmulti.sendCommand(Bedienung,'D','1','o');
	}
	else
	{
		make_blocking(false);
		MyTimers[TIMER_ROT_TOGGLE].state = TM_START;
	}
  kmulti.sendStandardInt(Bedienung,'C','1','b',BlockingStatus);
}

void make_blocking(uint8_t reset)
{
  if(reset==true)
  {
    BlockingStatus = UNBLOCKED;
    MyTimers[TIMER_BLOCKING_END].state = TM_STOP;
  }
  else
  {
    BlockingStatus+=1;
    if(BlockingStatus == BLOCKED_LAST)
      BlockingStatus--;
    MyTimers[TIMER_BLOCKING_END].value = BlockadeZeiten[BlockingStatus-1];
    MyTimers[TIMER_BLOCKING_END].state = TM_RESET;
  }
}

