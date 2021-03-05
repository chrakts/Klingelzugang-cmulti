#include "codeInput.h"

volatile uint8_t inputStatus=NO_INPUT;
volatile char testCode[7];
volatile uint8_t testCodePointer = 0;
volatile uint8_t BlockingStatus = UNBLOCKED;
uint16_t BlockadeZeiten[BLOCKED_LAST] = {50,200,400,600,1000,2000,6000};


void numberPressed(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t result;
  wakeup();
  if(inputStatus != INPUT_BLOCKED)
  {
    auto_door_status = false;
    inputStatus++;
    testCode[testCodePointer] = address;
    testCodePointer++;
    switch(inputStatus)
    {
      case INPUT_READY:
        testCode[testCodePointer]=0;
        testCodePointer = 0;
        inputStatus  = NO_INPUT;
        result = checkCodeInput(testCode);
        make_blocking(result);
        open_door(result);
      break;
      case SPECIAL_READY:
        testCode[testCodePointer]=0;
        testCodePointer = 0;
        inputStatus  = NO_INPUT;
        result = checkSpecialCodeInput(testCode);
        make_blocking(result);
      break;
    }
    sendSignalLamps(false);
  }
}

void specialPressed(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  wakeup();
  if(inputStatus != INPUT_BLOCKED)
  {
    auto_door_status = false;
    switch(address)
    {
      case 'H':
        inputStatus=NO_INPUT;
      break;
      case 'S':
        if(inputStatus==NO_INPUT)
          inputStatus=START_SPECIAL;
        else
          inputStatus=NO_INPUT;
      break;
    }
    if(inputStatus==NO_INPUT)
      testCodePointer=0;
    sendSignalLamps(false);
  }
}

void make_blocking(uint8_t reset)
{
  if(reset==true)
  {
    BlockingStatus = UNBLOCKED;
    MyTimers[TIMER_BLOCKING_END].state = TM_STOP;
    inputStatus = NO_INPUT;
  }
  else
  {
    BlockingStatus+=1;
    if(BlockingStatus == BLOCKED_LAST)
      BlockingStatus--;
    MyTimers[TIMER_BLOCKING_END].value = BlockadeZeiten[BlockingStatus-1];
    MyTimers[TIMER_BLOCKING_END].state = TM_START;
    inputStatus = INPUT_BLOCKED;
  }
}

uint8_t checkCodeInput(volatile char *toTest)
{
bool right;
uint8_t i,j;

  right = true;
  j=0;
  do
  {
    i=0;
    do
    {
      if (toTest[i]!=eeprom_read_byte((uint8_t*)&CodeList[j][i]))
        right = false;
      else
        right = true;
      i++;
    }while( (i<4) && (right) );
    j++;
  }while( (j<CODE_NUM) && (right==false) );
  return( right );
}

uint8_t checkSpecialCodeInput(volatile char *toTest)
{
bool right;
uint8_t i,j;

  right = true;
  j=0;
  do
  {
    i=0;
    do
    {
      if (toTest[i]!=eeprom_read_byte((uint8_t*)&SpecialCodeList[j][i]))
        right = false;
      else
        right = true;
      i++;
    }while( (i<6) && (right) );
    j++;
  }while( (j<CODE_NUM) && (right==false) );
  return( right );
}
