#include "Klingelzugang.h"

volatile uint8_t iLichtKleinSet    = LICHT_SET_AUS;
volatile uint8_t iLichtKleinActual = 0;
volatile uint8_t iLichtGrossSet    = LICHT_SET_AUS;
volatile uint8_t iLichtGrossActual = 0;

volatile float fHelligkeit;
volatile uint16_t iLichtgrenzwert=200,iLichtwertHysterese=50;

const char *LichtStatus[]={"aus","ein","Auto"};

void calcLichtActualStatus()
{
uint8_t oldStatus;

  oldStatus = iLichtKleinActual;
  switch(iLichtKleinSet)
  {
    case LICHT_SET_AUS:
      iLichtKleinActual = LICHT_SET_AUS;
    break;
    case LICHT_SET_EIN:
      iLichtKleinActual = LICHT_SET_EIN;
    break;
    case LICHT_SET_AUTO:

    break;
  }
  if( iLichtKleinActual!=oldStatus)
    sendLichtActualStatus(1);

  oldStatus = iLichtGrossActual;
  switch(iLichtGrossSet)
  {
    case LICHT_SET_AUS:
      iLichtGrossActual = LICHT_SET_AUS;
    break;
    case LICHT_SET_EIN:
      iLichtGrossActual = LICHT_SET_EIN;
    break;
    case LICHT_SET_AUTO:

    break;
  }
  if( iLichtGrossActual!=oldStatus)
    sendLichtActualStatus(2);
}

void lightToggle(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  switch(address)
  {
    case '1':
      iLichtKleinSet += 1;
      if(iLichtKleinSet==LICHT_SET_UNVALID)
        iLichtKleinSet=LICHT_SET_AUS;
      cmulti.broadcastString(LichtStatus[iLichtKleinSet],'L','1','s');
      calcLichtActualStatus();
      cmulti.broadcastUInt8(iLichtKleinActual,'L','1','a');
    break;
    case '2':
      iLichtGrossSet += 1;
      if(iLichtGrossSet==LICHT_SET_UNVALID)
        iLichtGrossSet=LICHT_SET_AUS;
      cmulti.broadcastString(LichtStatus[iLichtKleinSet],'L','2','s');
      calcLichtActualStatus();
      cmulti.broadcastUInt8(iLichtGrossActual,'L','2','a');
    break;
  }
}


void sendLichtActualStatus(uint8_t adr)
{
  if(adr == 1)
  {
    cmulti.broadcastUInt8(iLichtKleinActual,'L','1','a');
    //kmulti.broadcastUInt8(iLichtKleinActual,'L','1','a');
  }
  else
  {
    cmulti.broadcastUInt8(iLichtGrossActual,'L','2','a');
    //kmulti.broadcastUInt8(iLichtGrossActual,'L','2','a');
  }
}

void sendLichtSetStatus(uint8_t adr)
{
  if(adr == 1)
  {
    cmulti.broadcastString(LichtStatus[iLichtKleinSet],'L','1','s');
    //kmulti.broadcastUInt8(LichtStatus[iLichtKleinSet],'L','1','s');
  }
  else
  {
    cmulti.broadcastString(LichtStatus[iLichtKleinSet],'L','2','s');
    //kmulti.broadcastUInt8(iLichtGrossActual,'L','2','a');
  }
}

void setLichtSet(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t status;
  // aus|ein|Auto
  switch(((char *)pMem)[0])
  {
    case 'a':
      status = LICHT_SET_AUS;
    break;
    case 'e':
      status = LICHT_SET_EIN;
    break;
    case 'A':
      status = LICHT_SET_AUTO;
    break;
    default:
      status = LICHT_SET_UNVALID;
  }
  if(status != LICHT_SET_UNVALID)
  {
    if(address=='1')
      iLichtKleinSet = status;
    else
      iLichtGrossSet = status;
    cmulti.broadcastString((char *)pMem,function,address,job);  // informierte über neuen SET-Status
    calcLichtActualStatus();
  }
  else
    comRec->sendAnswer("UNVALID",function,address,job,false);
}

