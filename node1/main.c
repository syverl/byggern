#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "drivers/uart.h"
#include "drivers/pwm_clock.h"
#include "drivers/input_pos.h"
#include "drivers/SRAM.h"



int main(void)
{
    USART_init(MYUBRR);
    fdevopen(uart_putchar, uart_getchar);

    xmem_init();   // External memory interface
    pwm_init();    // Clock to MAX156
    volatile uint8_t *adc = (uint8_t *)0x1400;

    while (1)
    {
        *adc = 0xAA;
    }
}

