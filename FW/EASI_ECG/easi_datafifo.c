#include "easi_datafifo.h"

fifo sample_fifo1,sample_fifo2, sample_fifo3, sample_fifo4;
fifo* pChannels[4] = {0};

Uint8 sample_buffer[DATAFIFO_SIZE*NUM_CHANNELS] = {0};

void EASI_SAMPLE_FIFO_INIT()
{
	fifo_initialize(&sample_fifo1,DATAFIFO_SIZE,sample_buffer);
	fifo_initialize(&sample_fifo2,DATAFIFO_SIZE,&sample_buffer[DATAFIFO_SIZE]);
	fifo_initialize(&sample_fifo3,DATAFIFO_SIZE,&sample_buffer[(2*DATAFIFO_SIZE)]);
	fifo_initialize(&sample_fifo4,DATAFIFO_SIZE,&sample_buffer[(3*DATAFIFO_SIZE)]);
	
	pChannels[0] = &sample_fifo1;
	pChannels[1] = &sample_fifo2;
	pChannels[2] = &sample_fifo3;
	pChannels[3] = &sample_fifo4;
}

Bool EASI_SAMPLE_FIFO_ISEMPTY(FIFO_CHANNEL chan)
{
	return (pChannels[chan]->flags.isEmpty);
}

Bool EASI_SAMPLE_FIFO_PUSH_8BIT(FIFO_CHANNEL chan,Uint8 msb, Uint8 mb, Uint8 lsb)
{
	Bool retval = FALSE;
	//Put data in the fifo MSB first
	retval = pChannels[chan]->push(pChannels[chan],msb);
	if(retval == FALSE)
		//The buffer is full
		return FALSE;

	retval = pChannels[chan]->push(pChannels[chan],mb);
	if(retval == FALSE)
		return FALSE;
	
	pChannels[chan]->push(pChannels[chan],lsb);
	if(retval == FALSE)
		return FALSE;
		
	return TRUE;
}

Bool EASI_SAMPLE_FIFO_PUSH(FIFO_CHANNEL chan, Uint32 sample)
{
	Bool retval = FALSE;
	Uint8 tempval = 0;
	
	//Put data in the fifo MSB first
	tempval = (Uint8)((sample & 0x00FF0000) >> 16);
	retval = pChannels[chan]->push(pChannels[chan],tempval);
	if(retval == FALSE)
		//The buffer is full
		return FALSE;

	tempval = (Uint8)((sample & 0x0000FF00) >> 8);
	retval = pChannels[chan]->push(pChannels[chan],tempval);
	if(retval == FALSE)
		return FALSE;
	
	tempval = (Uint8)(sample & 0x000000FF);
	pChannels[chan]->push(pChannels[chan],tempval);
	if(retval == FALSE)
		return FALSE;
		
	
	return TRUE;
}

Bool EASI_SAMPLE_FIFO_POP(FIFO_CHANNEL chan, Int32 *sample)
{
		Bool retval = FALSE;
		Uint8 chunk;
		
		retval = pChannels[chan]->pop(pChannels[chan],&chunk);
		if(retval == FALSE)
			return FALSE;
		*sample |= (Int32)(((Uint32)chunk << 16) & 0x00FF0000);
		
		retval = pChannels[chan]->pop(pChannels[chan],&chunk);
		if(retval == FALSE)
			return FALSE;
		*sample |= (Int32)(((Uint32)chunk << 8) & 0x0000FF00);
		
		retval = pChannels[chan]->pop(pChannels[chan],&chunk);
		if(retval == FALSE)
			return FALSE;
		*sample |= (Int32)((Uint32)chunk & 0x000000FF);
		
		//Change the 24 bit 2's complement number into a 32 bit two's complement number
		if( (((*sample) >> 23)&0x1) > 0)
		{
			*sample |= 0xFF000000;
		}
		
		*sample = ((*sample) << 8);
		*sample = ((*sample)/256);
		
		return TRUE;
}

