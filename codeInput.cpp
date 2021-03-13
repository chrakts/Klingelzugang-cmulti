#include "codeInput.h"

volatile uint8_t inputStatus=NO_INPUT;
volatile char testCode[9];
volatile uint8_t testCodePointer = 0;
volatile uint8_t BlockingStatus = UNBLOCKED;
uint16_t BlockadeZeiten[BLOCKED_LAST] = {50,200,400,600,1000,2000,6000};


void numberPressed(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t result, cardNumber;
  wakeup();
  if( (inputStatus < INPUT_BLOCKED1) | ((inputStatus > INPUT_BLOCKED)) )
  {
    auto_door_status = false;
    if( (inputStatus==GOT_BAD_CODE) | (inputStatus==GOT_GOOD_CODE) )
    {
        inputStatus  = NO_INPUT;
        testCodePointer = 0;
    }
    else
    {
      inputStatus++;
      testCode[testCodePointer] = address;
      testCodePointer++;
    }
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
        switch(result)
        {
          case SC_NEW_CODE:
            inputStatus = GET_NEW_CODE1;
          break;
          case SC_WRITE_NEW_CARD:
            inputStatus = WRITE_NEW_CARD1;
          break;
          case SC_LOCK_CARD:
            inputStatus = LOCK_CARD_NUMBER_1;
          break;
          case SC_DELETE_CARD:
            inputStatus = DELETE_CARD_READY;
            kmulti.sendCommand("ZB",'C','1','p');
          break;
          default:
            make_blocking(false);
          break;
        }
      break;
      case GET_NEW_READY:
        testCode[testCodePointer]=0;
        testCodePointer=0;
        uint8_t check;
        do
        {
          check = (testCode[testCodePointer]==testCode[testCodePointer+4]);
          testCodePointer++;
        }while( (testCodePointer<4) & (check==true) );
        if(check==true)
        {
          for(uint8_t i=0;i<4;i++)
            eeprom_update_byte((uint8_t*)&CodeList[0][i],testCode[i]);
          inputStatus  = GOT_GOOD_CODE;
        }
        else
          inputStatus  = GOT_BAD_CODE;
        testCodePointer = 0;
      break;
      case WRITE_NEW_CARD_NUMBER_READY:
        testCode[testCodePointer]=0;
        cardNumber = atoi((const char*)testCode);

        if(cardNumber<KEY_NUM)
        {
          uint8_t toTransfer[KEY_LENGTH+INFO_LENGTH+1];
          for(uint8_t i=0;i<KEY_LENGTH;i++)
            toTransfer[i]  = eeprom_read_byte(&KeyList[cardNumber][i]);
          for(uint8_t i=0;i<INFO_LENGTH;i++)
            toTransfer[i+KEY_LENGTH]  = eeprom_read_byte(&InfoList[cardNumber][i]);
          toTransfer[KEY_LENGTH+INFO_LENGTH]=0;
          kmulti.sendByteArray(toTransfer,KEY_LENGTH+INFO_LENGTH,"ZB",'S','C','0'+cardNumber,'w');
          inputStatus = WRITE_NEW_CARD_WAITING;
        }
        else
        {
          inputStatus  = NO_INPUT;

        }
        testCodePointer = 0;
      break;
      case LOCK_CARD_NUMBER_READY:
        testCode[testCodePointer]=0;
        cardNumber = atoi((const char*)testCode);

        if(cardNumber<KEY_NUM)
        {
          for(uint8_t i=0;i<KEY_LENGTH;i++)
            eeprom_update_byte(&KeyList[cardNumber][i],0xff);
          for(uint8_t i=0;i<INFO_LENGTH;i++)
            eeprom_update_byte(&InfoList[cardNumber][i],0xff);
          inputStatus  = LOCK_CARD_NUMBER_WAITING;
        }
        else
        {
          inputStatus  = NO_INPUT;

        }
        testCodePointer = 0;
      break;
    }
    sendSignalLamps(false);
  }
}

void specialPressed(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  wakeup();
  if( (inputStatus < INPUT_BLOCKED1) | ((inputStatus > INPUT_BLOCKED)) )
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
  }while( (j<SPECIAL_NUM) && (right==false) );
  if(right==false)
    return(SC_UNVALID_CODE);
  else
    return( j-1 );
}
