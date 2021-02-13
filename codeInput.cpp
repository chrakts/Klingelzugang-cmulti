#include "codeInput.h"

enum{NO_INPUT,INPUT_1,INPUT_2,INPUT_3,INPUT_READY,START_SPECIAL,SPECIAL_1,SPECIAL_2,SPECIAL_3,SPECIAL_4,SPECIAL_5,SPECIAL_READY};

volatile uint8_t inputStatus=NO_INPUT;
volatile char testCode[7];
volatile uint8_t testCodePointer = 0;

void numberPressed(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  inputStatus++;
  testCode[testCodePointer] = address;
  testCodePointer++;
  switch(inputStatus):
  {
    case INPUT_READY:
      testCode[testCodePointer]=0
      testCodePointer = 0;
      inputStatus  = 0;
      #info Hier Funktion zum INPUT
    break;
    case SPECIAL_READY:
      testCode[testCodePointer]=0
      testCodePointer = 0;
      inputStatus  = 0;
      #info Hier Funktion zum Special
    break;
  }
}

void specialPressed(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  switch(address):
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
}

