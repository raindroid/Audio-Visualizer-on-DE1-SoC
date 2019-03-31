#ifndef _HEX_H__
#define _HEX_H__

void VIS_HEX_Set(unsigned index, unsigned data);

/**
 * set a single digit to display, support hex
 */
void VIS_HEX_SetDigit(unsigned index, unsigned data);

/** 
 * set a number to display, only support 0 - 0xFFFFFF
 */
void VIS_HEX_SetUint_HEX(unsigned data);
void VIS_HEX_SetUint_DEC(unsigned data);

#endif