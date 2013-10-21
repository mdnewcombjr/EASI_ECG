#include "stdio.h"
#include "csl_general.h"
#include "csl_intc.h"
#include "cslr_sysctrl.h"
#include "csl_pll.h"
#include "csl_pllAux.h"
#include "cslr.h"
#include "easi_uart.h"
#include "easi_spi.h"

//The frequency of the crystal osc input to the DSP (in Hz)
#define ezDSP_PLL_CLOCKIN    (32768u)

PLL_Config pllCfg_12p288MHz = {0x8173, 0x8000, 0x0806, 0x0000};
PLL_Config pllCfg_40MHz     = {0x8988, 0x8000, 0x0806, 0x0201};
PLL_Config pllCfg_60MHz     = {0x8724, 0x8000, 0x0806, 0x0000};
PLL_Config pllCfg_75MHz     = {0x88ED, 0x8000, 0x0806, 0x0000};
PLL_Config pllCfg_100MHz    = {0x8BE8, 0x8000, 0x0806, 0x0000};
PLL_Config pllCfg_120MHz    = {0x8E4A, 0x8000, 0x0806, 0x0000};




CSL_Status ezDSP_SetupClocks()
{
	CSL_Status status;
	PLL_Obj pllObj;
	PLL_Handle hPLL;
	PLL_Config* pllConfigInfo;
	
	//Initialize the PLL
	status = PLL_init(&pllObj, CSL_PLL_INST_0);
	if(status != CSL_SOK)
	{
		printf("Failed to init the PLL\n");
		return status;
	}
	
	hPLL = (PLL_Handle)(&pllObj);
	PLL_reset(hPLL);
	
	pllConfigInfo = &pllCfg_100MHz;
	status = PLL_config(hPLL,pllConfigInfo);
	if(status != CSL_SOK)
	{
		printf("Failed to config the PLL\n");
		return (status);
	}
	
	
	return (status);
	
}

void wait(Uint32 delay)
{
	volatile Uint32 i;
	for(i = 0;i < delay;i++){};
}

void waitusec(Uint32 usec)
{
	wait((Uint32)usec * 8);
}

void main( void )
{
	int j = 0;
	asm(" BCLR XF");
	easiSpiInit();
	
	easiReadAdsId();
	//ezDSP_SetupClocks();
	//easiUartInit();
	//printf("Initialized UART\n");
	while(1)
	{
		if(j == 0)
			asm(" BCLR XF");
		else
			asm(" BSET XF");
			
	    //UART_txChars("This is a test\n\0");
	    //printf("Send test UART string\n");
	    j ^= 1;
	    
	    easiReadAdsId();
	    
	   waitusec(500000);
	}   
}
