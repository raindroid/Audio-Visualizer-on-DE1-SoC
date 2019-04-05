#include "tests/tests.h"
#include "address_map_arm.h"
#include "lib/JTAG_UART.h"
#include "lib/Audio.h"
#include <string.h>
#include "lib/LED.h"
#include "lib/HEX.h"
#include "views/VGA_Display.h"
#include <stdlib.h>
#include "values.h"
#include "models/FFT.h"

void audio_visualizer();

int main(void)
{
    // led_test();
    // uart_test();
    //audio_test();
    // hex_test();
    //  audio_transform_test();
    //display_test();
    // color_test();

    audio_visualizer();

    for (;;); 
}

void audio_visualizer() {
VIS_VGA_SetBuffer(SDRAM_BASE, FPGA_ONCHIP_BASE);
    volatile int * red_LED_ptr = (int *)LEDR_BASE;
    volatile int * audio_ptr   = (int *)AUDIO_BASE;

    /* used for audio record/playback */
    int fifospace;
    int buffer_index = 0;
    Complex cArray[FOURIER_LENGTH];
    unsigned result[FOURIER_LENGTH/2-IGNORE];
    unsigned lastResult3[FOURIER_LENGTH/2-IGNORE];
    unsigned lastResult2[FOURIER_LENGTH/2-IGNORE];
    unsigned lastResult[FOURIER_LENGTH/2-IGNORE];
    unsigned displayResult[FOURIER_LENGTH/2-IGNORE];
    Complex c;
    float ampmax = 0;
	

    Complex omegaInverse[FOURIER_LENGTH];
    init(FOURIER_LENGTH, omegaInverse);
    int left, right;


    fifospace = *(audio_ptr + 1); // read the audio port fifospace register
    while(1) {

        if(buffer_index == FOURIER_LENGTH){

            buffer_index = 0;
            fft(cArray,FOURIER_LENGTH);
            for(int i=0;i<FOURIER_LENGTH/2-IGNORE; i++){ //negelect first 4 spectrum
                lastResult3[i] = lastResult2[i];
                lastResult2[i] = lastResult[i];
                lastResult[i] = result[i];
                result[i] = magnitude(cArray[i+IGNORE]);
                displayResult[i] = 
                (int)((lastResult3[i]+lastResult2[i]+lastResult[i]+result[i])/4*ampmax );
            }
            VIS_VGA_UpdateFrame(FOURIER_LENGTH/2-IGNORE, displayResult);
            ampmax = 0;
        }

        // wait for sound
        while (!(fifospace & 0x000000FF));
        
        // play the sound
        left  = *(audio_ptr + 2) >> 6;
        *(audio_ptr + 2) = left;
        right  = *(audio_ptr + 3) >> 6;
        *(audio_ptr + 3) = right;

        // save to our buffer
        c.i = 0;
        c.r = left/100000000.0f;
        if(abs(c.r) > ampmax){
            ampmax = abs(c.r);
        }
        cArray[buffer_index] = c;


        ++buffer_index;
        if (buffer_index >= 0xFFFFFF) buffer_index = 0;
        VIS_HEX_SetUint_HEX(buffer_index);
    }
}