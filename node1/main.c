#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "drivers/uart.h"
#include "drivers/pwm_clock.h"
#include "drivers/input_pos.h"
#include "drivers/SRAM.h"
#include "drivers/SPI.h"
#include "drivers/oled.h"
#include "drivers/fonts.h"

int main(void)
{
    USART_init(MYUBRR);   //Kommunikasjon med UART mellom RS og mikrokontroller
    fdevopen(uart_putchar, uart_getchar); //Sender og mottar

    xmem_init();   // Ekstern minne
    pwm_init();    // Klokke
    SPI_Init();
    oled_init();                    // først: nullstill og skru på
    oled_print(2, 20, "Hvor er du ");
    oled_print(3, 40, "Syver?");
    oled_update();
    while (1);                  // hold programmet i gang
    // adc_max156_init(); //PD4 som inngang. Lese fra BUSY

    // uint8_t adc_values[ADC_NUM_CHANNELS];  //De fire analoge kanalene

    // while (1)
    // {
    //     // SRAM_test();
    //     _delay_ms(1000);
    //     adc_max156_convert_and_read(adc_values); //Lese verdiene

    //     // Eksempel: send verdiene over UART for debugging
    //     printf("CH0=%d CH1=%d CH2=%d CH3=%d\n",
    //             adc_values[0], adc_values[1], adc_values[2], adc_values[3]);

    // }
}