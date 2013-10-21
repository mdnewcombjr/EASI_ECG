#ifndef INTERRUPT_HANDLERS_H_
#define INTERRUPT_HANDLERS_H_
#include "stdio.h"
#include "csl_general.h"
#include "easi_uart.h"
#include "easi_datafifo.h"
#include "fixed_buffer.h"
#include "easi_spi.h"

#define TX_ADS1294_STATUS 0
#define MOVING_AVG_LENGTH 4

//Define INT_FILTER_DATA = 1 to perform a moving avg filter
//of length MOVING_AVG_LENGTH on incoming data and to enable
//the FIR filters in the main loop

#define INT_FILTER_DATA 0
#define USE_FIFO 1

//initInterruptBuffers() sets up the buffers for the moving avg
//filter
void initInterruptBuffers();

//All GPIO interrups go here.  For the EASI ECG system, this is only
//one interrupt from the ADS1294, signaling that there is data ready
interrupt void ads1294_GPIO_ISR();

#endif /*INTERRUPT_HANDLERS_H_*/
