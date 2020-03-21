/*
 * ComReceiver.cpp
 *
 * Created: 15.04.2017 06:14:06
 *  Author: Christof
 */
#include "ComReceiver_alt.h"
#include "CommandFunctions.h"
#include <ctype.h>



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


#define NUM_COMMANDS 16
#define NUM_INFORMATION 8
