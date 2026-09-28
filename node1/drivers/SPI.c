#include "SPI.h"
#include <stdio.h>
#include <stdint.h>
#include "../config.h"
#include <avr/io.h>




void SPI_Init(void)
{
    //OUTPUTS
    DDRB |=(1<<PB5); //MOSI
    DDRB |=(1<<PB7); //SCK
    DDRB  |= (1<<SS1) | (1<<SS2);   // utganger
    DDRB |=(1<<PB1); // D/C
    DDRB |= (1<<PB4);   // SS som utgang, bare for at SPI skal forbli master

    //INPUTS
    DDRB &= ~(1<<PB6); // MISO
    PORTB |= (1<<SS1) | (1<<SS2);   // begge høye = ingen slave valgt
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_Transmit(char data, uint8_t pin)
{
    PORTB &= ~(1<<pin); 
/* Start transmission */
    SPDR = data;
/* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

     PORTB |=  (1<<pin); 
}

uint8_t SPI_Read(uint8_t pin)
{
    PORTB &= ~(1<<pin); 
    SPDR = 0x00;                                // dummy ut, genererer SCK
    while (!(SPSR & (1<<SPIF)));                // vent til ferdig
    PORTB |=  (1<<pin);
    return SPDR;                                // byten som kom inn på MISO
}

uint8_t SPI_transfer(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}