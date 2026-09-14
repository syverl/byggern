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

    xmem_init();   // Enable external memory interface (XMEM) - MÅ være på
    pwm_init();    // Clock til MAX156

    uint8_t adc_values[ADC_NUM_CHANNELS];

    while (1)
    {
        adc_max156_convert_and_read(adc_values);

        // Eksempel: send verdiene over UART for debugging
        printf("CH0=%d CH1=%d CH2=%d CH3=%d\n",
                adc_values[0], adc_values[1], adc_values[2], adc_values[3]);

        _delay_ms(200);
    }
}