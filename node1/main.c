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
#include "drivers/CAN.h"

uint8_t joystick_button(void) {
    return (b.nav & (1 << 0)) ? 1 : 0;
}

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
    Game game = menu_select();
    oled_game_screen(game);
    uint8_t liv = start_liv(game);
    while (1) {
    adc_max156_convert_and_read(num);   // fyller num[0..3]
    io_read_buttons();                  // fyller b.right, b.left, b.nav

    CAN_Message message = {
        .id = 0x10,
        .length = 5,
        .data = { num[0], num[1], num[2], num[3], joystick_button() }
    };

    if (!(CAN_Read(0x30) & (1 << 3))) {   // TXB0 ledig?
        CAN_Send(&message);
    }

    // Node2 sier fra med id 0x30 når IR brytes, data[0] = antall brudd totalt
    if (CAN_Receive(&rx) && rx.id == 0x30) {
        uint8_t brudd = rx.data[0];
        liv = (brudd >= start_liv(game)) ? 0 : start_liv(game) - brudd;
        printf("Brudd: %u  liv: %u\r\n", brudd, liv);
        oled_show_lives(liv);

        if (liv == 0) {
            CAN_Message stopp = { .id = 0x31, .length = 0 };
            CAN_Send(&stopp);             // node2 stopper motoren
            oled_lost();
            while (1) {}                  // spillet er over
        }
    }
    _delay_ms(20);
}
}

