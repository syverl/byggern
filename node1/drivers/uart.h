#include <config.h>
#include <avr/io.h>

#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void USART_init(unsigned int ubrr);
void USART_transmit(unsigned int data);
unsigned char USART_Receive(void);