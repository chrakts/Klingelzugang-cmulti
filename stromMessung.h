#ifndef STROMMESSUNG_H_INCLUDED
#define STROMMESSUNG_H_INCLUDED

extern volatile uint16_t stromWert;

void initTemperatureMessung();
void initStromMessung();
void startMeasure();
uint16_t readStrom();

#endif // STROMMESSUNG_H_INCLUDED
