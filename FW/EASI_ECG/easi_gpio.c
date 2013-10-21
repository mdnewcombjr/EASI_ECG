#include "easi_gpio.h"
GPIO_Handle hGpio;
Uint16 tempct = 0;


extern void VECSTART(void);

void setPin(CSL_GpioPinNum pin, Uint16 val)
{
	CSL_Status result;
	
	result = GPIO_write(hGpio,pin,val);
	if(result != CSL_SOK)
	{
		printf("setPin() went wrong!\n");
		return;
	}
}

Uint16 readPin(CSL_GpioPinNum pin)
{
	CSL_Status status;
	Uint16 returnval = 0;
	
	status = GPIO_read(hGpio,pin,&returnval);
	if(status != CSL_SOK)
	{
		printf("Problem reading GPIO pin.\n");
		return 0xffff;
	}
	
	return returnval;
}
	
//A very conservative reset function for the ECG reset pulse
CSL_Status pulseEcgReset(GPIO_Handle hGPIO)
{
	CSL_Status result;
	//Pulse the #ECG_RESET signal
	result = GPIO_write(hGPIO,ADS1294_RESET,1);
	if(result != CSL_SOK)
	{
		printf("Couldn't start #ECG_RESET pulse!\n");
		return result;
	}
	wait(1);
	//Pulse the #ECG_RESET signal
	result = GPIO_write(hGPIO,ADS1294_RESET,0);
	if(result != CSL_SOK)
	{
		printf("Couldn't start #ECG_RESET pulse!\n");
		return result;
	}
	wait(1);
	result = GPIO_write(hGPIO,ADS1294_RESET,1);
	if(result != CSL_SOK)
	{
		printf("Couldn't end #ECG_RESET pulse!\n");
		return result;
	}
	
	//Wait after #ECG_RESET pulse
	wait(1);
		
	return result;
}

void PRINT_GPIO_REGS()
{
	//DEBUG REGS
	printf("IODIR1: %#X | IODIR2: %#X\n",(CSL_GPIO_REGS)->IODIR1,hGpio->baseAddr->IODIR2);
	printf("IOINTEN1: %#X | IOINTEN2: %#X\n",(CSL_GPIO_REGS)->IOINTEN1,hGpio->baseAddr->IOINTEN2);
	printf("IOINTEDG1: %#X | IOINTEDG2: %#X\n",(CSL_GPIO_REGS)->IOINTEDG1,hGpio->baseAddr->IOINTEDG2);
	printf("IOINTFLG1: %#X | IOINTFLG2: %#X\n",(CSL_GPIO_REGS)->IOINTFLG1,hGpio->baseAddr->IOINTEN2);
}

void CONFIG_GPIO_INTERRUPTS()
{
	CSL_Status status;
	CSL_IRQ_Config  irqConfig;
	CSL_IRQ_Config irqTest;
	Bool flag = 1;
	
	//Disable CPU interrupt
	IRQ_globalDisable();
	
	//Clear any pending interrupts
	IRQ_clearAll();
	
	//Disable all interrupts
	IRQ_disableAll();
	
	IRQ_setVecs((Uint32)&VECSTART);
	
	//Init IRQ 
	status = IRQ_init((CSL_IRQ_Dispatch*)0x0000,0);
	if(status != CSL_SOK)
		printf("Had a problem initializing CSL IRQ lib\n");

	irqConfig.funcAddr = &ads1294_GPIO_ISR;
    irqConfig.funcArg = 0;
    IRQ_config(GPIO_EVENT,&irqConfig);
	
	//Test that the IRQ_Config worked
	IRQ_getConfig(GPIO_EVENT,&irqTest);
	if(irqTest.funcAddr != irqConfig.funcAddr)
		printf("Problem configuring global irq table\n");
	
	
	CSL_GPIO_REGS->IODIR2 = 0x0002; //Set GPIO17(#ECG_RESET) as an output (GPIO16 - #DRDY - is an input)
	CSL_GPIO_REGS->IODIR1 = 0xc000; //Set GPIO15(ECG_START) and GPIO14(ADC_CLK_SEL) as outputs
	CSL_GPIO_REGS->IOINTEN1 = 0x0000; //No GPIO interrupt capability on bottom 0-15 GPIOs
	CSL_GPIO_REGS->IOINTEN2 = 0x0001; //Enable interrupt on GPIO16(#DRDY)
	CSL_GPIO_REGS->IOINTEDG2 = 0x0001; //GPIO16(#DRDY) triggers on falling edge
	CSL_GPIO_REGS->IOINTEDG1 = 0x0000;
	
	//Clear the interrupt flags by writing 1 to everything
	CSL_GPIO_REGS->IOINTFLG1 = 0xFFFF;
	CSL_GPIO_REGS->IOINTFLG2 = 0xFFFF;
		
	if(ADS1294_DRDY < CSL_GPIO_PIN16)
	{
		CSL_FINSR(CSL_GPIO_REGS->IOINTEN2,ADS1294_DRDY,ADS1294_DRDY,CSL_GPIO_BIT_SET);
	}
	else
	{
		CSL_FINSR(CSL_GPIO_REGS->IOINTEN2,(ADS1294_DRDY-CSL_GPIO_PIN16),(ADS1294_DRDY-CSL_GPIO_PIN16),CSL_GPIO_BIT_SET);
	}
	
	//Clear any pending interrupts
	IRQ_clear(GPIO_EVENT);
	IRQ_test(GPIO_EVENT,&flag);
	IRQ_map(GPIO_EVENT);
	
	//Register the GPIO ISR
	IRQ_plug(GPIO_EVENT,&ads1294_GPIO_ISR);
	
	//Enable the interrupts
	IRQ_enable(GPIO_EVENT);
	IRQ_globalEnable();
	
}

CSL_Status EASI_GPIO_INIT()
{
	CSL_GpioObj GpioObj;
	CSL_GpioConfig config;
	CSL_Status result;		
	
	
	hGpio = GPIO_open(&GpioObj,&result);
	if((hGpio == NULL) || (result != CSL_SOK))
	{
		printf("GPIO_open failed!\n");
	}
	else
	{
		printf("GPIO_open succeeded\n");
	}
	
	config.GPIODIRH = 0x0002; //Set GPIO17(#ECG_RESET) as an output (GPIO16 - #DRDY - is an input)
	config.GPIODIRL = 0xc000; //Set GPIO15(ECG_START) and GPIO14(ADC_CLK_SEL) as outputs
	config.GPIOINTENAL = 0x0000; //No GPIO interrupt capability on bottom 0-15 GPIOs
	config.GPIOINTENAH = 0x0001; //Enable interrupt on GPIO16(#DRDY)
	config.GPIOINTTRIGH = 0x0001; //GPIO16(#DRDY) triggers on falling edge
	config.GPIOINTTRIGL = 0x0000;
	
	result = GPIO_config(hGpio,&config);
	if(result != CSL_SOK)
	{
		printf("Had a problem configuring the GPIOs!\n");
		return result;	
	}
	
	
	//Per ads1294 quickstart guide, once power supplies are stable we begin supplying a clock to the ADS1294
	printf("Waiting for ADS1294 clock to start...");
	result = GPIO_write(hGpio,ADS1294_ADC_CLK_SEL,1);
	if(result != CSL_SOK)
	{
		printf("Couldn't set ADC_CLK_SEL!\n");
		return result;
	}
	
	
	//Wait at least 20 us for the internal oscillator to start up
	wait(1);
	
	printf("done.\nWaiting for ADS1294 to be ready for RESET pulse...");
	
	result = GPIO_write(hGpio,ADS1294_RESET,1);
	if(result != CSL_SOK)
	{
		printf("Couldn't set #ECG_RESET!\n");
		return result;
	}
	
	//Must wait before pulsing the #ECG_RESET signal
	wait(1000);
	printf("Pulsing the #ECG_RESET signal...");
	
	
	//Pulse the #ECG_RESET line
	result = pulseEcgReset(hGpio);
	if(result != CSL_SOK)
		return result;

	printf("done.\n");
	//Wait about 100 ms to let the device start up
	wait(100);
	//We should be ready to program config registers!	

	return result;
}

CSL_Status EASI_DRDY_INTERRUPT_EN()
{
	CSL_Status result;
	//enable the interrupt on the DRDY pin
	result = GPIO_enableInt(hGpio,ADS1294_DRDY);
	
	return result;
}


int getGpioInterruptStatus(CSL_GpioPinNum pin)
{
	int flag = -1;
	CSL_Status status;
	flag = GPIO_statusBit(hGpio,pin,&status);
	
	if(status != CSL_SOK)
	{
		printf("Could not get GPIO interrupt status\n");
	}
	
	return flag;
}



