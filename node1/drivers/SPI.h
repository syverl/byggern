#include <avr/io.h>
#define SS1  PB2  // OLED
#define SS2  PB3  // I/O-kort

void SPI_Init(void);
void SPI_Transmit(char data, uint8_t pin);
char SPI_MasterRead(uint8_t pin);
uint8_t SPI_transfer(uint8_t data);
// void SPI_Transmit(char data, uint8_t pin, uint8_t n);
// char SPI_MasterRead(uint8_t pin, );