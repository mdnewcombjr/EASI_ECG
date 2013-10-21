/*
 *  Copyright 2009 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

#include "stdio.h"
#include "csl_general.h"
#include "csl_intc.h"
#include "easi_pll.h"
#include "easi_rtc.h"
#include "easi_gpio.h"
#include "easi_uart.h"
#include "easi_spi.h"
#include "easi_datafifo.h"
#include "easi_filter.h"
#include "ads1294.h"
#include "interrupt_handlers.h"

#define UART_BUFSIZE 48
#define STARTUP_WAIT_TIME 2 //Number of seconds to wait before measuring the offset of the channels
//Reference the start of the interrupt vector table - defined in vectors.asm
extern void VECSTART(void);

void _wait(Uint32 delay);
void waitusec(Uint32 usec);

//Basically performs the same function as the GEL file, verbatim.
void devicePllWorkaround();


void DEVICE_INIT();

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void main( void )
{	
	Int32 currentSample1 = 0,currentSample2 = 0, currentSample3 = 0, currentSample4 = 0;
	Int32 ch1Offset = 0, ch2Offset = 0, ch3Offset = 0, ch4Offset = 0;
	Uint8 uartBuf[UART_BUFSIZE] = {0};
	Uint8 i = 0;
	Uint16 curSec = 0xFFFF, targetSec = 0xFFFF;
	Bool startupComplete = FALSE; //A flag to see if the startup offset calibration has been completed
	
	//Configure and start the PLL, RTC, UART, SPI, GPIO, etc
	DEVICE_INIT();
   	
   	wait(1000);
	
	EASI_WRITE_SDATAC();
	wait(1000);
	EASI_ADS1294_CONFIG();
	
	wait(1);
	CONFIG_GPIO_INTERRUPTS();
	UART_txChars("Starting the ECG!\n\0");
	EASI_SET_START_SIGNAL;
	

	while(1)
	{	
		while(EASI_SAMPLE_FIFO_ISEMPTY(CHAN_1) == FALSE && EASI_SAMPLE_FIFO_ISEMPTY(CHAN_2) == FALSE
			&& EASI_SAMPLE_FIFO_ISEMPTY(CHAN_3) == FALSE && EASI_SAMPLE_FIFO_ISEMPTY(CHAN_4) == FALSE)
		{
			
			currentSample1 = 0;
			currentSample2 = 0;
			currentSample3 = 0;
			currentSample4 = 0;
			
			IRQ_disable(GPIO_EVENT);

#if USE_FIFO==1
	
			EASI_SAMPLE_FIFO_POP(CHAN_1,&currentSample1);
			EASI_SAMPLE_FIFO_POP(CHAN_2,&currentSample2);
			EASI_SAMPLE_FIFO_POP(CHAN_3,&currentSample3);
			EASI_SAMPLE_FIFO_POP(CHAN_4,&currentSample4);
			
#if ENABLE_FIR_FILTER == 1
			
			/*EASI_RUN_FILTERLOOP(&currentSample1,1,&currentSample1,1,1);
			EASI_RUN_FILTERLOOP(&currentSample2,1,&currentSample2,1,2);
			EASI_RUN_FILTERLOOP(&currentSample3,1,&currentSample3,1,3);
			EASI_RUN_FILTERLOOP(&currentSample4,1,&currentSample4,1,4);
			*/
#endif	
			currentSample1 = currentSample1-ch1Offset;
			currentSample2 = currentSample2-ch2Offset;
			currentSample3 = currentSample3-ch3Offset;
			currentSample4 = currentSample4-ch4Offset;
			
			sprintf((char*)uartBuf,"%li %li %li %li",currentSample1,currentSample2, currentSample3, currentSample4);
			UART_txChars((char*)uartBuf);
			for(i=0;i<UART_BUFSIZE;i++)
				uartBuf[i] = 0;
			
#endif
			
			if(startupComplete == FALSE)
			{
				if(curSec > 59)
				{
					curSec = getCurrentSecond();
					if( (59-curSec) < STARTUP_WAIT_TIME)
						targetSec = STARTUP_WAIT_TIME-(59-curSec)-1;
					else
						targetSec = curSec + STARTUP_WAIT_TIME;					
				}
				
				if(getCurrentSecond() == targetSec)
				{
					//Measure the offset voltages and switch to normal electrode input
					ch1Offset = currentSample1;
					ch2Offset = currentSample2;
					ch3Offset = currentSample3;
					ch4Offset = currentSample4;
					
					EASI_WRITE_SDATAC();
					wait(1000);
					EASI_ADS1294_CONFIG_CHANNELS();
					startupComplete = TRUE;
					printf("Startup Complete!\n");
				}
			}
			
			IRQ_enable(GPIO_EVENT);
			UART_txChars("\r\n");
			
		}
	}
}


void _wait(Uint32 delay)
{
	volatile Uint32 i;
	for(i = 0;i < delay;i++){};
}

void waitusec(Uint32 usec)
{
	_wait((Uint32)usec * 50);
}

void devicePllWorkaround()
{   
	int i;
	
    //Reset all of the peripherals
    CSL_SYSCTRL_REGS->PSRCR = 0x0020;
    CSL_SYSCTRL_REGS->PRCR = 0x00BF;
    while(CSL_SYSCTRL_REGS->PRCR != 0x0000); //Wait until the peripherals are out of reset
    
    //Enable clocks to all peripherals
    CSL_SYSCTRL_REGS->PCGCR1 = 0x0000;
    CSL_SYSCTRL_REGS->PCGCR2 = 0x0000;
    
    //Not sure why the following PLL configuration must be performed for the easiPll() function
    //To properly program the pll later...something to look into when time allows.
    //Bypass pll
    CSL_SYSCTRL_REGS->CCR2 = 0x0;
    
    //Set CLR_CNTL = 0
    CSL_SYSCTRL_REGS->CGCR1 &= 0x7fff;
    
    CSL_SYSCTRL_REGS->CGCR2 = 0x8000;
    CSL_SYSCTRL_REGS->CGCR4 = 0x0000;
    CSL_SYSCTRL_REGS->CGCR3 = 0x0806;
    CSL_SYSCTRL_REGS->CGCR1 = 0x82fa;
    
    //Wait for pll lock
    for(i=0;i<0x7fff;i++);
    
    //Switch to pll clk
    CSL_SYSCTRL_REGS->CCR2 = 0x1;
}

void DEVICE_INIT()
{
	devicePllWorkaround();
   	EASI_PLL_INIT();
	EASI_RTC_INIT();
	EASI_SAMPLE_FIFO_INIT();
	initInterruptBuffers(); 
	EASI_UART_INIT();
	//Change the multiplexing to work on the EzDSP hardware we have
	CSL_FINS(CSL_SYSCTRL_REGS->EBSR, SYS_EBSR_PPMODE, CSL_SYS_EBSR_PPMODE_MODE1);
   	EASI_SPI_INIT();
    EASI_UART_REINIT();  //Workaround for a bug in the CSL - otherwise spi setup causes UART to break
   	EASI_GPIO_INIT();
   	
}
