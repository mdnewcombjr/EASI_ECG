#include "easi_rtc.h"

void EASI_RTC_INIT()
{
	CSL_RtcConfig rtcConfig;
	CSL_Status status;
	
	//Put in some initial values for the 'date'
	rtcConfig.rtcyear = 13; 
	rtcConfig.rtcmonth = 2;
	rtcConfig.rtcday = 12;
	rtcConfig.rtchour = 0;
	rtcConfig.rtcmin = 0;
	rtcConfig.rtcsec = 0;
	rtcConfig.rtcmSec = 0;
	
	rtcConfig.rtcyeara = 12;
	rtcConfig.rtcmontha = 12;
	rtcConfig.rtcdaya = 12;
	rtcConfig.rtchoura = 12;
	rtcConfig.rtcmina = 12;
	rtcConfig.rtcseca = 12;
	rtcConfig.rtcmSeca = 12;
	
	rtcConfig.rtcintcr = 0x0000;  //Disable all the RTC interrupts
	
	
	//RTC_start();
	status = RTC_config(&rtcConfig);
	if(status != CSL_SOK)
	{
		printf("Couldn't configure the RTC!\n");
		return;
	}
}

Uint16 getCurrentSecond()
{
	CSL_RtcTime theTime;
	CSL_Status status;
	Uint16 secValue = 0;
	
	status = RTC_getTime(&theTime);
	if(status != CSL_SOK)
	{
		printf("Had a problem in getCurrentSecond()!\n");
		return 0xFFFF;
	}
	
	return theTime.secs;
}
//Function that waits one millisecond
//TODO:Find a good way to disable all interrupts in this function
void waitmSec()
{
	CSL_RtcTime theTime;
	CSL_Status status;
	Uint16 mSecValue = 0, nextVal = 0;
	
	status = RTC_getTime(&theTime);
	if(status != CSL_SOK)
	{
		printf("Had a problem in waitmSec()!\n");
		return;
	}
	
	mSecValue = theTime.mSecs;
	if(mSecValue == 1023)
	{
		nextVal = 0;
	}
	else
	{
		nextVal = mSecValue+1;
	}
	
	do
	{
		status = RTC_getTime(&theTime);
		if(status != CSL_SOK)
		{
			printf("Had a problem in waitmSec()!\n");
			return;
		}
	}while(theTime.mSecs != nextVal);
	
}

void wait(Uint16 mSecDelay)
{
	Uint16 _delay = mSecDelay;
	while(_delay > 0)
	{
		waitmSec();
		_delay--;
	}
}

