#include "address_map_arm.h"
#include <math.h>
/* globals */
#define BUF_SIZE 80000 // about 10 seconds of buffer (@ 8K samples/sec)
#define BUF_THRESHOLD 96 // 75% of 128 word buffer
/* function prototypes */
void check_KEYs(int *, int *, int *);
volatile int pixel_buffer_start; // global variable

void plot_pixel(int x, int y, short int line_color);
void clear_screen();
void draw_line(int x0, int y0, int x1, int y1, short int line_color);
void draw_rect(int x, int y, short int line_color);
void clear_screen();
void wait_for_vsync();
/*******************************************************************************
* This program performs the following:
* 1. records audio for 10 seconds when KEY[0] is pressed. LEDR[0] is lit
* while recording.
* 2. plays the recorded audio when KEY[1] is pressed. LEDR[1] is lit while
* playing.
******************************************************************************/
int main(void) {
    /* Declare volatile pointers to I/O registers (volatile means that IO load
    and store instructions will be used to access these pointer locations,
    instead of regular memory loads and stores) */
    volatile int * red_LED_ptr = (int *)LEDR_BASE;
    volatile int * audio_ptr = (int *)AUDIO_BASE;
    /* used for audio record/playback */
    int fifospace;
    int record = 0, play = 0, buffer_index = 0;
    int left_buffer[BUF_SIZE];
    int right_buffer[BUF_SIZE];

    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)
    int n = 8;
    int x_xs[n], y_xs[n], dx_xs[n], dy_xs[n], color_xs[n];
    int x_last[n], y_last[n];
    int x_last2[n], y_last2[n];
    int color[6] = {0x001F, 0x07E0, 0xF800, 0xFFFF, 0X0AAA, 0X4514};
    int i;
    for(i = 0; i < n ; i++){
        x_xs[i] = rand()%316 + 2;
        y_xs[i] = rand()%236 + 2;
        x_last2[i] = x_xs[i];
        y_last2[i] = y_xs[i];
        x_last[i] = x_xs[i];
        y_last[i] = y_xs[i];

        dx_xs[i] = rand()%2 *2 - 1;
        dy_xs[i] = rand()%2 *2 - 1;
        
        color_xs[i] = color[rand()%6];
    }
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    // pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    // clear_screen();

    /* read and echo audio data */
    record = 0;
    play = 0;
    while (1) {
        check_KEYs(&record, &play, &buffer_index);
        if (record) {
            *(red_LED_ptr) = 0x1; // turn on LEDR[0]
            fifospace = *(audio_ptr + 1); // read the audio port fifospace register
            if ((fifospace & 0x000000FF) > BUF_THRESHOLD) // check RARC
            {
                // store data until the the audio-in FIFO is empty or the buffer
                // is full
                while ((fifospace & 0x000000FF) && (buffer_index < BUF_SIZE)) {
                    left_buffer[buffer_index] = *(audio_ptr + 2);
                    right_buffer[buffer_index] = *(audio_ptr + 3);
                    ++buffer_index;
                if (buffer_index == BUF_SIZE) {
                        // done recording
                    record = 0;
                    *(red_LED_ptr) = 0x0; // turn off LEDR
                }
                    fifospace = *(audio_ptr + 1); // read the audio port fifospace register
                }
            }
        } else if (play) {
            *(red_LED_ptr) = 0x2; // turn on LEDR_1
            fifospace = *(audio_ptr + 1); // read the audio port fifospace register
            if ((fifospace & 0x00FF0000) > BUF_THRESHOLD) // check WSRC
            {
                // output data until the buffer is empty or the audio-out FIFO
                // is full
                while ((fifospace & 0x00FF0000) && (buffer_index < BUF_SIZE)) {
                    *(audio_ptr + 2) = left_buffer[buffer_index];
                    *(audio_ptr + 3) = right_buffer[buffer_index];
                    ++buffer_index;
                    if (buffer_index == BUF_SIZE) {
                        // done playback
                        play = 0;
                        *(red_LED_ptr) = 0x0; // turn off LEDR
                    }
                    fifospace = *(audio_ptr + 1); // read the audio port fifospace register


                    int j;
                    /* Erase any boxes and lines that were drawn in the last iteration */
                    //clear_screen();
                    int line = 0;
                    int lastline = 0;
                    int colorLine = 0x0000;
                    if(buffer_index % 100 == 0){
                        lastline = line;
                        line = 10*log(*(audio_ptr + 2)*10);
                        draw_line(0,120,319,120,0x0000);
                        colorLine = color[rand()%6];
                    }
                    draw_line(0,120,line,120,colorLine);
                }
            }
        }
    }
}
/****************************************************************************************
* Subroutine to read KEYs
****************************************************************************************/
void check_KEYs(int * KEY0, int * KEY1, int * counter) {
volatile int * KEY_ptr = (int *)KEY_BASE;
volatile int * audio_ptr = (int *)AUDIO_BASE;
int KEY_value;
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
}

// code for subroutines (not shown)
void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;

}

void draw_rect(int x, int y, short int line_color){
    int i;
    int j;
    for(i=-1;i<=1;i++){
        for(j=-1;j<=1;j++){
            plot_pixel(x+i,y+j,line_color);
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, short int line_color){
    int is_steep;
    if(abs(y1 - y0) > abs(x1 - x0)){
        is_steep = 1;
    }
    else
    {
        is_steep = 0;
    }
    
    if(is_steep == 1){
        /*
        swap(x0, y0)
        swap(x1, y1)
        */
        int temp1 = x0;
        x0 = y0;
        y0 = temp1;
        int temp2 = x1;
        x1 = y1;
        y1 = temp2;
    }
    if(x0 > x1){
        /*
        swap(x0, x1)
        swap(y0, y1)
        */
       int temp1 = x0;
       x0 = x1;
       x1 = temp1;
       int temp2 = y0;
       y0 = y1;
       y1 = temp2;
    }


    int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    int error = -(deltax / 2);
    int y = y0;
    int y_step;
    if(y0 < y1){
        y_step = 1;
    }
    else{
        y_step = -1;
    } 
    int x;
    for(x=x0;x<=x1;x++){
        if( is_steep == 1){
            plot_pixel(y,x,line_color);
        }
        else{
            plot_pixel(x,y,line_color);
        }
        
        error = error + deltay;
    
        if (error >= 0) {
            y = y + y_step;
            error = error - deltax;
        }
    }
}

//write ervery pixel to clear screen
void clear_screen(){
    int x;
    int y;
    for(x = 0;x < 320; x++){
        for(y = 0;y < 240; y++){
            plot_pixel(x,y,0x0000);
        }
    }
}


void wait_for_vsync(){
    volatile int * pixel_ctr_ptr =(int *) 0xFF203020;  //pixel controller
    register int status;

    *pixel_ctr_ptr = 1; // start synchronization process

    status = *(pixel_ctr_ptr + 3);
    while((status & 0x01) !=0){
        status = *(pixel_ctr_ptr + 3);
    }
}