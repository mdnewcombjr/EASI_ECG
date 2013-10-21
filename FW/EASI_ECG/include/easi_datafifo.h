/**************************************************
 * File: easi_datafifo.h
 * Author: Mark Newcomb
 * Description: Wraps the fifo struct in functions that make
  	handling 24 bit samples easier  
 *************************************************/

#ifndef EASI_DATAFIFO_H_
#define EASI_DATAFIFO_H_

#include "fifo.h"

#define DATAFIFO_SAMPLE_LEN 20
#define DATAFIFO_SIZE (DATAFIFO_SAMPLE_LEN*3+4) //Each sample is 3 bytes, so should be a multiple of 3
#define NUM_CHANNELS 4

typedef enum e_FIFO_CHANNEL {CHAN_1 = 0, CHAN_2, CHAN_3, CHAN_4} FIFO_CHANNEL;

void EASI_SAMPLE_FIFO_INIT();

Bool EASI_SAMPLE_FIFO_ISEMPTY(FIFO_CHANNEL chan);

Bool EASI_SAMPLE_FIFO_PUSH_8BIT(FIFO_CHANNEL chan,Uint8 msb, Uint8 mb, Uint8 lsb);

Bool EASI_SAMPLE_FIFO_PUSH(FIFO_CHANNEL chan, Uint32 sample);

Bool EASI_SAMPLE_FIFO_POP(FIFO_CHANNEL chan, Int32 *sample);

#endif /*EASI_DATAFIFO_H_*/
