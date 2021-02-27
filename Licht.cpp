#include "Klingelzugang.h"

volatile uint8_t iLichtKleinSet    = LICHT_SET_AUS;
volatile uint8_t iLichtKleinActual = 0;
volatile uint8_t iLichtGrossSet    = LICHT_SET_AUS;
volatile uint8_t iLichtGrossActual = 0;

volatile float fHelligkeit=300;
volatile uint16_t iLichtgrenzwert=200,iLichtwertHysterese=50;

const char *LichtStatus[]={"aus","ein","Auto","PIR"};

void updateLicht(uint8_t setStatus,char adr)
{
  cmulti.broadcastString(LichtStatus[setStatus],'L',adr,'s');  // send set status
  calcAndSendLichtActualStatus();
  sendSignalLamps();
}

void calcAndSendLichtActualStatus()
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
      iLichtKleinActual = calcHysterese(oldStatus);
    break;
    case LICHT_SET_PIR:
      iLichtKleinActual = calcPIR();
    break;
  }
  if( iLichtKleinActual!=oldStatus)
    cmulti.broadcastUInt8(iLichtKleinActual,'L','1','a');         // send actual status

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
      iLichtGrossActual = calcHysterese(oldStatus);
    break;
    case LICHT_SET_PIR:
      iLichtGrossActual = calcPIR();
    break;
  }
  if( iLichtGrossActual!=oldStatus)
    cmulti.broadcastUInt8(iLichtGrossActual,'L','2','a');         // send actual status
}

uint8_t calcHysterese(uint8_t oldValue)
{
  if(oldValue==LICHT_SET_EIN)
  {
    if(fHelligkeit>(float)iLichtgrenzwert*(1.0+(float)iLichtwertHysterese/100.0))
      return(LICHT_SET_AUS);
  }
  else
  {
    if(fHelligkeit<(float)iLichtgrenzwert)
      return(LICHT_SET_EIN);
  }
  return( oldValue );
}

uint8_t calcPIR()
{
  if( (inputStatus!=INPUT_SLEEP) && (fHelligkeit<(float)iLichtgrenzwert) )
    return(LICHT_SET_AUS);
  else
    return(LICHT_SET_EIN);
}

void lightToggle(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
  wakeup();
  switch(address)
  {
    case '1':
      iLichtKleinSet += 1;
      if(iLichtKleinSet==LICHT_SET_UNVALID)
        iLichtKleinSet=LICHT_SET_AUS;
      updateLicht(iLichtKleinSet,address);
    break;
    case '2':
      iLichtGrossSet += 1;
      if(iLichtGrossSet==LICHT_SET_UNVALID)
        iLichtGrossSet=LICHT_SET_AUS;
      updateLicht(iLichtGrossSet,address);
    break;
  }
}

void setLichtSet(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
uint8_t status;
   wakeup();
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
    case 'P':
      status = LICHT_SET_PIR;
    break;
    default:
      status = LICHT_SET_UNVALID;
  }
  if(status != LICHT_SET_UNVALID)
  {
    if(address=='1')
    {
      iLichtKleinSet = status;
      updateLicht(iLichtKleinSet,address);
    }
    else
    {
      iLichtGrossSet = status;
      updateLicht(iLichtGrossSet,address);
    }

  }
  else
    comRec->sendAnswer("UNVALID",function,address,job,false);
}

