#ifndef LICHT_H_INCLUDED
#define LICHT_H_INCLUDED

extern volatile uint8_t iLichtKleinSet;
extern volatile uint8_t iLichtKleinActual;
extern volatile uint8_t iLichtGrossSet;
extern volatile uint8_t iLichtGrossActual;

extern volatile float fHelligkeit;
extern volatile uint16_t iLichtgrenzwert,iLichtwertHysterese;

enum{LICHT_SET_AUS=0,LICHT_SET_EIN,LICHT_SET_AUTO,LICHT_SET_UNVALID};

void setLichtSet(ComReceiver *comRec, char function,char address,char job, void * pMem);
void sendLichtActualStatus(uint8_t adr);
void calcLichtActualStatus();
void lightToggle(ComReceiver *comRec, char function,char address,char job, void * pMem);

#endif // LICHT_H_INCLUDED
