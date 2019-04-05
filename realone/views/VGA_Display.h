#ifndef VGA_DISPLAY_H__
#define VGA_DISPLAY_H__

#define SCREEN_W 320
#define SCREEN_H 240
#define COLOR_RANGE 240

typedef struct {
    int colorSeed;  // color base of the gradient
    int offsetDeg;  // offset for circle rotating
    int radius;     // radius of circile
    int inOffset;   // percent offset for line inside the circle (0 - 1024)
    int cX, cY;     // center point
    int maxLength;  // max length of a single line
    int brightness;
    unsigned loudnessThreshold;
} RingProperty;

typedef struct {
    unsigned iX, iY;
    unsigned oX, oY;
    int color;
} Line;

RingProperty *get_ring();
void VIS_VGA_SetBuffer(unsigned frontAddress, unsigned backAddress);
void VIS_VGA_Setup();
void VIS_VGA_UpdateFrame(unsigned size, unsigned spect[]);
void VIS_VGA_ColorTest();

// private
void draw_line(int p1_x, int p1_y, int p2_x, int p2_y, int color);
void draw_lineStruct(Line line);
void plot_pixel(int x, int y, unsigned short int line_color);
void draw_box(int px, int py, int w, int h, int color);
void clear_screen();
void wait_for_vsync();
void swap(int *a, int *b);
int color_from_RGB888(unsigned r, unsigned g, unsigned b);
int color_from_gradient_hsv(int);
void ring_controller();
void ring_reset();


#endif