#include "lib/JTAG_UART.h"
#include "lib/Audio.h"
#include "address_map_arm.h"
#include "string.h"
#include "lib/LED.h"
#include "tests/tests.h"

int main(void)
{
    // led_test();
    // uart_test();
    // audio_test();
    hex_test();

    for (;;); 
}
