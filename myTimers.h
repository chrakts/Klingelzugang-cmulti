/*
 * MyTimer.h
 *
 * Created: 11.02.2016 20:20:03
 *  Author: Christof
 */


#ifndef MYTIMER_H_
#define MYTIMER_H_

#include "timer.h"
#include "Externals.h"
#include "myConstants.h"

#define MYTIMER_NUM	9

enum{TIMER_BEEPER_OFF,TIMER_NEXT_REPORT,TIMER_LED_BLINKEN,TIMER_SLEEP,TIMER_STOP_DOOR,TIMER_STOP_RING_BEL,TIMER_ENTPRELLT_TASTER,TIMER_BLOCKING_END,TIMER_SAVE_DELAY};

void nextReport(uint8_t test);
void Beeper_Ready(uint8_t test);
void LED_toggle(uint8_t test);
void Licht_Gross(uint8_t test);
void goto_sleep(uint8_t test);
void Stop_Open_Door(uint8_t test);
void Stop_Ring_Bel(uint8_t test);
void LED_rot_toggle(uint8_t test);
void BlockingEnde(uint8_t test);

extern volatile TIMER MyTimers[MYTIMER_NUM];

#endif /* MYTIMER_H_ */
