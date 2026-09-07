#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include "drivers/uart.h"

#define SQUARE_PIN PA3


int main(void) {
    USART_init(MYUBRR);
    while (1){
        USART_transmit('A');
        _delay_ms(10);
    }
}

