#include "JTAG_UART.h"

static volatile int * red_LED_ptr = (int *)0xFF200000;
const char intToChar[10] = {'0','1','2','3','4','5','6','7','8','9'};  

void VIS_Uart_TxChar(volatile int * JTAG_UART_ptr, char c) {
    *(red_LED_ptr) = 0x1; // turn on LEDR[0]
    while (!((*(JTAG_UART_ptr + 1)) & 0xFFFF0000)) {
        // if space, echo character, else wait
    } 
    *(JTAG_UART_ptr) = c;
    *(red_LED_ptr) = 0x0; // turn off LEDR[0]
}

void VIS_Uart_Tx(volatile int * JTAG_UART_ptr, char * s, int size) {
    
    for (int i = 0; i < size; i++) {
        VIS_Uart_TxChar(JTAG_UART_ptr, s[i]);
        if (s[i] == 0) break;       // already reach the end of the string
    }
}

void VIS_UART_TxInt(volatile int * JTAG_UART_ptr, int num) {
    if (num < 0) {
        num = -num;
        VIS_Uart_TxChar(JTAG_UART_ptr, '-');
    }
    if (num < 10) {
        VIS_Uart_TxChar(JTAG_UART_ptr, intToChar[num]);
    } else {
        int tens = 1;
        while (num / tens > 0) tens *= 10;
        while (tens > 1) {
            tens /= 10;
            VIS_Uart_TxChar(JTAG_UART_ptr, intToChar[num / tens]);
            num %= tens;
        }
    }
}

void VIS_UART_TxEnter(volatile int * JTAG_UART_ptr) {
    VIS_Uart_TxChar(JTAG_UART_ptr, '\r');
    VIS_Uart_TxChar(JTAG_UART_ptr, '\n');
}

char VIS_Uart_RxChar(volatile int * JTAG_UART_ptr) {
    int data;
    data = *(JTAG_UART_ptr); // read the JTAG_UART data register
    if (data & 0x00008000)   // check RVALID to see if there is new data
        return ((char)data & 0xFF);
    else
        return ('\0');
}
