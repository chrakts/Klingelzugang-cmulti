/*
 * Klingel.h
 *
 * Created: 09.02.2016 20:24:01
 *  Author: Christof
 */


#ifndef KLINGEL_H_
#define KLINGEL_H_

void init_klingel(void);
void jobKlingel(ComReceiver *comRec, char function,char address,char job, void * pMem);

void ring_bel(char klingel);



#endif /* KLINGEL_H_ */
