#include "easi_spi.h"

Uint16 spiWriteBuff[CSL_SPI_BUF_LEN];
Uint16 spiReadBuff[CSL_SPI_BUF_LEN];


CSL_SpiHandle _hSpi;
SPI_Config	hwConfig;

CSL_Status SPI_DATA_TRANSACTION(Uint16* buffer, Uint16 bufLen)
{
	CSL_Status result;
	result = SPI_dataTransaction(_hSpi,buffer,bufLen,SPI_WRITE);
	return result;
}

void SPI_WRITE_BUFFER(Uint16* writeBuffer, Uint16 bufLen)
{
	volatile Uint16 	bufIndex;
	Uint16 	spiStatusReg;
	volatile Uint16 	spiBusyStatus;
	volatile Uint16 	spiWcStaus;
	volatile Uint16     delay;

	bufIndex = 0;
	if(0 == bufLen)
	{
		return;
	}

	while(bufIndex < bufLen)
	{
	
		CSL_SPI_REGS->SPIDR2 = (Uint16)(writeBuffer[bufIndex] << 0x08);
		CSL_SPI_REGS->SPIDR1 = 0x0000;
		bufIndex++;
		
		// Set command for Writing to registers 
		CSL_FINS(CSL_SPI_REGS->SPICMD2, SPI_SPICMD2_CMD, SPI_WRITE_CMD);

		for(delay = 0; delay < 100; delay++);

		do
		{
			spiStatusReg = CSL_SPI_REGS->SPISTAT1;
			spiBusyStatus = (spiStatusReg & CSL_SPI_SPISTAT1_BSY_MASK);
			spiWcStaus = (spiStatusReg & CSL_SPI_SPISTAT1_CC_MASK);
		}
		while((spiBusyStatus == CSL_SPI_SPISTAT1_BSY_BUSY) && (spiWcStaus == CSL_SPI_SPISTAT1_CC_NOTCOMPLETE));
	}
}

//Writes in 24 bit words
void SPI_WRITE_BUFFER24(Uint32* writeBuffer, Uint16 bufLen)
{
	volatile Uint16 	bufIndex;
	Uint16 	spiStatusReg;
	volatile Uint16 	spiBusyStatus;
	volatile Uint16 	spiWcStaus;
	volatile Uint16     delay;

	bufIndex = 0;
	if(0 == bufLen)
	{
		return;
	}

	while(bufIndex < bufLen)
	{
		//Organize the data out regs so that the 24 bits are in the right position
		CSL_SPI_REGS->SPIDR2 = (Uint16)((writeBuffer[bufIndex] & 0x00ff0000) >> 8);
		CSL_SPI_REGS->SPIDR2 |= (Uint16)((writeBuffer[bufIndex] & 0x0000ff00) >> 8);
		CSL_SPI_REGS->SPIDR1 = (Uint16)((writeBuffer[bufIndex] & 0x000000ff) << 8);
		bufIndex++;
		
		// Set command for Writing to registers 
		CSL_FINS(CSL_SPI_REGS->SPICMD2, SPI_SPICMD2_CMD, SPI_WRITE_CMD);

		for(delay = 0; delay < 25; delay++);

		do
		{
			spiStatusReg = CSL_SPI_REGS->SPISTAT1;
			spiBusyStatus = (spiStatusReg & CSL_SPI_SPISTAT1_BSY_MASK);
			spiWcStaus = (spiStatusReg & CSL_SPI_SPISTAT1_CC_MASK);
		}
		while((spiBusyStatus == CSL_SPI_SPISTAT1_BSY_BUSY) && (spiWcStaus == CSL_SPI_SPISTAT1_CC_NOTCOMPLETE));
	}
}


CSL_Status ECG_SPI_OPEN()
{
	volatile Uint16 delay;
	ioport volatile CSL_SysRegs	*sysRegs;

	sysRegs = (CSL_SysRegs *)CSL_SYSCTRL_REGS;
	
	//Enable the SPI clock
	CSL_FINS(sysRegs->PCGCR1, SYS_PCGCR1_SPICG, CSL_SYS_PCGCR1_SPICG_ACTIVE);

	/* Value of 'Reset Counter' */
	CSL_FINS(sysRegs->PSRCR, SYS_PSRCR_COUNT, 0x20);

	CSL_FINS(sysRegs->PRCR, SYS_PRCR_PG4_RST, CSL_SYS_PRCR_PG4_RST_RST);

	for(delay = 0; delay < 100; delay++);
	
	return (CSL_SOK);
}

CSL_Status EASI_SPI_INIT()
{
	CSL_Status result;
	ioport volatile CSL_SysRegs	*sysRegs;

	sysRegs = (CSL_SysRegs *)CSL_SYSCTRL_REGS;
	
	
	result = ECG_SPI_OPEN();
	if(result != CSL_SOK)
		return (result);
	
	_hSpi = SPI_open(SPI_CS_NUM_1, SPI_POLLING_MODE);
	
	if(_hSpi == NULL)
	{
		_hSpi = SPI_open(SPI_CS_NUM_1, SPI_POLLING_MODE);
		if(_hSpi == NULL)
		{
			printf("Could not open SPI instance\n");
			return result;
		}
	}	
	else
		printf ("SPI Instance Opened successfully\n");
	
	/** Set the hardware configuration 							*/
	hwConfig.spiClkDiv	= SPI_CLK_DIV;
	hwConfig.wLen		= SPI_WORD_LENGTH_8;
	hwConfig.frLen		= SPI_FRAME_LENGTH;
	hwConfig.wcEnable	= SPI_WORD_IRQ_DISABLE;
	hwConfig.fcEnable	= SPI_FRAME_IRQ_DISABLE;
	hwConfig.csNum		= SPI_CS_NUM_1;
	hwConfig.dataDelay	= SPI_DATA_DLY_0;
	hwConfig.csPol		= SPI_CSP_ACTIVE_LOW;
	hwConfig.clkPol		= SPI_CLKP_LOW_AT_IDLE;
	hwConfig.clkPh		= SPI_CLK_PH_RISE_EDGE;
	
	result = SPI_config(_hSpi, &hwConfig);

	if(result != CSL_SOK)
	{
		return (result);
	}
	else
	{
		printf ("SPI Instance Configured successfully\n");
	}
	
	return (result);
}



void EASI_ADS1294_RREG(Uint8 registerAddress,Uint8 length, Uint16* readBuffer)
{
	Uint16 writeBuffer[3] = {0};
	Uint16 prevFrLen;
	int i = 0;
	
	//ADS1294 doesn't allow more than 32 registers to be read at once
	if(length > 0x32 || length < 1)
		return;
	
	//The RREG command requires three 8-bit characters to be sent in order to read one register
	prevFrLen = hwConfig.frLen;
	hwConfig.frLen = 3;
	SPI_config(_hSpi, &hwConfig);
	
	writeBuffer[0] = (ADS_1294_RREG & ADS_1294_RREG_MASK) | (registerAddress & 0x1f); //RREG command and starting address
	writeBuffer[1] = (length-1) & 0x1f; //(# of registers to read)-1
	writeBuffer[2] = 0x00; //Dummy value to give clocks
	
	//Clock out the first two bytes of the RREG command
	SPI_WRITE_BUFFER(writeBuffer, 2); 
	
	//Now clock in the response.  This isn't the most efficient way to do things since we have a 16 bit register to read from
	//And we're only using half of it, but it works since we're using 8 bit character lengths
	for(i=0; i<length; i++)
	{
		SPI_WRITE_BUFFER(&writeBuffer[2],1);
		readBuffer[i] = ((CSL_SPI_REGS->SPIDR1) & 0x00ff);	
	}
	
	hwConfig.frLen = prevFrLen;
	SPI_config(_hSpi, &hwConfig);
}

//Issues enough clocks to read one data capture from the ADS1294
void EASI_CLK_DOUT(Uint8* readBuffer,Uint8 size)
{
	Uint32 writeBuffer = 0x00;
	Uint8 i = 0;
	Uint16 prevFrLen = 0;
	SPI_WordLen prevWordLen = SPI_WORD_LENGTH_1;
	if(size < 15)
	{
		printf("Buffer size too small when issuing SCLKs for data.\n");
		return;
	}
	
	prevFrLen = hwConfig.frLen;
	prevWordLen = hwConfig.wLen;
	hwConfig.frLen = 5;
	hwConfig.wLen = SPI_WORD_LENGTH_24;
	hwConfig.spiClkDiv = FAST_SPI_CLK_DIV;
	SPI_config(_hSpi, &hwConfig);
	
	for(i=0; i<5; i++)
	{
		SPI_WRITE_BUFFER24(&writeBuffer,1);
		readBuffer[3*i] = ((CSL_SPI_REGS->SPIDR2) & 0x00ff);
		readBuffer[3*i+1] = (((CSL_SPI_REGS->SPIDR1) & 0xff00) >> 8);
		readBuffer[3*i+2] = ((CSL_SPI_REGS->SPIDR1) & 0x00ff);
	}
	
	hwConfig.frLen = prevFrLen;
	hwConfig.wLen = prevWordLen;
	hwConfig.spiClkDiv = SPI_CLK_DIV;
	SPI_config(_hSpi,&hwConfig);
}

void EASI_WRITE_RDATA(Uint8* readBuffer,Uint8 size)
{
	Uint16 writeBuffer = 0x12; //RDATA opcode
	
	if(size < 15) 
	{
		printf("Buffer size too small when issuing RDATA command.\n");
		return;
	}
	
	SPI_WRITE_BUFFER(&writeBuffer,1); //Write the RDATA opcode
	EASI_CLK_DOUT(readBuffer, size);
}

void EASI_ADS1294_WREG(Uint8 registerAddress,Uint8 length, Uint16* writeBuffer)
{
	Uint16 writeCmd[2] = {0};
	Uint16 prevFrLen;
	int i = 0;
	
	//ADS1294 doesn't allow more than 32 registers to be written at once
	if(length > 0x32 || length < 1)
		return;
	
	//The RREG command requires three 8-bit characters to be sent in order to read one register
	prevFrLen = hwConfig.frLen;
	hwConfig.frLen = 2 + length;
	SPI_config(_hSpi, &hwConfig);
	
	writeCmd[0] = (ADS_1294_WREG & ADS_1294_WREG_MASK) | (registerAddress & 0x1f); //RREG command and starting address
	writeCmd[1] = (length-1) & 0x1f; //(# of registers to write)-1
	
	
	//Clock out the first two bytes of the WREG command
	SPI_WRITE_BUFFER(writeCmd, 2); 
	
	//Now clock out the data to write. 
	for(i=0; i<length; i++)
	{
		SPI_WRITE_BUFFER(&writeBuffer[i],1);	
	}
	
	hwConfig.frLen = prevFrLen;
	SPI_config(_hSpi, &hwConfig);
	
}








