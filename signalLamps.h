#ifndef SIGNALLAMPS_H_INCLUDED
#define SIGNALLAMPS_H_INCLUDED

#include "Klingelzugang.h"

enum{C_SCHWARZ=65,C_BLAU,C_WEISS,C_ROT,C_GRUEN,C_LILA,C_GELB,C_ORANGE,C_TUERKIS,C_BRAUN};

char getLedAutobright();
char getKlingelAutobright();
void sendSignalLamps();
void fillColor(uint8_t start,uint8_t ende,uint8_t color);

#endif // SIGNALLAMPS_H_INCLUDED

