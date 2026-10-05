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

// int main(void)
// {
//     USART_init(MYUBRR);   //Kommunikasjon med UART mellom RS og mikrokontroller
//     fdevopen(uart_putchar, uart_getchar); //Sender og mottar

//     xmem_init();   // Ekstern minne
//     pwm_init();    // Klokke
//     SPI_Init();
//     oled_init();                    // først: nullstill og skru på
//     oled_home();
//     adc_max156_init(); //PD4 som inngang. Lese fra BUSY
//     button_init();
//     uint8_t adc_values[ADC_NUM_CHANNELS];  //De fire analoge kanalene
//     while (1) {
//         uint8_t valg = menu_select();
//         printf("valgt: %d\r\n", valg+1);
//         io_read_buttons();
//         printf("right: %02X  left: %02X  nav: %02X\r\n", b.right, b.left, b.nav);
//     }

// }
#include "drivers/CAN.h"

int main(void)
{
    USART_init(MYUBRR);
    fdevopen(uart_putchar, uart_getchar);
    SPI_Init();
    CAN_init();
    xmem_init();   // Ekstern minne
    pwm_init();    // Klokke
    oled_init();                    // først: nullstill og skru på
    adc_max156_init(); //PD4 som inngang. Lese fra BUSY
    button_init();
    oled_home();
    _delay_ms(10000);
    CAN_test();
    while (1) { }
}
