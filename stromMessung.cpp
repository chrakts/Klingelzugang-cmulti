#include "Klingelzugang.h"

volatile uint16_t stromWert;

void initStromMessung()
{
  ADCA.CTRLA     = ADC_ENABLE_bm;
  ADCA.CTRLB     = ADC_CONMODE_bm;// | ADC_FREERUN_bm;
  ADCA.REFCTRL   = ADC_REFSEL_INT1V_gc | ADC_BANDGAP_bm | ADC_TEMPREF_bm;
  ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc;
  ADCA.CH0.CTRL    = ADC_CH_GAIN_16X_gc | ADC_CH_INPUTMODE_DIFFWGAIN_gc;
  ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN6_gc | ADC_CH_MUXNEG_PIN7_gc;
  ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
}

void initTemperatureMessung()
{
  ADCA.CTRLA     = ADC_ENABLE_bm;
  ADCA.CTRLB     = 0; // ADC_CONMODE_bm;
  ADCA.REFCTRL   = ADC_REFSEL_INT1V_gc | ADC_BANDGAP_bm | ADC_TEMPREF_bm;
  ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc;
  ADCA.CH0.CTRL    = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_INTERNAL_gc;
  ADCA.CH0.MUXCTRL = 0;
  ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
}

void startMeasure()
{
  ADCA.CH0.CTRL    |= ADC_CH_START_bm;
}

uint16_t readStrom()
{
int16_t result;
  result = ADCA.CH0.RES;
  return(abs(result));
}

void waitMeasure()
{

}

SIGNAL(ADCA_CH0_vect)
{
#define ANZAHL_WERTE 32

static uint8_t listPointer=0;
static uint16_t tempList[ANZAHL_WERTE];
uint16_t result;

  LEDROT_TOGGLE;
  tempList[listPointer] = readStrom();
  listPointer++;
  if(listPointer>=ANZAHL_WERTE)
  {
    result = 0;
    for(uint8_t i=0;i<ANZAHL_WERTE;i++)
      result += tempList[i];
    stromWert = (uint16_t)(result >> 5);
    listPointer=0;
  }
}
