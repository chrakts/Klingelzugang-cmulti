#ifndef EEPROMDATA_H_INCLUDED
#define EEPROMDATA_H_INCLUDED

extern uint16_t  EEMEM ee_iLichtgrenzwert,ee_iLichtwertHysterese;
extern volatile uint16_t ioldLichtgrenzwert,ioldLichtwertHysterese;


void nowSaveEEProm(uint8_t test);
void readEEData();
void writeEEData();


#endif // EEPROMDATA_H_INCLUDED
