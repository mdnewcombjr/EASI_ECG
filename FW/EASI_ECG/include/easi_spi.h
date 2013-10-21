#ifndef EASI_SPI_H_
#define EASI_SPI_H_
#include "stdio.h"
#include "csl_general.h"
#include "easi_gpio.h"
#include "csl_spi.h"
#include "easi_hardware.h"

#define	CSL_SPI_BUF_LEN			(64)

#if SYS_CLKSPEED==100
	#define SPI_CLK_DIV			(400)
	#define FAST_SPI_CLK_DIV    (40)
#else
	#define	SPI_CLK_DIV				(200) 
	#define FAST_SPI_CLK_DIV 		(20) //Note: max SCLK speed is 20 MHz
#endif

#define	SPI_FRAME_LENGTH		(1)
#define SYS_CLK_SPEED 50003968

#define ADS_1294_RREG 0x20 //OR this with the proper value to read specific registers
#define ADS_1294_RREG_MASK 0xE0

#define ADS_1294_WREG 0x40 //OR this with the proper value to write specific registers
#define ADS_1294_WREG_MASK 0xE0

CSL_Status EASI_SPI_INIT();
CSL_Status SPI_DATA_TRANSACTION(Uint16* buffer, Uint16 bufLen);

void EASI_ADS1294_RREG(Uint8 registerAddress,Uint8 length, Uint16* readBuffer);
void EASI_ADS1294_WREG(Uint8 registerAddress,Uint8 length, Uint16* writeBuffer);
void EASI_WRITE_RDATA(Uint8* readBuffer,Uint8 size);
void EASI_CLK_DOUT(Uint8* readBuffer,Uint8 size);

#endif /*EASI_SPI_H_*/
