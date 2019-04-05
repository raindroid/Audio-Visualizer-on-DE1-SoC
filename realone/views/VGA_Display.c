#include "VGA_Display.h"
#include <stdbool.h>
#include "../values.h"
#include <math.h>
#include "../helpers/Math.h"
#include "../lib/JTAG_UART.h"
#include <string.h>

static volatile unsigned pixel_buffer_start; // global variable
static volatile unsigned * pixel_ctrl_ptr = (unsigned *)0xFF203020;
static volatile int *JTAG_UART_ptr = (int *)JTAG_UART_BASE; // JTAG UART address

static RingProperty ring;
static Line canvasHistory[2][MAX_LINES]; 
static int historyIndicator;

void VIS_VGA_Setup() {
    ring_reset();
    historyIndicator = 0; 
    char help[512] = "Help: \r\n"
            "\t1. Press [W/A/S/D] to move the ring up/left/down/right.\r\n"
            "\t2. Press [Q/E] to change the length of ring (shorter/longer).\r\n"
            "\t3. Press [Z/X] to change the brightness.\r\n"
            "\t4. Press [R] to reset\r\n";
    VIS_Uart_Tx(JTAG_UART_ptr, help, 512);
}

void ring_reset() {
    ring.colorSeed = 0;
    ring.offsetDeg = 0;
    ring.radius = SCREEN_H / 8;
    ring.inOffset = 100;
    ring.loudnessThreshold = 1000;
    ring.cX = SCREEN_W / 2;
    ring.cY = SCREEN_H / 2;
    ring.maxLength = (SCREEN_H / 3 - ring.radius);
    ring.brightness = 128;
}

RingProperty *get_ring() {
    return &ring;
}

void ring_controller() {
    char control;
    if ((control = VIS_Uart_RxChar(JTAG_UART_ptr)) != 0) {
        switch (control)
        {
            case 'W':
                ring.cY --;
                if (ring.cY < 0) ring.cY = 0;
                break;
            case 'A':
                ring.cX --;
                if (ring.cX < 0) ring.cX = 0;
                break;
            case 'S':
                ring.cY ++;
                if (ring.cY > SCREEN_H) ring.cY = SCREEN_H;
                break;
            case 'D':
                ring.cX ++;
                if (ring.cX > SCREEN_W) ring.cX = SCREEN_W;
                break;
            case 'Q':
                ring.maxLength --;
                break;
            case 'E':
                ring.maxLength ++;
                break;
            case 'Z':
                ring.brightness ++;
                break;
            case 'X':
                ring.brightness --;
                break;
            case 'R':
                ring_reset();
                break;
            default:
                break;
        }
    }
}

void VIS_VGA_UpdateFrame(unsigned size, unsigned spect[]) {
    while((*(pixel_ctrl_ptr + 3)) & 0x1); // wait for the previous swap
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    historyIndicator = 1 - historyIndicator;

    // find the largest number and default max is set in ring property
    unsigned maxLoudness = ring.loudnessThreshold;
    for (int i = 0; i < size; i++) {
        maxLoudness = maxLoudness >= spect[i] ? maxLoudness : spect[i];
    }

    // // draw on VGA
    // unsigned startDeg = ring.offsetDeg;
    for (int i = 0; i < size; i++) {
        unsigned color = color_from_gradient_hsv(ring.colorSeed + i * COLOR_RANGE / size);
        unsigned degree = i * 360 / size + (ring.offsetDeg >> 4);

        // // calculate start point
        int innerR = ring.radius - ((ring.inOffset * ring.maxLength * spect[i] / maxLoudness) >> 10);
        unsigned iX = ring.cX + ((innerR * VIS_FastSin_d16(degree)) >> 16);
        unsigned iY = ring.cY - ((innerR * VIS_FastCos_d16(degree)) >> 16);

        // // calculate color end point
        int outerR = ring.radius + (((1024 - ring.inOffset) * ring.maxLength * spect[i] / maxLoudness) >> 10); 
        unsigned oX = ring.cX + ((outerR * VIS_FastSin_d16(degree)) >> 16);
        unsigned oY = ring.cY - ((outerR * VIS_FastCos_d16(degree)) >> 16);

        // update history
        draw_lineStruct(canvasHistory[historyIndicator][i]);
        canvasHistory[historyIndicator][i] = (Line){iX, iY, oX, oY, 0};

        draw_line(iX, iY, oX, oY, color);

        // update ring properties
        // ring.colorSeed = (ring.colorSeed + 1) & 1F;
        ring.offsetDeg = (ring.offsetDeg + 1) % (360 << 4);
    }


    // swap front and back buffers on VGA vertical sync
    *pixel_ctrl_ptr = 0x1;
    ring_controller();
}

void VIS_VGA_ColorTest() {
    
    while((*(pixel_ctrl_ptr + 3)) & 0x1); // wait for the previous swap
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    
    clear_screen();
    for (int i = 0; i < SCREEN_H; i++) {
        draw_line(0, i, SCREEN_W - 1, i, color_from_gradient_hsv(i * COLOR_RANGE / SCREEN_H));
    }

    // swap front and back buffers on VGA vertical sync
    *pixel_ctrl_ptr = 0x1;
}

void VIS_VGA_SetBuffer(unsigned frontAddress, unsigned backAddress) {
    *(pixel_ctrl_ptr + 1) = frontAddress;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen();
    wait_for_vsync();
    
    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = backAddress;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the front buffer
    clear_screen();
    wait_for_vsync();

    VIS_VGA_Setup();
}

int color_from_RGB888(unsigned r, unsigned g, unsigned b) {
    r = (r > 255) ? 255 : r;
    g = (g > 255) ? 255 : g;
    b = (b > 255) ? 255 : b;
    return (r >> 3 << 11 ) | (g >> 2 << 5 ) | (b >> 3 & 0x1F);
}

int color_from_gradient_hsv(int cid) {
    cid = cid % COLOR_RANGE;
    cid = COLOR_RANGE - cid;    // reverse the colors of high freq and low freq
    static int r = 0, g = 0, b = 0;
    if (cid <= COLOR_RANGE * 3 / 10) {
        r = 0xFF;
        g = cid * 85 / 24;
        b = 0x0;
    } else if (cid < COLOR_RANGE * 5 / 10) {
        r = (COLOR_RANGE * 3 / 10 - cid) * 85 / 24;
        g = 0xFF;
        b = 0x0;
    } else if (cid < COLOR_RANGE * 7 / 10) {
        r = 0x0;
        g = 0xFF;
        b = (cid - COLOR_RANGE * 5 / 10) * 85 / 24;;
    } else {
        r = 0x0;
        g = (COLOR_RANGE - cid) * 85 / 24;
        b = 0xFF;
    }
    r = r * 128 / ring.brightness;
    g = g * 128 / ring.brightness;
    b = b * 128 / ring.brightness;
    return color_from_RGB888(r, g, b);
}

void wait_for_vsync() {
    *pixel_ctrl_ptr = 0x1;
    while((*(pixel_ctrl_ptr + 3)) & 0x1); // wait for the previous swap
}

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void draw_lineStruct(Line line) {
    draw_line(line.iX, line.iY, line.oX, line.oY, line.color);
}

void draw_line(int p1_x, int p1_y, int p2_x, int p2_y, int color)
{
    // check if it is a point or a line
    if (p2_y == p1_y && p1_x == p1_y) {
        plot_pixel(p1_x, p2_y, color);
    }

    bool is_steep = abs(p1_y - p2_y) > abs(p1_x - p2_x);
    if (is_steep)
    {
        swap(&p1_x, &p1_y);
        swap(&p2_x, &p2_y);
    }
    if (p1_x > p2_x)
    {
        swap(&p1_x, &p2_x);
        swap(&p1_y, &p2_y);
    }

    int deltaX = p2_x - p1_x,
        deltaY = abs(p2_y - p1_y),
        error = -(deltaX / 2),
        yStep = deltaY ? deltaY / (p2_y - p1_y) : 0;
    
    for (int x = p1_x, y = p1_y; x <= p2_x; x++) {
        if (is_steep) {
            plot_pixel(y, x, color);
        } else {
            plot_pixel(x, y, color);
        }

        error += deltaY;
        if (error >= 0) {
            y += yStep;
            error -= deltaX;
        }
    }
}
void draw_box(int px, int py, int w, int h, int color) {
    for (int x = px; x < px + w; x++) {
        draw_line(x, py, x, py + h - 1, color);
    }
}

void clear_screen() {
    for (int x = 0; x <= 319; x++) {
        draw_line(x, 0, x, 239, 0);
    }
}

void plot_pixel(int x, int y, unsigned short int line_color)
{
    if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H) return; 
    if (line_color > 0xFFFF) line_color = 0xFFFF;
    *(unsigned short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}
