#include "door.h"

volatile uint8_t auto_door_status = false;

void open_door(uint8_t open)
{
	if (open)
	{
	  inputStatus = DOOR_OPENING;
		LEDGRUEN_ON;
		OEFFNER_1;
		OEFFNER_2;
		MyTimers[TIMER_STOP_DOOR].value = 700;
		MyTimers[TIMER_STOP_DOOR].state = TM_START;
    sendSignalLamps(false);
    kmulti.broadcastString("Oeffne Tuer",'D','1','o');
    MyTimers[TIMER_LED_BLINKEN].state = TM_STOP;
	}
	else
	{
		MyTimers[TIMER_LED_BLINKEN].state = TM_START;
	}
}

void doorToggle(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  wakeup();
  if( inputStatus == DOOR_OPENING )
  {
    auto_door_status = true;
  }
  else
  {
    if( auto_door_status == true)
    {
      open_door(auto_door_status);
    }
  }
}

void broadcastOpenDoorStatus()
{
  cmulti.broadcastUInt8(auto_door_status,'D','0','S');
}

