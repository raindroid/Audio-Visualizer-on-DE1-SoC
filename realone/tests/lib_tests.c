#include "tests.h"
#include "../address_map_arm.h"
#include "../lib/JTAG_UART.h"
#include "../lib/Audio.h"
#include <string.h>
#include "../lib/LED.h"
#include "../lib/HEX.h"


static volatile int *JTAG_UART_ptr = (int *)JTAG_UART_BASE; // JTAG UART address
static volatile int * audio_ptr   = (int *)AUDIO_BASE; // Audio in address

// LED test passed
void led_test() {
    VIS_LED_Set(1, VIS_LED_SET);
    VIS_LED_Set(3, VIS_LED_SET);
    VIS_LED_Set(5, VIS_LED_SET);
    VIS_LED_Set(7, VIS_LED_SET);
    VIS_LED_Toggle(9);
    VIS_LED_Toggle(2);
    VIS_LED_Toggle(2);
    VIS_LED_Set(4, VIS_LED_SET);
    VIS_LED_Set(4, VIS_LED_RESET);
}

void audio_test() {
 /* used for audio record/playback */
    int fifospace, buffer_index = 0;
    int left_buffer[BUF_SIZE];
    int right_buffer[BUF_SIZE];

    fifospace = *(audio_ptr + 1); // read the audio port fifospace register
    if ((fifospace & 0x000000FF) > BUF_THRESHOLD) // check RARC
    {
        // store data until the the audio-in FIFO is empty or the buffer
        // is full
        while ((fifospace & 0x000000FF) && (buffer_index < BUF_SIZE)) {
            left_buffer[buffer_index]  = *(audio_ptr + 2);
            right_buffer[buffer_index] = *(audio_ptr + 3);
            ++buffer_index;
            fifospace = *(audio_ptr + 1); // read the audio port fifospace register
        }
    }
    VIS_Uart_Tx(JTAG_UART_ptr, "Buffer Size=", strlen("Buffer Size="));
    VIS_UART_TxInt(JTAG_UART_ptr, BUF_SIZE);
    VIS_Uart_Tx(JTAG_UART_ptr, "\r\n", strlen("\r\n"));
    

    for (int i = 0; i < 200; i++) {
        VIS_UART_TxInt(JTAG_UART_ptr, i);
        VIS_Uart_TxChar(JTAG_UART_ptr, ':');
        VIS_UART_TxInt(JTAG_UART_ptr, left_buffer[i]);
        VIS_Uart_Tx(JTAG_UART_ptr, "\r\n", strlen("\r\n"));
    }
}

void uart_test() {
    
    char text_string[] = "\nJTAG UART test\n> \0";
    char c;

    VIS_Uart_Tx(JTAG_UART_ptr, text_string, strlen(text_string));

    for (int i = 0; i < BUF_SIZE; i++) {
        VIS_UART_TxInt(JTAG_UART_ptr, i);
        VIS_Uart_TxChar(JTAG_UART_ptr, ' ');
    }

    /* read and echo characters */
    while (1)
    {
        c = VIS_Uart_RxChar(JTAG_UART_ptr);
        if (c != '\0') {
            VIS_Uart_Tx(JTAG_UART_ptr, text_string, strlen(text_string));

            VIS_Uart_TxChar(JTAG_UART_ptr, c);
        }
    }
}

void hex_test() {
    char text_string[] = "\nPress any key to continue\n> \0";
    VIS_HEX_Set(0, (0x1U << 6) + 0x1U);
    VIS_Uart_Tx(JTAG_UART_ptr, text_string, strlen(text_string));
    while(VIS_Uart_RxChar(JTAG_UART_ptr) == '\0');
    VIS_HEX_SetDigit(1, 0x3);
    VIS_Uart_Tx(JTAG_UART_ptr, text_string, strlen(text_string));
    while(VIS_Uart_RxChar(JTAG_UART_ptr) == '\0');
    VIS_HEX_SetDigit(2, 0x2);
    VIS_Uart_Tx(JTAG_UART_ptr, text_string, strlen(text_string));
    while(VIS_Uart_RxChar(JTAG_UART_ptr) == '\0');
    VIS_HEX_SetDigit(3, 0x1);
    VIS_Uart_Tx(JTAG_UART_ptr, text_string, strlen(text_string));
    while(VIS_Uart_RxChar(JTAG_UART_ptr) == '\0');
    VIS_HEX_Set(4, (0x1U << 6) + 0x1U);
    VIS_Uart_Tx(JTAG_UART_ptr, text_string, strlen(text_string));
    while(VIS_Uart_RxChar(JTAG_UART_ptr) == '\0');
    VIS_HEX_SetDigit(5, 0xF);

    VIS_Uart_Tx(JTAG_UART_ptr, text_string, strlen(text_string));
    while(VIS_Uart_RxChar(JTAG_UART_ptr) == '\0');

    VIS_HEX_SetUint_DEC(123456);
    VIS_Uart_Tx(JTAG_UART_ptr, text_string, strlen(text_string));
    while(VIS_Uart_RxChar(JTAG_UART_ptr) == '\0');

    VIS_HEX_SetUint_HEX(0x123ACF);
    
}