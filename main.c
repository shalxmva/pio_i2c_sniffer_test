#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "pio_i2c_sniffer.h"

#ifdef RUN_LCD_MASTER
extern void core1_entry();
#endif

int main()
{
    stdio_init_all();
#ifdef RUN_LCD_MASTER
    multicore_launch_core1(core1_entry);
#endif
    sleep_ms(3000); // Wait for the system to stabilize

    printf("\033[2J\033[H"); // Print a message on the LCD
    i2c_sniffer_init();
    
    
    while (true) {
        i2c_message_t msg;
        if (i2c_sniffer_get_message(&msg)) {
            i2c_print_message(&msg);
        }
    }
}
