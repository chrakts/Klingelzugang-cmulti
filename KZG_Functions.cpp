/*
 * KZG_Functions.cpp
 *
 * Created: 20.02.2016 18:53:31
 *  Author: Christof
 */

#include "Klingelzugang.h"
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

ComReceiver knetCom(&kmulti,Node, knetCommands,NUM_KZG_COMMANDS, knetInformation,NUM_KZG_INFORMATION);


void jobNextLichtStatus(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  my_random_timer.Make_New();

  if(iLichtKleinStatus<2)
    iLichtKleinStatus++;
  else
    iLichtKleinStatus=0;
  kmulti.broadcastUInt8(iLichtKleinStatus,'L','1','s');
  cmulti.broadcastUInt8(iLichtKleinStatus,'L','1','s');
}

void jobPirTrigger(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  my_random_timer.Make_New();
  cmulti.broadcastUInt8(iLichtKleinStatus,function,address,job);
}

void jobGetRandom(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  uint8_t adr = getAddress(address);
  if(adr<2)
  {
    LED_ROT_OFF;
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



void jobKlingel(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  my_random_timer.Make_New();
  uint8_t adr = getAddress(address);
  auto_door_status = false;
  ring_bel(adr);
}

/* Sendet den Key der Karte mit der Nummer keynum. Die ersten Stellen enthalten den Key, der Rest ActualRandom */
void jobGetCardKey(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t tosend[KEY_LENGTH];
uint8_t i,keynum;

  uint8_t adr = getAddress(address);
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

  uint8_t adr = getAddress(address);
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
  uint8_t adr = getAddress(address);
  for(i=0;i<16;i++)
    ((uint8_t*)pMem)[i] = ((uint8_t*)pMem)[i] ^ Actual_Random[adr][i];
  open_door(check_info((uint8_t*)pMem) );
}

void jobTryCode(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t i,j,right;

  my_random_timer.Make_New();
  auto_door_status = false;
  uint8_t adr = getAddress(address);
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


/*

void rec_KNET()
{
uint8_t i,j;
uint8_t right;
	rec_state_machine_KNET();
	if( (rec_state_KNET == RCST_DO_JOB) & (job_KNET!= 0) )
	{
		switch( function_KNET )
		{
			case 0:
				switch( job_KNET )
				{
					case JOB_ERROR:
						send_answer(&serKNET,fehler_text[ERROR_JOB],false);
					break;
				}
			break;
			case FUNCTION_RANDOM:
				switch( job_KNET )
				{
					case JOB_NEW_RANDOM:
						my_random_timer.Make_New();
					break;
					case JOB_GET_RANDOM:
						LED_ROT_OFF;
						my_random_timer.Make_New();
						my_random_timer.GetRandom(Actual_Random);
						_delay_us(1200);
						encrypt.Output_Encrypt(&serKNET,'B','R','S',Actual_Random); // Zielangaben sind noch unklar
					break;
				}
			break;
			case FUNCTION_ZUGANG:
				switch( job_KNET )
				{
					case JOB_AUTO_DOOR_OFF:
						auto_door_status = false;
					break;
					case JOB_KLINGEL:
						auto_door_status = false;
						ring_bel(2);
						send_answer(&serKNET,"iO",true);
					break;
					case JOB_AUTO_DOOR:
						if (auto_door_status==true)
						{
							open_door(true);
						}
						else
						{
							if (MyTimers[TIMER_STOP_DOOR].state == TM_RUN)
							{
								send_answer(&serKNET,"enabled",true);
								auto_door_status = true;
							}
							else
							{
								send_answer(&serKNET,"nope",false);
								auto_door_status = false;
							}
						}
					break;
					case JOB_GET_KEY:						// schickt den Card-Key - Nummer der Karte im Parameter
						if( parameter_text_KNET!= NULL)
						{
							j=atoi(parameter_text_KNET);
							free_parameter_KNET();
							if (!send_key(&serKNET,j))
								send_answer(&serKNET,fehler_text[ERROR_PARAMETER],false);
						}
						else
							send_answer(&serKNET,fehler_text[ERROR_SPEICHER],false);
					break;
					case JOB_GET_INFO:						// schickt die Card-Info - Nummer der Karte im Parameter
						if( parameter_text_KNET!= NULL)
						{
							j=atoi(parameter_text_KNET);
							free_parameter_KNET();
							if (!send_info(&serKNET,j))
							send_answer(&serKNET,fehler_text[ERROR_PARAMETER],false);
						}
						else
							send_answer(&serKNET,fehler_text[ERROR_SPEICHER],false);
					break;
					case JOB_TRY_INFO:
						auto_door_status = false;
						if( parameter_text_KNET!= NULL)
						{
							if ( recode_data(NODE,function_KNET,job_KNET,parameter_text_KNET) )
							{
								open_door(check_info(parameter_text_KNET) );
							}
							else
								send_answer(&serKNET,fehler_text[ERROR_TRANSMISSION],false);
							free_parameter_KNET();
						}
						else
							send_answer(&serKNET,fehler_text[ERROR_SPEICHER],false);
					break;
					case JOB_TRY_CODE:
						auto_door_status = false;
						if( parameter_text_KNET!= NULL)
						{
							CRC_Calc cc;
							cc.Data('\\');
							cc.Data('>');
							cc.Data(NODE);
							cc.Data(FUNCTION_ZUGANG);
							cc.Data(JOB_TRY_CODE);
							if (cc.Test_CRC(parameter_text_KNET,parameter_text_KNET+32,32))
							{
								for (i=0;i<16;i++)
								{
									parameter_text_KNET[i] = ((parameter_text_KNET[2*i]-65)<<4 ) | ((parameter_text_KNET[2*i+1]-65) & 0x0f );
								}
								encrypt.Decrypt_Data((uint8_t*)parameter_text_KNET);
								encrypt.Get_Data((uint8_t*)parameter_text_KNET);
								char test[5];
								encrypt.Get_Encrypt_Code(Actual_Random, (uint8_t*)parameter_text_KNET, (uint8_t*)test, 4 );
								test[4] = 0;
								right = true;
								j=0;
								do
								{
									i=0;
									do
									{
										if (test[i]!=eeprom_read_byte((uint8_t*)&CodeList[j][i]))
											right = false;
										else
											right = true;
										i++;
									}while( (i<4) && (right) );
									j++;
								}while( (j<INFO_NUM) && (right==false) );
								open_door(right);
							}
							else
								send_answer(&serKNET,fehler_text[ERROR_TRANSMISSION],false);
							free_parameter_KNET();
						}
						else
							send_answer(&serKNET,fehler_text[ERROR_PARAMETER],false);
					break;
				}
			break;

//			case FUNCTION_SYSTEM:
//				switch( job_KNET )
//				{
//					case JOB_BL_ATTENTION:
//						send_answer(KNET,"Bootattention",true);
//					break;
//					case JOB_BL_START:
//						send_answer(KNET,"Goto bootloader",true);
//						_delay_ms(600);
//						bootloader();
//					break;
//					case JOB_RESET_ACT:
//						send_answer(KNET,"Wait Reset",true);
//					break;
//					case JOB_RESET:
//						send_answer(KNET,"Do Reset",true);
//						reset();
//					break;
//				}
//			break;
//  		case FUNCTION_BOOT:
//				switch(job_KNET)
//				{
//					case DO_BOOTLOAD:
//						send_answer(KNET,"Goto bootloader",true);
//						_delay_ms(600);
//						bootloader();
//					break;
//				}
//			break;
		}
		rec_state_KNET = RCST_WAIT;
		job_KNET = 0;
		function_KNET = 0;
	}
}

void rec_state_machine_KNET(void)  //   NodeJob
{
	char act_char;
	//if( get_char(KNET,&act_char) == true )
	if( serKNET.getChar(act_char) == true )
	{
		if( (rec_state_KNET != RCST_WAIT_END2) && (act_char=='\\') )
		{
			rec_state_KNET = RCST_ATTENTION;
		}
		else
		{
			switch( rec_state_KNET )
			{
				case RCST_WAIT:
					if( act_char=='\\' )
					{
						rec_state_KNET = RCST_ATTENTION;
					}
				break;
				case RCST_ATTENTION:
					if( act_char=='>' )
						rec_state_KNET = RCST_WAIT_NODE;
					else
						rec_state_KNET = RCST_WAIT;
				break;
				case RCST_WAIT_NODE:
					if( act_char == NODE )
					{
						rec_state_KNET = RCST_WAIT_FUNCTION;
					}
					else
					rec_state_KNET = RCST_WAIT;
				break;
				case RCST_WAIT_FUNCTION:
					rec_state_KNET = RCST_WAIT_JOB;
//					temp = bootloader_attention;
//					bootloader_attention=false;
					switch( act_char )
					{
//						case FUNCTION_SYSTEM:
//							bootloader_attention=temp;
//							function_KNET = act_char;
//						break;
//						case FUNCTION_BOOT:
//							function_KNET = act_char;
//						break;
//						bootloader_attention = false;
						case FUNCTION_RANDOM:
							function_KNET = act_char;
						break;
						case FUNCTION_ZUGANG:
							function_KNET = act_char;
						break;
						default:
							function_KNET = 0;
						break;
					}
				break;
				case RCST_WAIT_JOB:
				rec_state_KNET = RCST_WAIT_END1;
				switch( function_KNET )
				{
					case FUNCTION_RANDOM:
					switch( act_char )
					{

						case JOB_GET_RANDOM:
							job_KNET = act_char;
						break;
						case JOB_NEW_RANDOM:
							job_KNET = act_char;
						break;
						default:
							function_KNET = 0;
							job_KNET = 0;
							rec_state_KNET = RCST_WAIT;
						break;
					}
					break;
					case FUNCTION_ZUGANG:
					switch( act_char )
					{
						case JOB_AUTO_DOOR:
						case JOB_AUTO_DOOR_OFF:
						case JOB_KLINGEL:
							job_KNET = act_char;
						break;
						case JOB_GET_KEY:
							parameter_text_KNET = (char*) malloc(GET_KEY_LENGTH);
							parameter_text_length_KNET = GET_KEY_LENGTH;
							parameter_text_pointer_KNET = 0;
							job_KNET = act_char;
							rec_state_KNET = RCST_GET_PARAMETER;
						break;
						case JOB_GET_INFO:
							parameter_text_KNET = (char*) malloc(GET_INFO_LENGTH);
							parameter_text_length_KNET = GET_INFO_LENGTH;
							parameter_text_pointer_KNET = 0;
							job_KNET = act_char;
							rec_state_KNET = RCST_GET_PARAMETER;
						break;
						case JOB_TRY_CODE:
							parameter_text_KNET = (char*) malloc(TRY_CODE_LENGTH);
							parameter_text_length_KNET = TRY_CODE_LENGTH;
							parameter_text_pointer_KNET = 0;
							job_KNET = act_char;
							rec_state_KNET = RCST_GET_PARAMETER;
						break;
						case JOB_TRY_INFO:
							parameter_text_KNET = (char*) malloc(TRY_INFO_LENGTH);
							parameter_text_length_KNET = TRY_INFO_LENGTH;
							parameter_text_pointer_KNET = 0;
							job_KNET = act_char;
							rec_state_KNET = RCST_GET_PARAMETER;
						break;
						default:
							function_KNET = 0;
							job_KNET = 0;
							rec_state_KNET = RCST_WAIT;
						break;
					}
					break;
					case FUNCTION_BOOT:
					switch(act_char)
					{
						case DO_BOOTLOAD:
						job_KNET = act_char;
						break;
					}
					break;
//					case FUNCTION_SYSTEM:
//					switch( act_char )
//					{
//						case JOB_BL_ATTENTION:
//							job_KNET = act_char;
//							bootloader_attention = true;
//						break;
//						case JOB_BL_START:
//							if( bootloader_attention==true )
//								job_KNET = act_char;
//							else
//							{
//								bootloader_attention = false;
//								function_KNET = 0;
//								job_KNET = 0;
//								rec_state_KNET = RCST_WAIT;
//							}
//						break;
//						case JOB_RESET_ACT:
//						job_KNET = act_char;
//						reset_attention = true;
//						break;
//						case JOB_RESET:
//						if( reset_attention==true )
//						job_KNET = act_char;
//						else
//						{
//							reset_attention = false;
//							function_KNET = 0;
//							job_KNET = 0;
//							rec_state_KNET = RCST_WAIT;
//						}
//						break;
//						default:
//						bootloader_attention = false;
//						function_KNET = 0;
//						job_KNET = 0;
//						rec_state_KNET = RCST_WAIT;
//						break;
//					}
//					break;
					default:
						function_KNET = 0;
						job_KNET = 0;
						rec_state_KNET = RCST_WAIT;
					break;
				}
				break;
				case RCST_WAIT_END1:
					if( act_char=='<' )
						rec_state_KNET = RCST_WAIT_END2;
					else
						rec_state_KNET = RCST_WAIT;
				break;
				case RCST_WAIT_END2:
					if( act_char=='\\' )
						rec_state_KNET = RCST_DO_JOB;
					else
					{
						rec_state_KNET = RCST_WAIT;
						job_KNET = 0;
						free( parameter_text_KNET );
						parameter_text_KNET = NULL;
					}
				break;
				case RCST_GET_PARAMETER:
					if( act_char=='<' )					// Parameterende
					{
						rec_state_KNET = RCST_WAIT_END2;
						parameter_text_KNET[parameter_text_pointer_KNET] = 0;
					}
					else
					{
						if( parameter_text_pointer_KNET < parameter_text_length_KNET-2 ) // zu langer Parameter
						{
							parameter_text_KNET[parameter_text_pointer_KNET] = act_char;
							parameter_text_pointer_KNET++;
						}

						else
						{
							job_KNET = JOB_ERROR;
							function_KNET = 0;
							free( parameter_text_KNET );
							parameter_text_KNET = NULL;
						}
					}
				break;

			} // end of switch
		}
	}
}

void free_parameter_KNET(void)
{
	free( parameter_text_KNET );
	parameter_text_KNET = NULL;
	parameter_text_length_KNET = 0;
}

void send_answer(Serial *file, const char *answer, uint8_t no_error)
{
	_delay_ms(2);
	file->transmit('<');
	file->print(answer);
	if( no_error==true )
		file->transmit('.');
	else
		file->transmit('!');
	file->transmit('>');
}

void send_hex_answer(Serial * file, uint8_t *answer, uint8_t length, uint8_t no_error)
{
uint8_t i;

	_delay_ms(2);
	file->transmit('<');
	for (i=0;i<length;i++)
	{
		//output_hex(file,answer[i]);
		file->pformat("%x",answer[i]);
	}
	if( no_error==true )
		file->transmit('.');
	else
		file->transmit('!');
	file->transmit('>');
}

void send_command(Serial* file, char *command)
{
	file->print("\\>");
	file->print(command);
	file->transmit('<');
	file->transmit('\\');
}
*/
/* Sendet den Key der Karte mit der Nummer keynum. Die ersten Stellen enthalten den Key, der Rest ActualRandom */
/*
uint8_t send_key(Serial* file, uint8_t keynum)
{
uint8_t tosend[16];
uint8_t i;

	if (keynum<KEY_NUM)
	{
		for (i=0;i<KEY_LENGTH;i++)
		{
			tosend[i] = (eeprom_read_byte(&KeyList[keynum][i])) ^ Actual_Random[i];
		}
		for (i=KEY_LENGTH;i<16;i++)
		{
			tosend[i] = Actual_Random[i];
		}
		encrypt.Output_Encrypt(file,'B','R','S',tosend);
		return(true);
	}
	else
		return(false);
}
*/
/* Sendet den Info der Karte mit der Nummer keynum. Die ersten Stellen enthalten den Info, der Rest ActualRandom */
/*
uint8_t send_info(Serial* file, uint8_t infonum)
{
	uint8_t tosend[16];
	uint8_t i;

	if (infonum<INFO_NUM)
	{
		for (i=0;i<INFO_LENGTH;i++)
		{
			tosend[i] = (eeprom_read_byte(&InfoList[infonum][i])) ^ Actual_Random[i];
		}
		for (i=INFO_LENGTH;i<16;i++)
		{
			tosend[i] = Actual_Random[i];
		}
		encrypt.Output_Encrypt(file,'B','R','S',tosend);
		return(true);
	}
	else
	return(false);
}
*/

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
		LED_GRUEN_ON;
		OEFFNEN_1;
		OEFFNEN_2;
		MyTimers[TIMER_STOP_DOOR].value = 700;
		MyTimers[TIMER_STOP_DOOR].state = TM_START;
		LED_ROT_OFF;
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

void ring_bel(uint8_t klingel)
{
	if (klingel == 0)
		KLINGEL0_START;
	else
		KLINGEL1_START;
	LED_ROT_ON;
	MyTimers[TIMER_STOP_RING_BEL].value = 700;
	MyTimers[TIMER_STOP_RING_BEL].state = TM_START;
	cmulti.broadcastUInt8(klingel,'K',klingel,'r');
}

uint8_t getAddress(char adr)
{
  if(adr== '1')
    return(1);
  else
    return(0);
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

