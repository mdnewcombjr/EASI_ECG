#ifndef EASI_HARDWARE_H_
#define EASI_HARDWARE_H_
#include "csl_general.h"
#include "csl_gpio.h"

#define ADS1294_START 			CSL_GPIO_PIN15
#define ADS1294_ADC_CLK_SEL 	CSL_GPIO_PIN14
#define ADS1294_DRDY 			CSL_GPIO_PIN16
#define ADS1294_RESET 			CSL_GPIO_PIN17

#define EASI_SET_START_SIGNAL (((CSL_GpioRegsOvly)0x1c00)->IOOUTDATA1 |= (0x8000))

#define EASI_CLEAR_START_SIGNAL (((CSL_GpioRegsOvly)0x1c00)->IOOUTDATA1 &= ~(0x8000))

#define ADS1294_BITS_PER_NV 286

#define SYS_CLKSPEED 100 //For 100 Mhz

#define ADS1294_DATA_RATE 0x6 //See CONFIG1:DR[2:0] of the ADS1294.  0x4 == 2000 SPS



#endif /*EASI_HARDWARE_H_*/
