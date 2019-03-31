#ifndef _LED_H__
#define _LED_H__

#define VIS_LED_ON 1
#define VIS_LED_OFF 0
#define VIS_LED_SET 1
#define VIS_LED_RESET 0

/**
 * get LED status
 * @param index: from 0 to 9
 * @return: VIS_LED_ON or VIS_LED_OFF
 */
unsigned VIS_LED_Get(unsigned index);

/**
 * set LED status
 * @param index: from 0 to 9
 * @param status: VIS_LED_SET or VIS_LED_RESET
 */
void VIS_LED_Set(unsigned index, unsigned status);

/**
 * set LED status
 * @param index: from 0 to 9
 * @return: current staus
 */
unsigned VIS_LED_Toggle(unsigned index);

void VIS_LED_SetAll();
void VIS_LED_ResetAll();

#endif // !_LED_H__