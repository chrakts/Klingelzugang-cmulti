#include "signalLamps.h"

char signalStatus[13];
char signalStatusOld[13] ={0};

#define NUM_AUTO_GAIN 16

uint16_t AutoGain[NUM_AUTO_GAIN]= {0,3,6,12,20,50,120,240,480,960,1920,4000,8000,16000,32000,40960};
uint16_t KlingelGain[6]= {0,0,200,800,10080,40960};

char getLedAutobright()
{
uint8_t i=NUM_AUTO_GAIN-1;
	while( (fHelligkeit<AutoGain[i]) & (i!=0) )
		i--;
	return((char)i+65+1); // plus 1 damit 100% 16/16 sind
}

char getKlingelAutobright()
{
uint8_t	i=5;
	while( (fHelligkeit<KlingelGain[i]) & (i!=0) )
		i--;
	return((char)i+65);
}

void sendSignalLamps()
{
  switch(inputStatus)
  {
    case INPUT_BLOCKED:
      fillColor(0,6,C_ROT);
    break;
    case INPUT_BLOCKED5:
      fillColor(0,5,C_ROT);
      fillColor(5,6,C_GRUEN);
    break;
    case INPUT_BLOCKED4:
      fillColor(0,4,C_ROT);
      fillColor(4,6,C_GRUEN);
    break;
    case INPUT_BLOCKED3:
      fillColor(0,3,C_ROT);
      fillColor(3,6,C_GRUEN);
    break;
    case INPUT_BLOCKED2:
      fillColor(0,2,C_ROT);
      fillColor(2,6,C_GRUEN);
    break;
    case INPUT_BLOCKED1:
      fillColor(0,1,C_ROT);
      fillColor(1,6,C_GRUEN);
    break;
    case INPUT_SLEEP:
      fillColor(0,6,C_LILA);
    break;
    case NO_INPUT:
      fillColor(0,6,C_WEISS);
    break;
    case INPUT_1:
      fillColor(0,6,C_WEISS);
      signalStatus[1] = C_GRUEN;
    break;
    case INPUT_2:
      fillColor(0,6,C_WEISS);
      signalStatus[1] = C_GRUEN;
      signalStatus[2] = C_GRUEN;
    break;
    case INPUT_3:
      fillColor(0,6,C_WEISS);
      signalStatus[1] = C_GRUEN;
      signalStatus[2] = C_GRUEN;
      signalStatus[3] = C_GRUEN;
    break;
    case INPUT_READY:
      fillColor(1,5,C_GRUEN);
      signalStatus[0] = C_WEISS;
      signalStatus[5] = C_WEISS;
    break;
    case DOOR_OPENING:
      fillColor(0,6,C_GRUEN);
    break;
    case START_SPECIAL:
      fillColor(0,6,C_GELB);
    break;
    case SPECIAL_1:
      signalStatus[0] = C_GRUEN;
      fillColor(1,6,C_GELB);
    break;
    case SPECIAL_2:
      signalStatus[0] = C_GRUEN;
      signalStatus[1] = C_GRUEN;
      fillColor(2,6,C_GELB);
    break;
    case SPECIAL_3:
      fillColor(0,3,C_GRUEN);
      fillColor(3,6,C_GELB);
    break;
    case SPECIAL_4:
      fillColor(0,4,C_GRUEN);
      fillColor(4,6,C_GELB);
    break;
    case SPECIAL_5:
      fillColor(0,5,C_GRUEN);
      signalStatus[5] = C_GELB;
    break;
    case SPECIAL_READY:
      fillColor(0,6,C_GRUEN);
    break;
  }
  switch(iLichtGrossSet)
  {
    case LICHT_SET_AUS:
      signalStatus[6]  = C_ROT;
    break;
    case LICHT_SET_EIN:
      signalStatus[6]= C_ORANGE;
    break;
    case LICHT_SET_AUTO:
      signalStatus[6]= C_BLAU;
    break;
    case LICHT_SET_PIR:
      signalStatus[6]= C_TUERKIS;
    break;
  }
  switch(iLichtKleinSet)
  {
    case LICHT_SET_AUS:
      signalStatus[7]  = C_ROT;
    break;
    case LICHT_SET_EIN:
      signalStatus[7]= C_ORANGE;
    break;
    case LICHT_SET_AUTO:
      signalStatus[7]= C_BLAU;
    break;
    case LICHT_SET_PIR:
      signalStatus[7]= C_TUERKIS;
    break;
  }
  signalStatus[8] = C_SCHWARZ;
  if(auto_door_status==true)
    signalStatus[9] = C_GRUEN;
  else
    signalStatus[9] = C_GELB;
  signalStatus[10] = getLedAutobright();
  signalStatus[11] = getKlingelAutobright();
  signalStatus[12] = 0;
  if(strcmp(signalStatusOld,signalStatus)!=0)
  {
    kmulti.broadcastString(signalStatus,'S','1','s');
    strcpy(signalStatusOld,signalStatus);
  }
}

void fillColor(uint8_t start,uint8_t ende,uint8_t color)
{
uint8_t i;
  for(i=start;i<ende;i++)
  {
    signalStatus[i] = color;
  }
}
