/*
 * Klingelzugang.h
 *
 * Created: 15.11.2015 20:19:17
 *  Author: Christof
 */
#ifndef TEST01_H_
#define TEST01_H_


#define WS_ARCH_XMEGA

#define INFO_NUM 10			// Anzahl der Karteninformationen
#define KEY_NUM	 10			// Anzahl der Keys
#define KEY_LENGTH 6
#define INFO_LENGTH 6

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "UART.h"
#include "CRC_Calc.h"
#include "MyTimer.h"
#include "KZG_Functions.h"
#include "CNET_Functions.h"
#include "Klingel.h"
#include "encrypt.hpp"
#include "RandomTimer.h"
#include "twi_master_driver.h"
#include "Externals.h"
#include "keys.h"
#include "watchdog.h"
#include <string.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>

#include "myConstants.h"
#include "xmegaClocks.h"
#include "xmegaClocks.h"
#include "Cmulti2Buffer.h"
#include "Communication.h"
#include "Serial.h"
#include "uartHardware.h"

#define Stringize( L )     #L
#define MakeString( M, L ) M(L)
#define Line MakeString( Stringize, __LINE__ )
#define Reminder __FILE__ "(" Line ") : Reminder: "



typedef struct Timer TIMER;

//enum {ERROR_SPEICHER=0,ERROR_PARAMETER,ERROR_NO_SMS,ERROR_HANDY,ERROR_COM_HANDY,ERROR_JOB,ERROR_TRANSMISSION};
//enum{ RCST_WAIT=0,RCST_ATTENTION,RCST_WAIT_NODE,RCST_WAIT_FUNCTION,RCST_WAIT_JOB,RCST_DO_JOB,RCST_WAIT_END1,RCST_WAIT_END2,RCST_GET_PARAMETER};

#define WDT_SHORT		WDT_PER_1KCLK_gc
#define WDT_LONG		WDT_PER_8KCLK_gc



// ------------- PORT A -------------------
#define OEFFNER_PIN_L	PIN0_bm
#define OEFFNER_PIN_H	PIN1_bm
#define LED_ROT_PIN		PIN2_bm
#define LED_GELB_PIN	PIN3_bm
#define LED_GRUEN_PIN	PIN4_bm
#define DAC_PIN2_PIN	PIN5_bm
#define DAC_PIN1_PIN	PIN6_bm
#define DAC_PIN3_PIN	PIN7_bm

// ------------- PORT B -------------------
#define DAC_PIN5_PIN		PIN0_bm
#define DAC_PIN7_PIN		PIN2_bm
#define DAC_PIN8_PIN		PIN3_bm

// ------------- PORT E -------------------
#define I2C_PIN6_PIN		PIN0_bm
#define KLINGEL1_PIN		PIN1_bm
#define KLINGEL0_PIN		PIN2_bm

// ------------- PORT C -------------------
#define RS485_0_TE_PIN		PIN0_bm
#define RS485_0_RE_PIN		PIN1_bm
#define RS485_0_RxD_PIN		PIN2_bm
#define RS485_0_TxD_PIN		PIN3_bm
#define STREET_WANT_MASTER	PIN4_bm
#define STREET_LET_MASTER	PIN5_bm
#define STREET_OUT0			PIN6_bm
#define STREET_OUT_RING		PIN7_bm

// ------------- PORT D -------------------
#define DAC_PIN10_PIN		PIN0_bm
#define TASTER_SINK			PIN1_bm
#define TASTER_PFORTE		PIN2_bm
#define HOUSE_OUT0			PIN3_bm
#define HOUSE_OUT_RING		PIN4_bm

#define RS485_1_TERE_PIN	PIN5_bm
#define RS485_1_RxD_PIN		PIN6_bm
#define RS485_1_TxD_PIN		PIN7_bm


#define LED_ROT_ON					PORTA_OUTCLR = LED_ROT_PIN
#define LED_ROT_OFF					PORTA_OUTSET = LED_ROT_PIN
#define LED_ROT_TOGGLE				PORTA_OUTTGL = LED_ROT_PIN
#define LED_GELB_ON					PORTA_OUTCLR = LED_GELB_PIN
#define LED_GELB_OFF				PORTA_OUTSET = LED_GELB_PIN
#define LED_GELB_TOGGLE				PORTA_OUTTGL = LED_GELB_PIN
#define LED_GRUEN_ON				PORTA_OUTCLR = LED_GRUEN_PIN
#define LED_GRUEN_OFF				PORTA_OUTSET = LED_GRUEN_PIN
#define LED_GRUEN_TOGGLE			PORTA_OUTTGL = LED_GRUEN_PIN

#define KLINGEL0_START				PORTE_OUTSET = KLINGEL0_PIN
#define KLINGEL1_START				PORTE_OUTSET = KLINGEL1_PIN
#define KLINGEL_STOP				PORTE_OUTCLR = KLINGEL0_PIN | KLINGEL1_PIN

#define WANT_MASTER_ALERT			PORTC_OUTSET = STREET_WANT_MASTER
#define WANT_MASTER_RELEASE			PORTC_OUTCLR = STREET_WANT_MASTER

#define OEFFNEN_1					PORTA_OUTCLR = OEFFNER_PIN_L
#define OEFFNEN_2					PORTA_OUTSET = OEFFNER_PIN_H
#define SCHLIESSEN_1				PORTA_OUTSET = OEFFNER_PIN_L
#define SCHLIESSEN_2				PORTA_OUTCLR = OEFFNER_PIN_H

enum{UNBLOCKED=0,BLOCKED0,BLOCKED1,BLOCKED2,BLOCKED3,BLOCKED4,BLOCKED5,BLOCKED_LAST};

extern USART_t *mein_serial;

void be_master_fkt();
void be_tunnel_fkt();
void init_clock();
void CLKSYS_AutoCalibration_Enable( uint8_t clkSource, bool extReference );
void init_io();
void TransmitByte( USART_t *serial, int8_t data );
void goto_sleep(int8_t test);
void new_random();
void broadcastOpenDoorStatus();
#endif /* TEST01_H_ */
