#include "config.h"
#include <avr/io.h>
#include <util/delay.h>

#define SQUARE_PIN PA2

// int main(void)
// {
//     DDRA |= (1 << PA2);

//     PORTA |= (1 << PA2);

//     while (1)
//     {
//     }
// }


int main(void) {
    DDRA |= (1<<SQUARE_PIN);

    while (1){
        PORTA |= (1<<SQUARE_PIN);
        _delay_ms(10);
        PORTA &= ~(1<<SQUARE_PIN);
        _delay_ms(10);
    }
}