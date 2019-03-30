#include "LED.h"
#include "../values.h"

static volatile unsigned * red_LED_ptr = (unsigned *)0xFF200000;

unsigned VIS_LED_Get(unsigned index) {
#ifdef DEBUG
    if (index < 0 || index > 9) VIS_ERROR_Handler();    
#endif // DEBUG
    unsigned led = *(red_LED_ptr);
    return led & (0x1u << index);
}

void VIS_LED_Set(unsigned index, unsigned status) {
#ifdef DEBUG
    if (index < 0 || index > 9) VIS_ERROR_Handler();    
    if (status != VIS_LED_SET && status != VIS_LED_RESET) VIS_ERROR_Handler();
#endif // DEBUG
    unsigned led = *(red_LED_ptr);
    if (status == VIS_LED_SET) {
        led |= 0x1u << index;
    } else {
        led &= ~(0x1u << index);
    }
    *(red_LED_ptr) = led;
}

unsigned VIS_LED_Toggle(unsigned index) {
#ifdef DEBUG
    if (index < 0 || index > 9) VIS_ERROR_Handler();    
#endif // DEBUG
    unsigned led = *(red_LED_ptr);
    led ^= 0x1u << index;
    *(red_LED_ptr) = led;
    return led & (0x1u << index);
}