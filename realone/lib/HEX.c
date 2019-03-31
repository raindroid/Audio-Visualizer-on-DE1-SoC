#include "HEX.h"
#include "../values.h"
#include "JTAG_UART.h"

static volatile unsigned * HEX3_HEX0_ptr = (unsigned *)HEX3_HEX0_BASE;
static volatile unsigned * HEX5_HEX4_ptr = (unsigned *)HEX5_HEX4_BASE;

static unsigned HEX_HEXs[16] = {
    [0x0] = 0b111111, 0b110, 0b1011011, 0b1001111,
    [0x4] = 0b1100110, 0b1101101, 0b11111101, 0b111,
    [0x8] = 0b1111111, 0b1101111, 0b1110111, 0b1111100, 
    [0xC] = 0b111001, 0b1011110, 0b1111001, 0b1110001
};

void VIS_HEX_Set(unsigned index, unsigned data) {
#ifdef DEBUG
    if (index > 6) VIS_ERROR_Handler();
#endif // DEBUG
    data &= 0x7F;
    if (index < 4) {
        // HEX 0 ~ 3
        unsigned original = *(HEX3_HEX0_ptr);
        // clear the byte
        original &= ~(0xFF << (index << 3));
        *(HEX3_HEX0_ptr) = original | (data << (index << 3));
    } else {
        // HEX 4 ~ 5
        unsigned original = *(HEX5_HEX4_ptr);
        // clear the byte
        original &= ~(0xFF << ((index - 4) << 3));
        *(HEX5_HEX4_ptr) = original | (data << ((index - 4) << 3));
    }
}

void VIS_HEX_SetDigit(unsigned index, unsigned data) {
#ifdef DEBUG
    if (index > 6) VIS_ERROR_Handler();
    if (data > 0xF) VIS_ERROR_Handler();
#endif // DEBUG
    VIS_HEX_Set(index, HEX_HEXs[data]);
}

void VIS_HEX_SetUint_HEX(unsigned data) {
    data &= 0xFFFFFF;
    for (unsigned i = 0; i <= 5; i++) {
        VIS_HEX_SetDigit(i, data & (0xF));
        data >>= 4;
    }
}

void VIS_HEX_SetUint_DEC(unsigned data) {
    data %= 1000000;
    for (unsigned i = 0; i <= 5; i++) {
        VIS_HEX_SetDigit(i, data %10);
        data /= 10;
    }
}