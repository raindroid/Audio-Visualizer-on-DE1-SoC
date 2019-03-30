#include "lib/JTAG_UART.h"
#include "lib/Audio.h"
#include "address_map_arm.h"
#include "string.h"

volatile int *JTAG_UART_ptr = (int *)JTAG_UART_BASE; // JTAG UART address
volatile int * audio_ptr   = (int *)AUDIO_BASE; // Audio in address

void uart_test();
void audio_test();

int main(void)
{
    // uart_test();
    audio_test();

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
    
    char text_string[] = "\nJTAG UART example code\n> \0";
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