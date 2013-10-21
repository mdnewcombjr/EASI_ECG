#ifndef EASI_PLL_H_
#define EASI_PLL_H_

#include "soc.h"
#include "easi_hardware.h"

#define PLLCNTL1_50MHz  (0x0be8) //Set M = 0xbe8 (3048d) and PLLPWRDN = 0 
#define PLLCNTL1_100MHz (0x0be8) //Set M = 0xbe8 (3048d) and PLLPWRDN = 0 

#define PLLCNTL2  (0x0806) //Always use this value

#define PLLINCNTL_50MHz (0x8000) //RDBYPASS = 1 and RDRATION = 0
#define PLLINCNTL_100MHz (0x8000) //RDBYPASS = 1 and RDRATION = 0

#define PLLOUTCNTL_50MHz 0x0201 //OUTDIVEN = 1, ODRATIO = 2
#define PLLOUTCNTL_100MHz 0x0000 //OUTDIVEN = 0, ODRATIO = 1


#if SYS_CLKSPEED==100
	#define PLLCNTL1 PLLCNTL1_100MHz
	#define PLLINCNTL PLLINCNTL_100MHz
	#define PLLOUTCNTL PLLOUTCNTL_100MHz
#else
	#define PLLCNTL1 PLLCNTL1_50MHz
	#define PLLINCNTL PLLINCNTL_50MHz
	#define PLLOUTCNTL PLLOUTCNTL_50MHz
#endif

Uint8 EASI_PLL_INIT();


#endif /*EASI_PLL_H_*/
