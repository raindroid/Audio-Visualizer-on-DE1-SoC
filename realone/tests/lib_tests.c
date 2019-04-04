#include "tests.h"
#include "../address_map_arm.h"
#include "../lib/JTAG_UART.h"
#include "../lib/Audio.h"
#include <string.h>
#include "../lib/LED.h"
#include "../lib/HEX.h"
#include "../views/VGA_Display.h"
#include <stdlib.h>
#include "../values.h"
#include "../models/FFT.h"


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
    // if ((fifospace & 0x000000FF) > BUF_THRESHOLD) // check RARC
    {
        // store data until the the audio-in FIFO is empty or the buffer
        // is full
        // while ((fifospace & 0x000000FF) && (buffer_index < BUF_SIZE)) {
        while(1) {
            // left_buffer[buffer_index]  = *(audio_ptr + 2);
            // right_buffer[buffer_index] = *(audio_ptr + 3);
            while (!(fifospace & 0x000000FF));
            *(audio_ptr + 2)  = *(audio_ptr + 2) >> 6;
            *(audio_ptr + 3)  = *(audio_ptr + 3) >> 6;
            ++buffer_index;
            if (buffer_index >= 0xFFFFFF) buffer_index = 0;
            VIS_HEX_SetUint_HEX(buffer_index);
            if (VIS_Uart_RxChar(JTAG_UART_ptr) != 0) {
                // reset counter
                buffer_index = 0;
            }
            // fifospace = *(audio_ptr + 1); // read the audio port fifospace register
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

void display_test() {
    VIS_VGA_SetBuffer(SDRAM_BASE, FPGA_ONCHIP_BASE);
    unsigned k = 100;
    unsigned fakeSpect[MAX_LINES] = {0}; 
    while(1) {
        for (unsigned i = 0; i < k; i++) {
            if (i != 0 && rand() % 10 < 2)
                fakeSpect[i] = fakeSpect[i - 1] + (rand() % 800 - 400);
            else if (rand() % 10 < 3)
                fakeSpect[i] = rand() % 20000;
            else 
                fakeSpect[i] = rand() % 12000;
        }
        VIS_VGA_UpdateFrame(k, fakeSpect);
        // VIS_VGA_ColorTest();
    }
}

void color_test() {
    VIS_VGA_SetBuffer(SDRAM_BASE, FPGA_ONCHIP_BASE);
    VIS_VGA_ColorTest();
}

void audio_transform_test() {
    VIS_VGA_SetBuffer(SDRAM_BASE, FPGA_ONCHIP_BASE);
    volatile int * red_LED_ptr = (int *)LEDR_BASE;
    volatile int * audio_ptr   = (int *)AUDIO_BASE;

    /* used for audio record/playback */
    int fifospace;
    int buffer_index = 0;
    int fourierIndex = 0;
    const int fourierLength = 256;
    Complex cArray[fourierLength];
    float inputArray[fourierLength];
    Complex fourierResult[fourierLength];
    unsigned result[fourierLength/2];
    Complex c;
	

    Complex omegaInverse[fourierLength];
    init(fourierLength, omegaInverse);

    // Complex omegaExp[fourierLength][fourierLength];
    // initExps( fourierLength, omegaExp);

    fifospace = *(audio_ptr + 1); // read the audio port fifospace register
    // if ((fifospace & 0x000000FF) > BUF_THRESHOLD) // check RARC
    {
        // store data until the the audio-in FIFO is empty or the buffer
        // is full
        // while ((fifospace & 0x000000FF) && (buffer_index < BUF_SIZE)) {
        while(1) {
            // left_buffer[buffer_index]  = *(audio_ptr + 2);
            // right_buffer[buffer_index] = *(audio_ptr + 3);
            while (!(fifospace & 0x000000FF));
            *(audio_ptr + 2)  = *(audio_ptr + 2) >> 6;
            *(audio_ptr + 3)  = *(audio_ptr + 3) >> 6;

            //  if(buffer_index - buffer_index_start == fourierLength){
            //         buffer_index_start = buffer_index;
            //     if(count == 1) {
            //         count = 0;
                    
            //         //idft(cArray,fourierLength,omegaInverse);
            //             //FastFourierTransform(cArray,fourierLength);
            //         //fftamp[fourierIndex][fourierLength] = magnitude(cArray[i])%15000;
            //         idftMag(result,cArray,fourierLength,omegaInverse);
            //         VIS_VGA_UpdateFrame(fourierLength,result );
            //     }else{
            //         count ++;
            //     }
            //   }
            
            if(buffer_index == fourierLength){
                buffer_index = 0;
                //sdft(cArray, fourierLength, omegaInverse, fourierResult);
                //fdft(cArray, fourierLength, omegaExp, fourierResult);
                fft(cArray,fourierLength);
                for(int i=0;i<fourierLength/2; i++){
                    result[i] = magnitude(cArray[i]);
                }
                    VIS_VGA_UpdateFrame(fourierLength/2, result );
            }
            
            
            c.i = 0;
            c.r = ((float) *(audio_ptr + 2))/500000000;
            cArray[buffer_index] = c;

            inputArray[buffer_index] = c.r;

            ++buffer_index;
            if (buffer_index >= 0xFFFFFF) buffer_index = 0;
            VIS_HEX_SetUint_HEX(buffer_index);
            if (VIS_Uart_RxChar(JTAG_UART_ptr) != 0) {
                // reset counter
                buffer_index = 0;
            }

            // fifospace = *(audio_ptr + 1); // read the audio port fifospace register
        }
    }
}
void check_KEYs(int * KEY0, int * KEY1, int * KEY2, int * counter) {
    volatile int * KEY_ptr   = (int *)KEY_BASE;
    volatile int * audio_ptr = (int *)AUDIO_BASE;
    int            KEY_value;

    KEY_value = *(KEY_ptr); // read the pushbutton KEY values
    while (*KEY_ptr)
        ; // wait for pushbutton KEY release

    if (KEY_value == 0x1) // check KEY0
    {
        // reset counter to start recording
        *counter = 0;
        // clear audio-in FIFO
        *(audio_ptr) = 0x4;
        *(audio_ptr) = 0x0;

        *KEY0 = 1;
    } else if (KEY_value == 0x2) // check KEY1
    {
        // reset counter to start playback
        *counter = 0;
        // clear audio-out FIFO
        *(audio_ptr) = 0x8;
        *(audio_ptr) = 0x0;

        *KEY1 = 1;
    }
    else if (KEY_value == 0x4) // check KEY2
    {
        // reset counter to start playback
        *counter = 0;
        // clear audio-out FIFO
        *(audio_ptr) = 0x8;
        *(audio_ptr) = 0x0;

        *KEY2 = 1;
    }
}
