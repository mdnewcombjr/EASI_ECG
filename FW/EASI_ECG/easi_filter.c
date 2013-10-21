#include "easi_filter.h"
#include "LP_Filter.h"
#include "HP_Filter.h"
 
/*
//#pragma DATA_SECTION(lp_delay_buffer1,".dbuffer")
static DATA lp_delay_buffer1[LP_DBSIZE] = {0};

//#pragma DATA_SECTION(lp_delay_buffer2,".dbuffer")
static DATA lp_delay_buffer2[LP_DBSIZE] = {0};

//#pragma DATA_SECTION(lp_delay_buffer3,".dbuffer")
static DATA lp_delay_buffer3[LP_DBSIZE] = {0};

//#pragma DATA_SECTION(lp_delay_buffer4,".dbuffer")
static DATA lp_delay_buffer4[LP_DBSIZE] = {0};

static DATA hp_delay_buffer1[HP_DBSIZE] = {0};
static DATA hp_delay_buffer2[HP_DBSIZE] = {0};
static DATA hp_delay_buffer3[HP_DBSIZE] = {0};
static DATA hp_delay_buffer4[HP_DBSIZE] = {0};
*/

static Int32 lp_delay_buffer1[LP_DBSIZE] = {0};
static Int32 lp_delay_buffer2[LP_DBSIZE] = {0};
static Int32 lp_delay_buffer3[LP_DBSIZE] = {0};
static Int32 lp_delay_buffer4[LP_DBSIZE] = {0};

#pragma DATA_ALIGN(sample,2);
DATA sample = 0;

#pragma DATA_ALIGN(sample_,2);
Int32 sample_ = 0;
//DATA is typedef'd as Int16 (short)


//This is a custom filter to use 24 bit input data and 10-bit filter coefficients
Int32 fir_(Int32* insample,Int32* dbuff, DATA* coeffs, Uint16 coeffsize)
{
	static Uint8 count = 0;
	static int index = 0;
	Uint16 i = 0;
	Int32 result = 0;
	
	dbuff[count] = *insample;
	
	for(i=0; i<coeffsize; i++)
	{
		result += coeffs[i] * dbuff[index--];
		if(index < 0) index = coeffsize-1;
	}
	
	if(++count >= coeffsize) count = 0;
	
	return result;
}
		
	

void EASI_RUN_FILTERLOOP(Int32* inbuf, short sizein, Int32* outbuf, short sizeout, short channel)
{
	ushort oflag = 0;
	
	
	//Currently only supports in-place computation
	if (sizein != 1)
		return;
		
	//sample = (DATA)((inbuf[0]/256)&0xFFFF); //convert to Q0.15 format
	sample_ = inbuf[0];
	
	switch(channel)
	{
		case 1:
			oflag = fir_(&sample_,lp_delay_buffer1,LP_Coeffs,LPSIZE);
			//oflag = fir(&sample,LP_Coeffs,&sample,lp_delay_buffer1,1,LPSIZE);
			//oflag = fir(&sample,HP_Coeffs,&sample,hp_delay_buffer1,1,HPSIZE);
			break;
		case 2:
			oflag = fir_(&sample_,lp_delay_buffer2,LP_Coeffs,LPSIZE);
			//oflag = fir(&sample,LP_Coeffs,&sample,lp_delay_buffer2,1,LPSIZE);
			//oflag = fir(&sample,HP_Coeffs,&sample,hp_delay_buffer2,1,HPSIZE);
			break;
		case 3:
			oflag = fir_(&sample_,lp_delay_buffer3,LP_Coeffs,LPSIZE);
			//oflag = fir(&sample,LP_Coeffs,&sample,lp_delay_buffer3,1,LPSIZE);
			//oflag = fir(&sample,HP_Coeffs,&sample,hp_delay_buffer3,1,HPSIZE);
			break;
		case 4:
			oflag = fir_(&sample_,lp_delay_buffer4,LP_Coeffs,LPSIZE);
			//oflag = fir(&sample,LP_Coeffs,&sample,lp_delay_buffer4,1,LPSIZE);
			//oflag = fir(&sample,HP_Coeffs,&sample,hp_delay_buffer4,1,HPSIZE);
			break;
		default:
			return;
			break;
	}
	
	outbuf[0] = (Int32)(0xFFFF & (sample));
	if( ((sample >> 15)&0x1) > 0)
		outbuf[0] |= 0xFFFF0000;
}

