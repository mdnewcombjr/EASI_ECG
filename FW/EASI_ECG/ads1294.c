#include "ads1294.h"

Uint16 adsSpiWriteBuff[CSL_SPI_BUF_LEN];
Uint16 adsSpiReadBuff[CSL_SPI_BUF_LEN];

		

void EASI_ADS1294_CONFIG()
{
	int i = 0;
	Uint16 buffer[2] = {0};
	
	//Write 0xC0 to CONFIG3 (Enable the internal reference buffer)
	buffer[0] = CONFIG3_REG;
	EASI_ADS1294_WREG(0x03,1,buffer);
	

	buffer[0] = CONFIG1_REG;
	buffer[1] = CONFIG2_REG;

	EASI_ADS1294_WREG(0x01,2,buffer);
	
	buffer[1] = ELECTRODE_SHORT_REG;

	//Set Registers 5-13 to 0x01 (short the inputs as a test) or 0x05(test signal)
	for(i=0; i<4; i++)
	{
		EASI_ADS1294_WREG((5+i),1,&buffer[1]);
	}
#if (RLD_ENABLED==1 && CALIBRATION_SIGNAL==0)
	buffer[0] = RLD_SENSEP;
	buffer[1] = RLD_SENSEN;
	
	EASI_ADS1294_WREG(0x0D,2,&buffer[0]);
#endif
	//Read the ID register.  Should be 0x90 for the ADS1294
	EASI_ADS1294_RREG(0x00,1,buffer);
	printf("Read a device frontend ID of 0x%x.\n",((buffer[0])& 0x00ff));
	
	//Read the CONFIG1,2,3, and electrode registers and print their values
	EASI_ADS1294_RREG(0x01,1,buffer);
	printf("CONFIG1: %#x\n",((buffer[0])&0x00ff));
	EASI_ADS1294_RREG(0x02,1,buffer);
	printf("CONFIG2: %#x\n",((buffer[0])&0x00ff));
	EASI_ADS1294_RREG(0x03,1,buffer);
	printf("CONFIG3: %#x\n",((buffer[0])&0x00ff));
	
#if (RLD_ENABLED==1 && CALIBRATION_SIGNAL==0)
	EASI_ADS1294_RREG(0x0D,1,buffer);
	printf("RLD_SENSEP: %#x\n",((buffer[0])&0x00ff));
	EASI_ADS1294_RREG(0x0E,1,buffer);
	printf("RLD_SENSEN: %#x\n",((buffer[0])&0x00ff));
#endif
	//*********Setup the interrupt for #DRDY**************
	
	
	//Now set the START signal high on the ADS1294
	setPin(ADS1294_START,1);
	
	EASI_WRITE_RDATAC();
	
}

void EASI_ADS1294_CONFIG_CHANNELS()
{
	short i = 0;
	Uint16 buffer = ELECTRODE_REG;
	
	for(i=0; i<4; i++)
	{
		EASI_ADS1294_WREG((5+i),1,&buffer);
	}
	
	EASI_ADS1294_RREG(0x05,1,&buffer);
	printf("Ch1SET: %#x\n",(buffer & 0x00ff));
	EASI_ADS1294_RREG(0x06,1,&buffer);
	printf("Ch2SET: %#x\n",(buffer & 0x00ff));
	EASI_ADS1294_RREG(0x07,1,&buffer);
	printf("Ch3SET: %#x\n",(buffer & 0x00ff));
	EASI_ADS1294_RREG(0x08,1,&buffer);
	printf("Ch4SET: %#x\n",(buffer & 0x00ff));
	
	//Now set the START signal high on the ADS1294
	setPin(ADS1294_START,1);
	
	EASI_WRITE_RDATAC();
}

CSL_Status EASI_WRITE_SDATAC()
{
	CSL_Status status;
	
	adsSpiWriteBuff[0] = 0x11;
	
	status = SPI_DATA_TRANSACTION(adsSpiWriteBuff,1);
	if(status != CSL_SOK)
	{
		printf("Could not send SDATAC command to ADS1294.\n");
		return status;
	}

	return status;
}

CSL_Status EASI_WRITE_RDATAC()
{
	CSL_Status status;
	
	adsSpiWriteBuff[0] = 0x10;
	
	status = SPI_DATA_TRANSACTION(adsSpiWriteBuff,1);
	if(status != CSL_SOK)
	{
		printf("Could not send RDATAC command to ADS1294.\n");
		return status;
	}

	return status;
}
