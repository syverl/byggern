#include "../config.h"
#include <avr/io.h>
#include <stdio.h>

#define BAUD 9600
#define MYUBRR (F_CPU / 16 / BAUD - 1)


void USART_init(unsigned int ubrr);
void USART_transmit(unsigned int data);
unsigned char USART_Receive(void);
int uart_getchar(FILE *stream);
int uart_putchar(char c, FILE *stream);