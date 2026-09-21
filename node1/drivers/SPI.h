#include <avr/io.h>
#define SS1  PB2
#define SS2  PB3

void SPI_Init(void);
void SPI_Transmit(char data, uint8_t pin);
char SPI_MasterRead(uint8_t pin);
// void SPI_Transmit(char data, uint8_t pin, uint8_t n);
// char SPI_MasterRead(uint8_t pin, );