#include "interrupt_handlers.h"

fbuff avgBufs[4]; //Ch1MvgAvgBuf, Ch2MvgAvgBuf, Ch3MvgAvgBuf, Ch4MvgAvgBuf;

Uint32 Ch1Buf[MOVING_AVG_LENGTH] = {0};
Uint32 Ch2Buf[MOVING_AVG_LENGTH] = {0};
Uint32 Ch3Buf[MOVING_AVG_LENGTH] = {0};
Uint32 Ch4Buf[MOVING_AVG_LENGTH] = {0};

void initInterruptBuffers()
{
	fbuff_initialize(&avgBufs[0],MOVING_AVG_LENGTH,Ch1Buf);
	fbuff_initialize(&avgBufs[1],MOVING_AVG_LENGTH,Ch2Buf);
	fbuff_initialize(&avgBufs[2],MOVING_AVG_LENGTH,Ch3Buf);
	fbuff_initialize(&avgBufs[3],MOVING_AVG_LENGTH,Ch4Buf);
}

interrupt void ads1294_GPIO_ISR()
{
	Uint32 temp;
	Int32 currentReading = 0;
	static Uint8 mvgAvgCnt = 0;
	static Uint8 dataBuf[15] = {0};
	Uint8 i = 0, j = 0;
#if TX_ADS1294_STATUS==1 || USE_FIFO==0
	Uint8 uartBuf[16] = {0};
#endif

	//Check the GPIO interrupt flag register for GPIO16
	if((CSL_GPIO_REGS->IOINTFLG2 & 0x0001) > 0)
	{
		IRQ_disable(GPIO_EVENT);
		//Read from the ADS1294
		EASI_CLK_DOUT(dataBuf,15);

		//The following two lines tx the status bits from the ADS1294
#if TX_ADS1294_STATUS
		sprintf(uartBuf,"%#.2x%.2x%.2x ",dataBuf[0],dataBuf[1],dataBuf[2]);
		UART_txChars(uartBuf);
#endif
		
		
		for(i=1;i<5;i++)
    	{
#if INT_FILTER_DATA==0

    		
    		//currentReading = (currentReading<<8);
    		//currentReading /= 256;

	#if USE_FIFO==1
    		
    		EASI_SAMPLE_FIFO_PUSH_8BIT((FIFO_CHANNEL)(i-1),dataBuf[3*i],dataBuf[3*i+1],dataBuf[3*i+2]);
    	}
    	
	#else
			currentReading = 0;
			currentReading |= (Int32)(((Uint32)dataBuf[3*i] << 16) & 0x00FF0000);
    		currentReading |= (Int32)(((Uint32)dataBuf[3*i+1] << 8) & 0x0000FF00);
    		currentReading |= (Int32)(((Uint32)dataBuf[3*i+2]) & 0x000000FF);
    		
    		//Convert the 24 bit 2's complement number to a 32 bit 2's complement
    		if( ((currentReading >> 23)&0x1) > 0)
    		{
    			currentReading |= 0xFF000000;
    		} 
    		
    		sprintf(uartBuf,"%li ",currentReading);
    		UART_txChars(uartBuf);
    	}
    	UART_txChars("\r\n");
    #endif
#else
    		currentReading = 0;
    		
    		currentReading |= (Int32)(((Uint32)dataBuf[3*i] << 16) & 0x00FF0000);
    		currentReading |= (Int32)(((Uint32)dataBuf[3*i+1] << 8) & 0x0000FF00);
    		currentReading |= (Int32)(((Uint32)dataBuf[3*i+2]) & 0x000000FF);
    		
    		avgBufs[i-1].push( &(avgBufs[i-1]) , (Uint32)currentReading);

    	}
    	
    	//Update the moving average count, then perform the average and add sample to the FIFO if necessary
    	
    	mvgAvgCnt += 1;
    	
    	if(mvgAvgCnt >= MOVING_AVG_LENGTH)
    	{
    		for(i=0;i<4;i++)
    		{
    			//Reuse the currentReading variable as a temporary sum buffer
    			currentReading = 0;
    			
    			for(j=0;j<MOVING_AVG_LENGTH;j++)
    			{
    				avgBufs[i].peekat( &(avgBufs[i]) , (Uint16)j , &temp ); 
    				currentReading += (Int32)temp;
    			}
    			
    			currentReading /= MOVING_AVG_LENGTH;
#if USE_FIFO==1
    			EASI_SAMPLE_FIFO_PUSH((FIFO_CHANNEL)i,(Uint32)currentReading);
#else
#endif
    		}
    		
    		mvgAvgCnt = 0;
    	}
    	
#endif	
	    	
		CSL_GPIO_REGS->IOINTFLG1 = 0xFFFF;
		CSL_GPIO_REGS->IOINTFLG2 = 0xFFFF; //Write to clear all GPIO interrupts
		IRQ_enable(GPIO_EVENT);
	}
	
}	
