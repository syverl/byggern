#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "SPI.h"
#include <util/delay.h>

#define CS_CAN  PB4  //CAN CS

void CAN_reset(void);

void CAN_init(void);

uint8_t CAN_Read(uint8_t address);

void CAN_Write(uint8_t data, uint8_t address);

void CAN_RTS(uint8_t nnn);

uint8_t CAN_Read_Status(void);

void CAN_Bit_Modify(uint8_t address, uint8_t data, uint8_t maske);

void CAN_Controller_Init(void);

typedef struct {
    uint16_t id;
    uint8_t  length;
    uint8_t  data[8];
} CAN_Message;

void CAN_Send(CAN_Message *msg);
uint8_t CAN_Receive(CAN_Message *msg);
void CAN_test(void);