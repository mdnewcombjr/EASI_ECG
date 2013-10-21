#ifndef EASI_UART_H_
#define EASI_UART_H_
#include "stdio.h"
#include "csl_uart.h"
#include "csl_uartAux.h"
#include "csl_intc.h"
#include "csl_general.h"

#define	CSL_UART_BUF_LEN    	(4U)

/* Global constants */
/* String length to be received and transmitted */
#define WR_STR_LEN        80
#define RD_STR_LEN        10


CSL_Status easiUartInit(void);

void uart_lsiIsr(void);
void uart_ctoIsr(void);
void uart_txIsr(void);
void uart_rxIsr(void);
void UART_intrDispatch(void);

CSL_Status UART_txChars(char* msg);



#endif /*EASI_UART_H_*/
