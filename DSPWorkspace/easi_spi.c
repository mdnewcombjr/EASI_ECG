#include "easi_spi.h"

Uint16 spiWriteBuff[CSL_SPI_BUF_LEN];
Uint16 spiReadBuff[CSL_SPI_BUF_LEN];
Uint16 byteBuf[1];
Uint16 cmdBuffer[4] = {0, 0, 0, 0};

CSL_SpiHandle hSpi;
SPI_Config	hwConfig;

CSL_Status easiSpiInit()
{
	CSL_Status result;
	
	result = SPI_init();
	if(result != CSL_SOK)
	{
		return (result);
	}
	else
	{
		printf ("SPI Instance Initialize successfully\n");
	}
	
	hSpi = SPI_open(SPI_CS_NUM_1, SPI_POLLING_MODE);
	if(hSpi == NULL)
	{
		return (CSL_ESYS_FAIL);
	}
	else
	{
		printf ("SPI Instance Opened successfully\n");
	}
	
	/** Set the hardware configuration 							*/
	hwConfig.spiClkDiv	= SPI_CLK_DIV;
	hwConfig.wLen		= SPI_WORD_LENGTH_8;
	hwConfig.frLen		= SPI_FRAME_LENGTH;
	hwConfig.wcEnable	= SPI_WORD_IRQ_ENABLE;
	hwConfig.fcEnable	= SPI_FRAME_IRQ_DISABLE;
	hwConfig.csNum		= SPI_CS_NUM_0;
	hwConfig.dataDelay	= SPI_DATA_DLY_0;
	hwConfig.csPol		= SPI_CSP_ACTIVE_LOW;
	hwConfig.clkPol		= SPI_CLKP_LOW_AT_IDLE;
	hwConfig.clkPh		= SPI_CLK_PH_FALL_EDGE;
	
	result = SPI_config(hSpi, &hwConfig);

	if(result != CSL_SOK)
	{
		return (CSL_ESYS_FAIL);
	}
	else
	{
		printf ("SPI Instance Configured successfully\n");
	}
	return (CSL_SOK);
}

void waitForCCBSY()
{
	volatile Uint16 value = 0;
	//Wait for the character transfer to complete
    do 
    {
    	value=CSL_FEXT(CSL_SPI_REGS->SPISTAT1,SPI_SPISTAT1_CC);
    }while((value & 0x01) == 0x0);
    
    //Wait for the BUSY flag to indicate that no chars are being transferred
    do { 
        value=CSL_FEXT(CSL_SPI_REGS->SPISTAT1,SPI_SPISTAT1_BSY);
    }while((value & 0x01) == 0x01);
}
	

void easiReadAdsId()
{
	volatile Uint16 value = 0;
	//Spin while the spi interface is busy
	do 
	{
    	value=CSL_FEXT(CSL_SPI_REGS->SPISTAT1,SPI_SPISTAT1_BSY);
    }while((value & 0x01) == 0x01);
    
    //Configure the C5505 SPI interface for a 3 character transfer
    CSL_FINS(CSL_SPI_REGS->SPICMD1,SPI_SPICMD1_FLEN,2); 
    
    //Set the data out to be the RREG command read Reg 00h    
    CSL_FINS(CSL_SPI_REGS->SPIDR2,SPI_SPIDR2_DATA,0x0200);
    CSL_FINS(CSL_SPI_REGS->SPIDR1,SPI_SPIDR1_DATA,0x0);
    
    //Initiate the start of a write
    CSL_FINS(CSL_SPI_REGS->SPICMD2,SPI_SPICMD2_CMD,CSL_SPI_SPICMD2_CMD_WRITE);
    
	waitForCCBSY();
	
	//Write the second byte, indicating we only want to read 1 register
	CSL_FINS(CSL_SPI_REGS->SPIDR2,SPI_SPIDR2_DATA,0x0000);
	CSL_FINS(CSL_SPI_REGS->SPIDR1,SPI_SPIDR1_DATA,0x0);
	//Initiate the start of a write
    CSL_FINS(CSL_SPI_REGS->SPICMD2,SPI_SPICMD2_CMD,CSL_SPI_SPICMD2_CMD_WRITE);
    waitForCCBSY();
    
    //Prep the data registers to receive 1 byte from the ADS1294
    CSL_FINS(CSL_SPI_REGS->SPIDR2,SPI_SPIDR2_DATA,0x0000);
	CSL_FINS(CSL_SPI_REGS->SPIDR1,SPI_SPIDR1_DATA,0x0000);
   	//Initiate the start of a read
    CSL_FINS(CSL_SPI_REGS->SPICMD2,SPI_SPICMD2_CMD,CSL_SPI_SPICMD2_CMD_WRITE);
    waitForCCBSY();
    
    value = CSL_FEXT(CSL_SPI_REGS->SPIDR1,SPI_SPIDR1_DATA);
    printf("Read a device ID of 0x%X\n",value);
}


