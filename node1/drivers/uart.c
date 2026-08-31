#include <config.h>
#include <uart.h>
#include <avr/io.h>

//Alle funksjonene under er laget i henhold til eksempelkode som er levert
//i databladet til ATmega162.


void USART_init(unsigned int ubrr){
/* Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;

UCSR0B = (1<<RXEN0)|(1<<TXEN0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}


void USART_transmit(unsigned int data){
/* Wait for empty transmit buffer */
while ( !( UCSR0A & (1<<UDRE0)) );
/* Put data into buffer, sends the data */
UDR0 = data;
}


unsigned char USART_Receive(void){
/* Wait for data to be received */
while ( !(UCSR0A & (1<<RXC0)) )
;
/* Get and return received data from buffer */
return UDR0;
}
