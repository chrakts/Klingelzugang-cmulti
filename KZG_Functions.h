/*
 * KZG_Functions.h
 *
 * Created: 20.02.2016 19:22:42
 *  Author: Christof
 */


#ifndef KZG_FUNCTIONS_H_
#define KZG_FUNCTIONS_H_

#include "Serial.h"

#define NODE			'Z'

#define FUNCTION_RANDOM	'R'
#define JOB_NEW_RANDOM	'N'
#define JOB_GET_RANDOM	'G'

#define FUNCTION_ZUGANG	'Z'
#define JOB_TRY_CODE	'C'
#define JOB_GET_KEY		'K'			// schickt den Kartenkey
#define JOB_GET_INFO	'G'
#define JOB_TRY_INFO	'I'
#define JOB_KLINGEL		'B'
#define JOB_AUTO_DOOR	'A'
#define JOB_AUTO_DOOR_OFF	'a'

#define FUNCTION_SYSTEM		'S'
#define JOB_BL_ATTENTION	'A'
#define JOB_BL_START		'B'
#define JOB_RESET_ACT		'r'
#define JOB_RESET			'R'

#define FUNCTION_BOOT	'B'
#define DO_BOOTLOAD 	's'

#define JOB_ERROR		'!'

#define TRY_CODE_LENGTH		45
#define TRY_INFO_LENGTH		45
#define GET_KEY_LENGTH		4
#define GET_INFO_LENGTH		4
#define GET_NUM_LENGTH		3
#define WRITE_CODE_LENGTH	8

//enum {ERROR_SPEICHER=0,ERROR_PARAMETER,ERROR_NO_SMS,ERROR_HANDY,ERROR_COM_HANDY,ERROR_JOB,ERROR_TRANSMISSION};
//enum{ RCST_WAIT=0,RCST_ATTENTION,RCST_WAIT_NODE,RCST_WAIT_FUNCTION,RCST_WAIT_JOB,RCST_DO_JOB,RCST_WAIT_END1,RCST_WAIT_END2,RCST_GET_PARAMETER};


void rec_KNET();
void rec_state_machine_KNET(void);
void free_parameter_KNET(void);
void send_answer(Serial* file, const char *answer, uint8_t no_error);
void send_hex_answer(Serial* file, uint8_t *answer, uint8_t length, uint8_t no_error);
void send_command(Serial* file, const char *command);
uint8_t send_key(Serial* file, uint8_t keynum);
uint8_t send_info(Serial* file, uint8_t infonum);
uint8_t check_info(char *data);
uint8_t recode_data(char _node, char _function, char _job, char *original);
void open_door(uint8_t open);
void ring_bel(uint8_t klingel);

#endif /* KZG_FUNCTIONS_H_ */
