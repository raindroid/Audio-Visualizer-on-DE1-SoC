#include "JTAG_UART.h"
#include "address_map_arm.h"

/*******************************************************************************
 * This program demonstrates use of the JTAG UART port
 *
 * It performs the following:
 *	1. sends a text string to the JTAG UART
 * 	2. reads character data from the JTAG UART
 * 	3. echos the character data back to the JTAG UART
 ******************************************************************************/
int main(void)
{
    /* Declare volatile pointers to I/O registers (volatile means that IO load
       and store instructions will be used to access these pointer locations,
       instead of regular memory loads and stores) */
    volatile int *JTAG_UART_ptr = (int *)JTAG_UART_BASE; // JTAG UART address

    char text_string[] = "\nJTAG UART example code\n> \0";
    char *str, c;

    /* print a text string */
    for (str = text_string; *str != 0; ++str)
        put_jtag(JTAG_UART_ptr, *str);

    /* read and echo characters */
    while (1)
    {
        c = get_jtag(JTAG_UART_ptr);
        if (c != '\0') {
            for (str = text_string; *str != 0; ++str)
                put_jtag(JTAG_UART_ptr, *str);
            put_jtag(JTAG_UART_ptr, c);
        }
    }
}
