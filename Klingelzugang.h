/*
 * Klingelzugang.h
 *
 * Created: 15.11.2015 20:19:17
 *  Author: Christof
 */
#ifndef TEST01_H_
#define TEST01_H_

#define INFO_NUM 10			// Anzahl der Karteninformationen
//#define SPECIAL_NUM 10			// Anzahl der Karteninformationen
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
#include "myTimers.h"
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
#include "ComReceiver.h"
#include "CNET_Functions.h"
#include "Serial.h"
#include "uartHardware.h"
#include "Licht.h"
#include "codeInput.h"
#include "door.h"
#include "signalLamps.h"
#include "ws2812.h"

#define Stringize( L )     #L
#define MakeString( M, L ) M(L)
#define Line MakeString( Stringize, __LINE__ )
#define Reminder __FILE__ "(" Line ") : Reminder: "



typedef struct Timer TIMER;

//enum {ERROR_SPEICHER=0,ERROR_PARAMETER,ERROR_NO_SMS,ERROR_HANDY,ERROR_COM_HANDY,ERROR_JOB,ERROR_TRANSMISSION};
//enum{ RCST_WAIT=0,RCST_ATTENTION,RCST_WAIT_NODE,RCST_WAIT_FUNCTION,RCST_WAIT_JOB,RCST_DO_JOB,RCST_WAIT_END1,RCST_WAIT_END2,RCST_GET_PARAMETER};

#define WDT_SHORT		WDT_PER_1KCLK_gc
#define WDT_LONG		WDT_PER_8KCLK_gc


#ifdef MAIN_BUILT

#define LEDROT_PORT   PORTA
#define LEDROT_PIN    PIN2_bm
#define LEDGRUEN_PORT PORTA
#define LEDGRUEN_PIN  PIN4_bm
#define LEDGELB_PORT  PORTA
#define LEDGELB_PIN   PIN3_bm

#define KLINGEL_PORT    PORTE
#define KLINGEL1_PIN		PIN1_bm
#define KLINGEL0_PIN		PIN2_bm

#define OEFFNER_PORT  PORTA
#define OEFFNER_PIN_L	PIN0_bm
#define OEFFNER_PIN_H	PIN1_bm

#define TASTER_PORT     PORTD
#define TASTER_LED			PIN1_bm
#define TASTER_PORT_C    D
#define TASTER_INPUT_C   2
#define TASTER_INT_NUM_C 0

// ------------- PORT A -------------------

#define DAC_PIN2_PIN	PIN5_bm
#define DAC_PIN1_PIN	PIN6_bm
#define DAC_PIN3_PIN	PIN7_bm

// ------------- PORT B -------------------
#define DAC_PIN5_PIN		PIN0_bm
#define DAC_PIN7_PIN		PIN2_bm
#define DAC_PIN8_PIN		PIN3_bm

// ------------- PORT E -------------------
#define I2C_PIN6_PIN		PIN0_bm

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
#define HOUSE_OUT0			PIN3_bm
#define HOUSE_OUT_RING		PIN4_bm

#define RS485_1_TERE_PIN	PIN5_bm
#define RS485_1_RxD_PIN		PIN6_bm
#define RS485_1_TxD_PIN		PIN7_bm


#define WANT_MASTER_ALERT			PORTC_OUTSET = STREET_WANT_MASTER
#define WANT_MASTER_RELEASE			PORTC_OUTCLR = STREET_WANT_MASTER

#endif // MAIN_BUILT

#ifdef PLUG_BUILT

/* LED auf Basisplatine */

#define LEDROT_PORT   PORTA
#define LEDROT_PIN    PIN2_bm
#define LEDGRUEN_PORT PORTA
#define LEDGRUEN_PIN  PIN3_bm
#define LEDGELB_PORT  PORTA
#define LEDGELB_PIN   PIN4_bm

#define KLINGEL_PORT    PORTA
#define KLINGEL1_PIN		PIN5_bm
#define KLINGEL0_PIN		PIN6_bm

#define OEFFNER_PORT  PORTA
#define OEFFNER_PIN_L	PIN0_bm
#define OEFFNER_PIN_H	PIN1_bm

#define TASTER_PORT_C    C
#define TASTER_INPUT_C   5
#define TASTER_INT_NUM_C 0
#define TASTER_LED			 PIN4_bm


#endif // PLUG_BUILT


#define LEDROTSETUP     LEDROT_PORT.DIRSET=LEDROT_PIN
#define LEDROT_ON       LEDROT_PORT.OUTCLR=LEDROT_PIN
#define LEDROT_OFF      LEDROT_PORT.OUTSET=LEDROT_PIN
#define LEDROT_TOGGLE   LEDROT_PORT.OUTTGL=LEDROT_PIN

#define LEDGRUENSETUP     LEDGRUEN_PORT.DIRSET=LEDGRUEN_PIN
#define LEDGRUEN_ON       LEDGRUEN_PORT.OUTCLR=LEDGRUEN_PIN
#define LEDGRUEN_OFF      LEDGRUEN_PORT.OUTSET=LEDGRUEN_PIN
#define LEDGRUEN_TOGGLE   LEDGRUEN_PORT.OUTTGL=LEDGRUEN_PIN

#define LEDGELBSETUP    LEDGELB_PORT.DIRSET=LEDGELB_PIN
#define LEDGELB_ON      LEDGELB_PORT.OUTCLR=LEDGELB_PIN
#define LEDGELB_OFF     LEDGELB_PORT.OUTSET=LEDGELB_PIN
#define LEDGELB_TOGGLE  LEDGELB_PORT.OUTTGL=LEDGELB_PIN

#define KLINGEL0_START				KLINGEL_PORT.OUTSET=KLINGEL0_PIN
#define KLINGEL1_START				KLINGEL_PORT.OUTSET=KLINGEL1_PIN
#define KLINGEL_STOP				  KLINGEL_PORT.OUTCLR = KLINGEL0_PIN && KLINGEL1_PIN

#define OEFFNER_1					OEFFNER_PORT.OUTCLR = OEFFNER_PIN_L
#define OEFFNER_2					OEFFNER_PORT.OUTSET = OEFFNER_PIN_H
#define SCHLIESSEN_1			OEFFNER_PORT.OUTSET = OEFFNER_PIN_L
#define SCHLIESSEN_2			OEFFNER_PORT.OUTCLR = OEFFNER_PIN_H

#define TASTER_PORT        JOIN(PORT,TASTER_PORT_C)
#define TASTER_INPUT		   JOIN3(PIN,TASTER_INPUT_C,_bm)   // PINx_bm
#define TASTER_INTPIN      JOIN3(PIN,TASTER_INPUT_C,CTRL)  // PINxCTRL
#define TASTERLEDSETUP     TASTER_PORT.DIRSET=TASTER_LED
#define TASTERLED_ON       TASTER_PORT.OUTCLR=TASTER_LED
#define TASTERLED_OFF      TASTER_PORT.OUTSET=TASTER_LED
#define TASTERLED_TOGGLE   TASTER_PORT.OUTTGL=TASTER_LED
#define TASTERINPUTSETUP   TASTER_PORT.DIRCLR=TASTER_INPUT
#define TASTER_INT_VEC     JOIN5( PORT,TASTER_PORT_C,_INT,TASTER_INT_NUM_C,_vect ) //PORTx_INTx_vect
#pragma message "Taster-Interrupt: " XSTR(TASTER_INT_VEC)

void init_io();
void broadcastOpenDoorStatus();
void wakeup();

#endif /* TEST01_H_ */
