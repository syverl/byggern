#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include "drivers/uart.h"

#define SQUARE_PIN PA3
#include <stdlib.h>
void SRAM_test(void)
{
    volatile char *ext_ram = (char *) 0x1800;  // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting SRAM test...\n");
    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();
    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n",
                   i, retreived_value, some_value);
            write_errors++;
        }
    }
    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed);
    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n",
                   i, retreived_value, some_value);
            retrieval_errors++;
        }
    }
    printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n",
           write_errors, retrieval_errors);
}
void xmem_init(void) {
    MCUCR |= (1 << SRE);       // enable XMEM
    SFIOR |= (1 << XMM2);      // sett riktig maskeverdi
}

int main(void) {
     USART_init(MYUBRR);
    fdevopen(uart_putchar, uart_getchar);
    while(1){
    printf("Test 1 - hello\n");

    // 2. Sett opp adresseporter og XMEM FØR testen kjøres
    DDRC |= 0x0F;
    xmem_init();   // <-- setter SRE + riktig XMM-maske, MÅ skje før SRAM brukes
    printf("Test 2 - after SRE\n");

    // 3. Kjør testen
    SRAM_test();
}
}


// DDRA = 0xFF;           // PA0-7 som utgang (databuss)
//     DDRE |= (1 << PE1);    // ALE som utgang

//     USART_init(MYUBRR);
//     fdevopen(uart_putchar, uart_getchar);

//     // Legg adressebyten ut på AD7:0 (samme buss som brukes til data)
//     PORTE |= (1 << PE1);   // ALE høy -> D-vippen er transparent
//     PORTA = 0x04;           // legg data/adresse på bussen

//     _delay_ms(1000);

//     // Sett ALE lav — vippen LÅSER nå adressen som lå der akkurat i det øyeblikket
//     PORTE &= ~(1 << PE1);
//     PORTA = 0x08;           // endre bussen etter latch (Q skal IKKE følge denne lenger)

//     while (1) {}  