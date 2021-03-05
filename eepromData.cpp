#include "Klingelzugang.h"

volatile uint16_t ioldLichtgrenzwert=200,ioldLichtwertHysterese=50;

uint16_t  EEMEM ee_iLichtgrenzwert=3,ee_iLichtwertHysterese=10;

void readEEData()
{
  iLichtgrenzwert      = eeprom_read_word(&ee_iLichtgrenzwert);
  iLichtwertHysterese  = eeprom_read_word(&ee_iLichtwertHysterese);
  ioldLichtgrenzwert      = iLichtgrenzwert   ;
  ioldLichtwertHysterese  = iLichtwertHysterese ;
}

void writeEEData()
{
    if( (ioldLichtgrenzwert      != iLichtgrenzwert     ) |
        (ioldLichtwertHysterese  != iLichtwertHysterese )
    )
    {
      MyTimers[TIMER_SAVE_DELAY].state = TM_START; // Speicherverzögerung läuft los
      ioldLichtgrenzwert      = iLichtgrenzwert   ;
      ioldLichtwertHysterese  = iLichtwertHysterese ;
    }
}

void nowSaveEEProm(uint8_t test)
{
  eeprom_update_word(&ee_iLichtgrenzwert,iLichtgrenzwert);
  eeprom_update_word(&ee_iLichtwertHysterese,iLichtwertHysterese);
}

