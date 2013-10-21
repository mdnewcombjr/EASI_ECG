#include "easi_uart.h"


CSL_UartSetup mySetup;

/* CSL UART data structures */
CSL_UartObj uartObj;
CSL_UartHandle hUart;

/* CSL UART data buffers */
Uint16    uartIntcWriteBuff[CSL_UART_BUF_LEN ];
Uint16    uartIntcReadBuff[CSL_UART_BUF_LEN ];
char      buffer1[30];
char      buffer2[10] = {0};
char      buffer3[30] = "\r\n";
Uint16    gcount;
Uint16    mutex;
char      *ptr;
Uint16    setbit;

CSL_UartSetup mySetup =
{
	/* Input clock freq in MHz */
#if SYS_CLKSPEED==100
	100000000,
#else    
    50000000,
#endif	
	/* Baud rate */
	460800,
	/* Word length of 8 */
    CSL_UART_WORD8,
	/* To generate 1 stop bit */
    0,
	/* Disable the parity */
    CSL_UART_DISABLE_PARITY,
	/* Enable trigger 14 fifo */
	CSL_UART_FIFO_DMA1_DISABLE_TRIG14,
	/* Loop Back enable */
	CSL_UART_NO_LOOPBACK,
	/* No auto flow control*/
	CSL_UART_NO_AFE ,
	/* No RTS */
	CSL_UART_NO_RTS ,
};

/* Interrupt vector start address */
extern void VECSTART(void);

#define CSL_PLL_CLOCKIN    (32768u)



Uint32 _getSysClk(void)
{
	Bool      pllRDBypass;
	Bool      pllOutDiv;
	Uint32    sysClk;
	Uint16    pllM;
	Uint16    pllRD;
	Uint16    pllOD;
   
	pllM = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR1, SYS_CGCR1_M);

	pllRD = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR2, SYS_CGCR2_RDRATIO);
	pllOD = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR4, SYS_CGCR4_ODRATIO);

	pllRDBypass = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR2, SYS_CGCR2_RDBYPASS);
	pllOutDiv   = CSL_FEXT(CSL_SYSCTRL_REGS->CGCR4, SYS_CGCR4_OUTDIVEN);

	sysClk = CSL_PLL_CLOCKIN;

	if (0 == pllRDBypass)
	{
		sysClk = sysClk/(pllRD + 4);
	}

	sysClk = (sysClk * (pllM + 4));

	if (1 == pllOutDiv)
	{
		sysClk = sysClk/(pllOD + 1);
	}

	/* Return the value of system clock in KHz */
	return(sysClk);
}

void uart_lsiIsr(void)
{
	Uint16 reg;
	reg=hUart->uartRegs->LSR;
	reg= reg;
}

/**
 *  \brief  Interrupt Service Routine to handle UART Character Timeout Interrupt
 *
 *  \param  none
 *
 *  \return none
 */
void uart_ctoIsr(void)
{
	UART_read( hUart,buffer2,0,0);
	ptr = buffer2;
	gcount = 1;
  	UART_eventEnable(hUart,CSL_UART_XMITOR_REG_EMPTY_INTERRUPT);
}

/**
 *  \brief  Interrupt Service Routine to handle UART Transmit Interrupt
 *
 *  \param  none
 *
 *  \return none
 */
void uart_txIsr(void)
{
	if(gcount == 1)
	{
		UART_write(hUart, buffer3, 2, 0);
		UART_write(hUart, ptr, gcount, 0);
		
		gcount = 13;
	}


	UART_write( hUart,buffer1,gcount,0);
	

	UART_eventDisable(hUart, CSL_UART_XMITOR_REG_EMPTY_INTERRUPT);
}

/**
 *  \brief  Interrupt Service Routine to handle UART Receive Interrupt
 *
 *  \param  none
 *
 *  \return none
 */
void uart_rxIsr(void)
{
	UART_read( hUart,buffer2,0,0);
	gcount = 1;
 	UART_eventEnable(hUart,CSL_UART_XMITOR_REG_EMPTY_INTERRUPT);
}

/**
 *  \brief  Interrupt Dispatcher to identify interrupt source
 *
 *  This function identify the type of UART interrupt generated and
 *  calls concerned ISR to handle the interrupt
 *
 *  \param  none
 *
 *  \return none
 */
interrupt void UART_intrDispatch(void)
{
	Uint16 eventId = 0;

	IRQ_disable(UART_EVENT);

	/* Get the event Id which caused interrupt */
	eventId = UART_getEventId(hUart);

	if (((void (*)(void))(hUart->UART_isrDispatchTable[eventId])))
	{
		((void (*)(void))(hUart->UART_isrDispatchTable[eventId]))();
	}

	IRQ_enable(UART_EVENT);

	return;
}

CSL_Status EASI_UART_REINIT()
{
	CSL_Status status;
	
	status = UART_setup(hUart,&mySetup);
    if(CSL_SOK != status)
    {
        printf("UART_setup re-init failed error code %d\n",status);
        return(status);
    }
	else
	{
		printf("UART_setup reinit was successful\n");
	}
	
	return status;
}

CSL_Status EASI_UART_INIT(void)
{
	CSL_UartIsrAddr    isrAddr;
	CSL_Status         status;

	Uint32            sysClk;
	
	sysClk = _getSysClk();
	
	mySetup.clkInput = sysClk;


	
	gcount = 16;
	ptr = buffer1;
	mutex = 0;
	
    /* Loop counter and error flag */
    status = UART_init(&uartObj,CSL_UART_INST_0,UART_INTERRUPT);
    if(CSL_SOK != status)
    {
        printf("UART_init failed error code %d\n",status);
        return(status);
    }
	else
	{
		printf("UART_init Successful\n");
	}

    /* Handle created */
    hUart = (CSL_UartHandle)(&uartObj);
    
    /* Configure UART registers using setup structure */
    status = UART_setup(hUart,&mySetup);
    if(CSL_SOK != status)
    {
        printf("UART_setup failed error code %d\n",status);
        return(status);
    }
	else
	{
		printf("UART_setup Successful\n");
	}
	
	// Send the details of the test to HyperTerminal //
   	status = UART_fputs(hUart,"UART IS UP!\r\n",0);
    if(CSL_SOK != status)
    {
        printf("UART_fputs failed error code %d\n",status);
        return(status);
    }
	
	/* Configure and Register the UART interrupts */
	isrAddr.rbiAddr  = uart_rxIsr;
	isrAddr.tbeiAddr = uart_txIsr;
	isrAddr.ctoi     = uart_ctoIsr;
	isrAddr.lsiAddr  = uart_lsiIsr;

    /* Disable interrupt */
    IRQ_globalDisable();

    /* Clear any pending interrupts */
	IRQ_clearAll();

	/* Disable all the interrupts */
	IRQ_disableAll();
	
	/* Configuring Interrupt */
	IRQ_plug (UART_EVENT, &UART_intrDispatch);

	/* Enabling Interrupt */
	IRQ_enable(UART_EVENT);
	IRQ_globalEnable();

	/* Set the UART callback function */
 	status = UART_setCallback(hUart,&isrAddr);
	if(status != CSL_SOK)
	{
		printf("UART_setCallback Failed\n");
		return(status);
	}

 	/* Enable the UART Events */
	status = UART_eventEnable(hUart, CSL_UART_XMITOR_REG_EMPTY_INTERRUPT);
	if(status != CSL_SOK)
	{
		printf("UART_eventEnable Failed\n");
		return(status);
	}

	status = UART_eventEnable(hUart, CSL_UART_RECVOR_REG_DATA_INTERRUPT);
	if(status != CSL_SOK)
	{
		printf("UART_eventEnable Failed\n");
		return(status);
	}

	status = UART_eventEnable(hUart, CSL_UART_RECVOR_LINE_STATUS_INTERRUPT);
	if(status != CSL_SOK)
	{
		printf("UART_eventEnable Failed\n");
		return(status);
	}
	
	return (status);
}



CSL_Status UART_txChars(char* msg)
{
	CSL_Status status;
	status = UART_fputs(hUart,msg,0);
    
    if(CSL_SOK != status)
    {
        printf("UART_fputs failed error code %d\n",status);
        return(status);
    }
    
    return (status);
}
