#include "CAN.h"

void CAN_reset(void){
    SPI_Transmit(0xC0, CS_CAN);
    _delay_ms(0.5); // 128 OSC1 clock cycles after power-up, det vil si 26 mikro secund. Ish 30, da holder 0.5 milli sekund
}

void CAN_init(void){

    DDRB  |= (1 << CS_CAN);   // Utgnagn CS
    PORTB |= (1<<CS_CAN);
    CAN_reset();
}

uint8_t CAN_Read(uint8_t address){
    PORTB &= ~(1<<CS_CAN);

    SPDR = 0x03;                                
    while (!(SPSR & (1<<SPIF))); 

    SPDR = address;  
    while (!(SPSR & (1<<SPIF)));

    SPDR = 0x00;                        // dummy-byte, klokker ut data
    while (!(SPSR & (1 << SPIF)));

    PORTB |=  (1<<CS_CAN);
    return SPDR;                                
}

void CAN_Write(uint8_t data, uint8_t address){
    PORTB &= ~(1<<CS_CAN);
    SPDR = 0x02; // Starter write
    while (!(SPSR & (1<<SPIF))); 

    SPDR = address;  // Hvilken adresse som skal skrives til
    while (!(SPSR & (1<<SPIF)));

    SPDR = data;
    while (!(SPSR & (1<<SPIF)));

    PORTB |=  (1<<CS_CAN);
}

void CAN_RTS(uint8_t nnn){
    PORTB &= ~(1<<CS_CAN);
    SPDR = 0x80 | (nnn & 0x07); // Starter RTS
    while (!(SPSR & (1<<SPIF))); 

    PORTB |=  (1<<CS_CAN);
}

uint8_t CAN_Read_Status(void){
    PORTB &= ~(1<<CS_CAN);
    
    SPDR = 0xA0;
    while (!(SPSR & (1<<SPIF))); 
    
    SPDR = 0x00;
    while (!(SPSR & (1<<SPIF))); 

    PORTB |=  (1<<CS_CAN);
    return SPDR;
}

void CAN_Bit_Modify(uint8_t address, uint8_t data, uint8_t maske){
    PORTB &= ~(1<<CS_CAN);
    
    SPDR = 0x05;
    while (!(SPSR & (1<<SPIF))); 
    
    SPDR = address;
    while (!(SPSR & (1<<SPIF))); 

    SPDR = maske;
    while (!(SPSR & (1<<SPIF))); 

    SPDR = data;
    while (!(SPSR & (1<<SPIF))); 

    PORTB |=  (1<<CS_CAN);
}

void CAN_Controller_Init(void){
    CAN_init();
    CAN_Bit_Modify(0x60, 0x60, 0x60);        // RXB0: ta imot alt
    CAN_Bit_Modify(0x0F, 0x40, 0xE0);        // loopback

}

void CAN_Send(CAN_Message *msg){
    // vent til forrige melding er sendt (TXREQ = bit 3 i TXB0CTRL)
    while (CAN_Read(0x30) & (1 << 3));

    CAN_Write(msg->id >> 3, 0x31);             // TXB0SIDH: ID bit 10-3
    CAN_Write((msg->id & 0x07) << 5, 0x32);    // TXB0SIDL: ID bit 2-0, EXIDE = 0
    CAN_Write(msg->length & 0x0F, 0x35);       // TXB0DLC: lengde

    for (uint8_t i = 0; i < msg->length; i++) {
        CAN_Write(msg->data[i], 0x36 + i);     // TXB0D0 ... TXB0D7
    }

    CAN_RTS(1);                                // send fra TXB0
}

uint8_t CAN_Receive(CAN_Message *msg){
  // RX0IF = bit 0 i CANINTF: 1 betyr at RXB0 har en ny melding
    if (!(CAN_Read(0x2C) & (1 << 0))) {
        return 0;                                  // ingen melding
    }

    uint8_t sidh = CAN_Read(0x61);                 // RXB0SIDH: ID bit 10-3
    uint8_t sidl = CAN_Read(0x62);                 // RXB0SIDL: ID bit 2-0 i bit 7-5
    msg->id = ((uint16_t)sidh << 3) | (sidl >> 5);

    msg->length = CAN_Read(0x65) & 0x0F;           // RXB0DLC: lengde
    if (msg->length > 8) msg->length = 8;

    for (uint8_t i = 0; i < msg->length; i++) {
        msg->data[i] = CAN_Read(0x66 + i);         // RXB0D0 ... RXB0D7
    }

    CAN_Bit_Modify(0x2C, 0x00, 0x01);              // nullstill RX0IF
    return 1;                                      // melding mottatt
}



void CAN_test(void)
{
    CAN_Controller_Init();

    CAN_Message tx[3] = {
        { .id = 0x123, .length = 2, .data = {0xAB, 0xCD} },
        { .id = 0x7FF, .length = 8, .data = {1, 2, 3, 4, 5, 6, 7, 8} },
        { .id = 0x001, .length = 1, .data = {0x55} }
    };

    for (uint8_t n = 0; n < 3; n++) {
        CAN_Send(&tx[n]);
        _delay_ms(10);

        CAN_Message rx;
        if (!CAN_Receive(&rx)) {
            printf("Melding %d: ingenting mottatt\r\n", n);
            continue;
        }

        uint8_t ok = (rx.id == tx[n].id) && (rx.length == tx[n].length);
        for (uint8_t i = 0; i < rx.length; i++) {
            if (rx.data[i] != tx[n].data[i]) ok = 0;
        }

        printf("Melding %d: ID %03X  len %d  %s\r\n",
               n, rx.id, rx.length, ok ? "OK" : "FEIL");
    }
}