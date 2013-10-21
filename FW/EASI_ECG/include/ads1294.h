#ifndef ADS1294_H_
#define ADS1294_H_

#include "easi_spi.h"
#include "easi_gpio.h"
#include "easi_hardware.h"

#define CALIBRATION_SIGNAL 0
#define RLD_ENABLED 1

//CONFIG1 = 0xA6
//	High-resolution mode
//	Daisy-chain mode
//	Oscillator clock output enabled
//	500 SPS output
//CONFIG2 = 0x10
//	Chopping freq varies
//	Test signals generated internally
//	Test signal amplitude = 1mV
//	Test signal pulsed at fclk/2^21 ~= 0.97 Hz
 
#if CALIBRATION_SIGNAL==1
	#define CONFIG1_REG (0x80|ADS1294_DATA_RATE)
	#define CONFIG2_REG 0x10
	#define CONFIG3_REG 0xC0
	#define ELECTRODE_REG 0x05 
#else
	#define CONFIG1_REG (0x80|ADS1294_DATA_RATE)
	#define CONFIG2_REG 0x00
	
	#if RLD_ENABLED==1
		#define CONFIG3_REG 0xCE
		#define RLD_SENSEP 0x0F
		#define RLD_SENSEN 0x0F
	#else
		#define CONFIG3_REG 0xC0
	#endif
	
	#define ELECTRODE_REG 0x00 //0x10 -> PGA_Gain = 1, 0x00 -> PGA_Gain = 6
#endif

#define ELECTRODE_SHORT_REG 0x01 //Used to measure the offset on startup



interrupt void EASI_DRDY_ISR(void);

CSL_Status EASI_WRITE_SDATAC();
CSL_Status EASI_WRITE_RDATAC();
void EASI_ADS1294_CONFIG();

extern Uint16 tempct;


#endif /*ADS1294_H_*/
