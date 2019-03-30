#ifndef _JTAG_UART_H__
#define _JTAG_UART_H__

/* function prototypes */
void VIS_Uart_TxChar(volatile int *, char);
void VIS_Uart_Tx(volatile int *, char * s, int size);
void VIS_UART_TxUint(volatile int *, int num);

char VIS_Uart_RxChar(volatile int *);


#endif // !_JTAG_UART_H__