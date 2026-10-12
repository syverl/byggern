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
    CAN_Controller_Init();          // reset + CNF1-3 + normal-modus
    _delay_ms(10000);
    printf("CANSTAT=%02X CNF=%02X %02X %02X\r\n",
       CAN_Read(0x0E), CAN_Read(0x2A), CAN_Read(0x29), CAN_Read(0x28));
    CAN_Message tx = { .id = 0x10, .length = 2, .data = {0xAB, 0xCD} };
    CAN_Message rx;
    uint8_t num[4];
    
    while (1) {
        adc_max156_convert_and_read(num);           // fyller num[0..3]

        CAN_Message message = {
            .id = 0x10,
            .length = 4,
            .data = { num[0], num[1], num[2], num[3] }
        };

        if (!(CAN_Read(0x30) & (1 << 3))) {         // TXB0 ledig?
            CAN_Send(&message);
        }
        _delay_ms(1000);
}
}

