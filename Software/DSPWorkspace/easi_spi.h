#ifndef EASI_SPI_H_
#define EASI_SPI_H_
#include "stdio.h"
#include "csl_general.h"
#include "csl_spi.h"

#define	CSL_SPI_BUF_LEN			(64)
#define	SPI_CLK_DIV				(25)
#define	SPI_FRAME_LENGTH		(1)

CSL_Status easiSpiInit();

void easiReadAdsId();


#endif /*EASI_SPI_H_*/
