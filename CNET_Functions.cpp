/*
 * CNET_Functions.cpp
 *
 * Created: 06.05.2016 20:03:58
 *  Author: Christof
 */
#include "Klingelzugang.h"
#include "ComReceiver.h"
#include "Externals.h"
#include "../Secrets/secrets.h"
/*
uint8_t rec_state_CNET=RCST_WAIT;
uint8_t function_CNET=0;
uint8_t job_CNET=0;

char *parameter_text_CNET=NULL;
uint8_t parameter_text_length_CNET;
uint8_t parameter_text_pointer_CNET;
*/
uint8_t bootloader_attention;		// nur wenn true, dann darf Bootloader gestartet werden.
uint8_t dc_attention;				// nur wenn true, dann darf direct-channel hergestellt werden.
uint8_t reset_attention;			// nur wenn true, dann darf Reset ausgeloest werden.

float fExternalTemperature;

void (*bootloader)( void ) = (void (*)(void)) (BOOT_SECTION_START/2);       // Set up function pointer
void (*reset)( void ) = (void (*)(void)) 0x0000;       // Set up function pointer


#define NUM_COMMANDS 16

COMMAND cnetCommands[NUM_COMMANDS] =
{
  {'-','-',CUSTOMER,NOPARAMETER,0,jobGotCRCError}, // Achtung, muss immer der erste sein
  {'S','K',CUSTOMER,STRING,16,jobSetSecurityKey},
  {'S','k',CUSTOMER,NOPARAMETER,0,jobGetSecurityKey},
  {'S','C',DEVELOPMENT,NOPARAMETER,0,jobGetCompilationDate},
  {'S','T',DEVELOPMENT,NOPARAMETER,0,jobGetCompilationTime},
  {'S','m',PRODUCTION,NOPARAMETER,0,jobGetFreeMemory},
  {'M','A',CUSTOMER,STRING,16,setBootloaderAttention},    //JOB_BL_ATTENTION
  {'M','B',CUSTOMER,NOPARAMETER,0,startBootloader},    // JOB_BL_START		'B'
  {'M','r',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_RESET_ACT		'r'
  {'M','R',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_RESET			'R'
  {'M','D',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_DC_ATTENTION	'D'			// Direkten Channel zum rueckwaerigen Geraet
  {'M','C',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_DC_START		'C'
  {'M','M',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_BE_MASTER		'M'
  {'M','m',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_RELEASE_MASTER	'm'
  {'M','T',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_BE_TUNNEL		'T'
  {'M','t',CUSTOMER,NOPARAMETER,0,NULL},    // JOB_RELEASE_TUNNEL	't'

};


#define NUM_INFORMATION 1

INFORMATION cnetInformation[NUM_INFORMATION]=
{
  {"CQ",'C','1','t',FLOAT,1,(void*)&fExternalTemperature,NULL}
};

ComReceiver cnetCom(&cmulti,Node, cnetCommands,NUM_COMMANDS, cnetInformation,NUM_INFORMATION);


void jobGotCRCError(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswer(fehler_text[CRC_ERROR],function,address,job,false);
}

void setBootloaderAttention(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	if (strcmp((char *)pMem,BOOTLOADER_ATTENTION_KEY)==0)
    bootloader_attention=true;
  else
    bootloader_attention=false;
}

void startBootloader(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  comRec->sendAnswer("GotoBootloader",function,address,job,true);
  _delay_ms(600);
  bootloader();
}

void jobSetSecurityKey(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t ret = true;
	if (strcmp((char *)pMem,SECURITY_LEVEL_PRODUCTION_KEY)==0)
	{
    comRec->SetSecurityLevel(PRODUCTION);
	}
	else if(strcmp((char *)pMem,SECURITY_LEVEL_DEVELOPMENT_KEY)==0)
	{
    comRec->SetSecurityLevel(DEVELOPMENT);
	}
	else
	{
    comRec->SetSecurityLevel(CUSTOMER);
		ret = false;
	}
	comRec->sendAnswerInt(function,address,job,comRec->GetSecurityLevel(),ret);
}

void jobGetSecurityKey(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
	comRec->sendAnswerInt(function,address,job,comRec->GetSecurityLevel(),true);
}


void jobGetCompilationDate(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
char temp[20];
	strcpy(temp,Compilation_Date);
	comRec->sendAnswer(temp,function,address,job,true);
}

void jobGetCompilationTime(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
char temp[20];
	strcpy(temp,Compilation_Time);
	comRec->sendAnswer(temp,function,address,job,true);
}

void jobGetFreeMemory(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
extern int __heap_start, *__brkval;
int v;

	uint16_t mem = (uint16_t) &v - (__brkval == 0 ? (uint16_t) &__heap_start : (uint16_t) __brkval);
	comRec->sendAnswerInt(function,address,job,mem,true);
}



/*
void rec_CNET()
{
	int j,i;
	char code[16];
	char *next;
	rec_state_machine_CNET();
	if( (rec_state_CNET == RCST_DO_JOB) & (job_CNET!= 0) )
	{
		WDT_Reset();
		LED_GRUEN_ON;
		switch( function_CNET )
		{
			case 0:
				switch( job_CNET )
				{
					case JOB_ERROR:
					send_answer(&cmulti,fehler_text[ERROR_JOB],false);
					break;
				}
			break;
			case FUNCTION_SYSTEM:
				switch( job_CNET )
				{
					case JOB_BL_ATTENTION:
						send_answer(&cmulti,"Bootattention",true);
					break;
					case JOB_BL_START:
						send_answer(&cmulti,"Goto bootloader",true);
						_delay_ms(600);
						OSC.CTRL = OSC_RC32MEN_bm | OSC_RC2MEN_bm | OSC_RC32KEN_bm; // schaltet die 2 MHz-Clock ein
						while((OSC.STATUS & OSC_RC2MRDY_bm) == 0)  // wartet bis diese stabil
							;
						while((OSC.STATUS & OSC_RC32KRDY_bm) == 0)  // wartet bis diese stabil
							;
						CCP = CCP_IOREG_gc;								// geschuetztes Register freigeben
						CLK.CTRL = CLK_SCLKSEL_RC2M_gc;		// umschalten auf 32 MHz-Clock
						OSC.CTRL = OSC_RC2MEN_bm | OSC_RC32KEN_bm;		// abschalten der 2 MHz-Clock
//						OSC_RC32KCAL = 0x84;
//						CLKSYS_AutoCalibration_Enable(OSC_RC2MCREF_RC32K_gc,false); // OSC_RC32MCREF_bm
						bootloader();
					break;
					case JOB_DC_ATTENTION:
						send_answer(&cmulti,"DC-Attention",true);
					break;
					case JOB_DC_START:
						send_answer(&cmulti,"Create direct channel",true);
						_delay_ms(100);
						direct_channel();
					break;
					case JOB_BE_MASTER:
						WANT_MASTER_ALERT;
						LED_ROT_ON;
						be_master = 1;
					break;
					case JOB_RELEASE_MASTER:
						if (be_tunnel==0)
						{
							be_master = 0;
							send_answer(&cmulti,"Release Master",true);
							WANT_MASTER_RELEASE;
						}
					break;
					case JOB_BE_TUNNEL:
						if(be_master>0)
							be_tunnel = 1;
					break;
					case JOB_RELEASE_TUNNEL:
						send_answer(&cmulti,"Release Tunnel",true);
						be_tunnel=0;
					break;
					case JOB_RESET_ACT:
						send_answer(&cmulti,"Wait Reset",true);
					break;
					case JOB_RESET:
						send_answer(&cmulti,"Do Reset",true);
						reset();
					break;
				}
			break;
			case FUNCTION_BOOT:
				switch(job_CNET)
				{
					case DO_BOOTLOAD:
						send_answer(&cmulti,"Goto bootloader",true);
						_delay_ms(600);
						bootloader();
					break;
				}
			break;
			case FUNCTION_CARD:
				switch(job_CNET)
				{
					case JOB_CODE_GET_NUM:
						sprintf(code,"%i",KEY_NUM);
						send_answer(&cmulti,code,true);
					break;
					case JOB_READ_KEY:
						if( parameter_text_CNET!= NULL)
						{
							j=atoi(parameter_text_CNET);
							if( j<KEY_NUM )
							{
								eeprom_read_block(code,KeyList[j],6);
								send_hex_answer(&cmulti,(uint8_t *)code,6,true);
							}
							else
								send_answer(&cmulti,fehler_text[ERROR_PARAMETER],false);
							free_parameter_CNET();
						}
						else
							send_answer(&cmulti,fehler_text[ERROR_SPEICHER],false);
					break;
					case JOB_READ_INFO:
						if( parameter_text_CNET!= NULL)
						{
							j=atoi(parameter_text_CNET);
							if( j<INFO_NUM )
							{
								eeprom_read_block(code,InfoList[j],6);
								send_hex_answer(&cmulti,(uint8_t *)code,6,true);
							}
							else
								send_answer(&cmulti,fehler_text[ERROR_PARAMETER],false);
							free_parameter_CNET();
						}
						else
							send_answer(&cmulti,fehler_text[ERROR_SPEICHER],false);
					break;
					case JOB_WRITE_KEY_INFO_RND:
					if( parameter_text_CNET!= NULL)
					{
						j=atoi(parameter_text_CNET);
						if( j<KEY_NUM )
						{
							my_random_timer.GetRandom((uint8_t*) code);
							encrypt.Encrypt_Data((uint8_t*) code);
							encrypt.Get_Data((uint8_t*) code);
							eeprom_write_block(code,KeyList[j],6);
							eeprom_write_block(code+8,InfoList[j],6);
							send_answer(&cmulti,"iO",true);
						}
						else
							send_answer(&cmulti,fehler_text[ERROR_PARAMETER],false);
						free_parameter_CNET();
					}
					else
						send_answer(&cmulti,fehler_text[ERROR_SPEICHER],false);
					break;
				}
			break;
			case FUNCTION_CODE:
				switch(job_CNET)
				{
					case JOB_READ_CODE:
						if( parameter_text_CNET!= NULL)
						{
							j=atoi(parameter_text_CNET);
							if( j<INFO_NUM )
							{
								eeprom_read_block(code,CodeList[j],5);
								if(  strlen( code ) == 4  )
									send_answer(&cmulti,code,true);
								else
									send_answer(&cmulti,"----",true);
							}
							else
								send_answer(&cmulti,fehler_text[ERROR_PARAMETER],false);
							free_parameter_CNET();
						}
						else
							send_answer(&cmulti,fehler_text[ERROR_SPEICHER],false);
					break;
					case JOB_DELETE_CODE:
						if( parameter_text_CNET!= NULL)
						{
							j=atoi(parameter_text_CNET);
							if( j<INFO_NUM )
							{
								for(i=0;i<5;i++)
									eeprom_write_byte((uint8_t*)&CodeList[j][i],0);
								send_answer(&cmulti,"OK",true);
							}
							else
								send_answer(&cmulti,fehler_text[ERROR_PARAMETER],false);
							free_parameter_CNET();
						}
						else
							send_answer(&cmulti,fehler_text[ERROR_SPEICHER],false);
					break;
					case JOB_DELETE_CODE_ALL:
						for( i=0;i<INFO_NUM;i++ )
						{
							for (j=0;j<5;j++)
							{
								eeprom_write_byte((uint8_t*)&CodeList[i][j],0);
							}
						}
						send_answer(&cmulti,"OK",true);
					break;
					case JOB_CODE_GET_NUM:
						sprintf(code,"%i",INFO_NUM);
						send_answer(&cmulti,code,true);
					break;
					case JOB_WRITE_CODE:
						if( parameter_text_CNET!= NULL)
						{

							next = strtok(parameter_text_CNET,",");
							j=atoi(next);
							next = strtok(NULL,",");
							if (strlen(next)==4)
							{
								for (i=0;i<5;i++)
								{
									eeprom_write_byte((uint8_t*)&CodeList[j][i],next[i]);
								}
								send_answer(&cmulti,"OK",true);
							}
							else
							{
								send_answer(&cmulti,fehler_text[ERROR_PARAMETER],false);
							}

							free_parameter_CNET();
						}
						else
							send_answer(&cmulti,fehler_text[ERROR_SPEICHER],false);
					break;
				}
			break;
		}
		LED_GRUEN_OFF;
		rec_state_CNET = RCST_WAIT;
		job_CNET = 0;
		function_CNET = 0;
	}
}
*/


/*
void rec_state_machine_CNET(void)  //   NodeJob
{
	char act_char,temp;
	if( cmulti.getChar(act_char) == true )
	{
		if( (rec_state_CNET != RCST_WAIT_END2) && (act_char=='\\') )
		{
			rec_state_CNET = RCST_ATTENTION;
		}
		else
		{
			switch( rec_state_CNET )
			{
				case RCST_WAIT:
				if( act_char=='\\' )
				{
					rec_state_CNET = RCST_ATTENTION;
				}
				break;
				case RCST_ATTENTION:
					if( act_char=='>' )
						rec_state_CNET = RCST_WAIT_NODE;
					else
						rec_state_CNET = RCST_WAIT;
				break;
				case RCST_WAIT_NODE:
					if( act_char == NODE )
					{
						rec_state_CNET = RCST_WAIT_FUNCTION;
					}
					else
						rec_state_CNET = RCST_WAIT;
				break;
				case RCST_WAIT_FUNCTION:
					rec_state_CNET = RCST_WAIT_JOB;
					temp = bootloader_attention;
					bootloader_attention=false;
					switch( act_char )
					{
						case FUNCTION_SYSTEM:
							bootloader_attention=temp;
							function_CNET = act_char;
						break;
						case FUNCTION_BOOT:
						case FUNCTION_CODE:
						case FUNCTION_CARD:
							function_CNET = act_char;
						break;
						bootloader_attention = false;
						default:
							function_CNET = 0;
						break;
					}
				break;
				case RCST_WAIT_JOB:
					rec_state_CNET = RCST_WAIT_END1;
					switch( function_CNET )
					{
						case FUNCTION_RANDOM:
						switch( act_char )
						{

							case JOB_GET_RANDOM:
							job_CNET = act_char;
							break;
							case JOB_NEW_RANDOM:
							job_CNET = act_char;
							break;
							default:
							function_CNET = 0;
							job_CNET = 0;
							rec_state_CNET = RCST_WAIT;
							break;
						}
						break;
						case FUNCTION_BOOT:
							switch(act_char)
							{
								case DO_BOOTLOAD:
									job_CNET = act_char;
								break;
							}
						break;
						case FUNCTION_CARD:
							switch(act_char)
							{
								case JOB_CARD_GET_NUM:
									job_CNET = act_char;
								break;
								case JOB_READ_KEY:
									parameter_text_CNET = (char*) malloc(GET_NUM_LENGTH);
									parameter_text_length_CNET = GET_NUM_LENGTH;
									parameter_text_pointer_CNET = 0;
									job_CNET = act_char;
									rec_state_CNET = RCST_GET_PARAMETER;
								break;
								case JOB_READ_INFO:
									parameter_text_CNET = (char*) malloc(GET_NUM_LENGTH);
									parameter_text_length_CNET = GET_NUM_LENGTH;
									parameter_text_pointer_CNET = 0;
									job_CNET = act_char;
									rec_state_CNET = RCST_GET_PARAMETER;
								break;
								case JOB_WRITE_KEY_INFO_RND:
									parameter_text_CNET = (char*) malloc(GET_NUM_LENGTH);
									parameter_text_length_CNET = GET_NUM_LENGTH;
									parameter_text_pointer_CNET = 0;
									job_CNET = act_char;
									rec_state_CNET = RCST_GET_PARAMETER;
								break;
							}
						break;
						case FUNCTION_CODE:
							switch(act_char)
							{
								case JOB_CODE_GET_NUM:
									job_CNET = act_char;
								break;
								case JOB_READ_CODE:
								case JOB_DELETE_CODE:
									parameter_text_CNET = (char*) malloc(GET_NUM_LENGTH);
									parameter_text_length_CNET = GET_NUM_LENGTH;
									parameter_text_pointer_CNET = 0;
									job_CNET = act_char;
									rec_state_CNET = RCST_GET_PARAMETER;
								break;
								case JOB_DELETE_CODE_ALL:
									job_CNET = act_char;
								break;
								case JOB_WRITE_CODE:
									parameter_text_CNET = (char*) malloc(WRITE_CODE_LENGTH);
									parameter_text_length_CNET = WRITE_CODE_LENGTH;
									parameter_text_pointer_CNET = 0;
									job_CNET = act_char;
									rec_state_CNET = RCST_GET_PARAMETER;
								break;
							}
						break;
						case FUNCTION_SYSTEM:
							switch( act_char )
							{
								case JOB_BL_ATTENTION:
									job_CNET = act_char;
									bootloader_attention = true;
								break;
								case JOB_BL_START:
									if( bootloader_attention==true )
										job_CNET = act_char;
									else
									{
										bootloader_attention = false;
										function_CNET = 0;
										job_CNET = 0;
										rec_state_CNET = RCST_WAIT;
									}
								break;
								case JOB_DC_ATTENTION:
									job_CNET = act_char;
									dc_attention = true;
								break;
								case JOB_DC_START:
									if( dc_attention==true )
										job_CNET = act_char;
									else
									{
										dc_attention = false;
										function_CNET = 0;
										job_CNET = 0;
										rec_state_CNET = RCST_WAIT;
									}
								break;
								case JOB_BE_TUNNEL:
								case JOB_BE_MASTER:
								case JOB_RELEASE_TUNNEL:
								case JOB_RELEASE_MASTER:
									job_CNET = act_char;
								break;
								case JOB_RESET_ACT:
									job_CNET = act_char;
									reset_attention = true;
								break;
								case JOB_RESET:
									if( reset_attention==true )
										job_CNET = act_char;
									else
									{
										reset_attention = false;
										function_CNET = 0;
										job_CNET = 0;
										rec_state_CNET = RCST_WAIT;
									}
									break;
								default:
									bootloader_attention = false;
									dc_attention = false;
									reset_attention = false;
									function_CNET = 0;
									job_CNET = 0;
									rec_state_CNET = RCST_WAIT;
								break;
							}
						break;
						default:
							function_CNET = 0;
							job_CNET = 0;
							rec_state_CNET = RCST_WAIT;
						break;
					}
				break;
				case RCST_WAIT_END1:
				if( act_char=='<' )
					rec_state_CNET = RCST_WAIT_END2;
				else
					rec_state_CNET = RCST_WAIT;
				break;
				case RCST_WAIT_END2:
				if( act_char=='\\' )
					rec_state_CNET = RCST_DO_JOB;
				else
				{
					rec_state_CNET = RCST_WAIT;
					job_CNET = 0;
					free( parameter_text_CNET );
					parameter_text_CNET = NULL;
				}
				break;
				case RCST_GET_PARAMETER:
				if( act_char=='<' )					// Parameterende
				{
					rec_state_CNET = RCST_WAIT_END2;
					parameter_text_CNET[parameter_text_pointer_CNET] = 0;
				}
				else
				{
					if( parameter_text_pointer_CNET < parameter_text_length_CNET-2 ) // zu langer Parameter
					{
						parameter_text_CNET[parameter_text_pointer_CNET] = act_char;
						parameter_text_pointer_CNET++;
					}

					else
					{
						job_CNET = JOB_ERROR;
						function_CNET = 0;
						free( parameter_text_CNET );
						parameter_text_CNET = NULL;
					}
				}

				break;

			} // end of switch
		}
	}
}

void free_parameter_CNET(void)
{
	free( parameter_text_CNET );
	parameter_text_CNET = NULL;
	parameter_text_length_CNET = 0;
}
*/
void direct_channel()
{
char got;
char magic_string[] = "-#!habe fertig!#-";
uint8_t length = 17;
uint8_t start = 0;
	LED_ROT_OFF;
	LED_GRUEN_ON;
	do
	{
		if (cmulti.getChar(got))
		{
			if (got==magic_string[start])
				start++;
			else
				start = 0;
			LED_GRUEN_ON;
			serKNET.transmit(got);
			LED_GRUEN_OFF;
		}
		if (serKNET.getChar(got))
		{
			LED_ROT_ON;
			cmulti.transmit(got);
			LED_ROT_OFF;
		}
	} while (start<length-1);
	LED_ROT_ON;
	LED_GRUEN_OFF;
}

