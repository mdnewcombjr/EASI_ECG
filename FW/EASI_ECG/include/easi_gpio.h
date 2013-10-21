#ifndef EASI_GPIO_H_
#define EASI_GPIO_H_
#include "stdio.h"
#include "csl_general.h"
#include "csl_gpio.h"
#include "csl_intc.h"
#include "easi_hardware.h"
#include "easi_rtc.h"
#include "interrupt_handlers.h"

CSL_Status EASI_GPIO_INIT();
void setPin(CSL_GpioPinNum pin, Uint16 val);
Uint16 readPin(CSL_GpioPinNum pin);
int getGpioInterruptStatus(CSL_GpioPinNum pin);
CSL_Status GpioClearInterrupt(CSL_GpioPinNum pin);
CSL_Status EASI_DRDY_INTERRUPT_EN();

void PRINT_GPIO_REGS();

void CONFIG_GPIO_INTERRUPTS();

#endif /*EASI_GPIO_H_*/
