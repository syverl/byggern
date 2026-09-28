#ifndef input_pos_h
#define input_pos_h


#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "SPI.h"
#include <util/delay.h>
#ifndef ADC_H
#define ADC_H
#endif

#include <stdint.h>

#define ADC_NUM_CHANNELS 4

void adc_max156_init(void);
void adc_max156_convert_and_read(uint8_t values[ADC_NUM_CHANNELS]);
extern volatile uint8_t button_pressed;
void button_init(void);

void io_read_buttons(void);
typedef struct __attribute__((packed)) {
    union {
        uint8_t right;
        struct {
            uint8_t R1:1;
            uint8_t R2:1;
            uint8_t R3:1;
            uint8_t R4:1;
            uint8_t R5:1;
            uint8_t R6:1;
        };
    };
    union {
        uint8_t left;
        struct {
            uint8_t L1:1;
            uint8_t L2:1;
            uint8_t L3:1;
            uint8_t L4:1;
            uint8_t L5:1;
            uint8_t L6:1;
            uint8_t L7:1;
        };
    };
    union {
        uint8_t nav;
        struct {
            uint8_t NB:1;
            uint8_t NR:1;
            uint8_t ND:1;
            uint8_t NL:1;
            uint8_t NU:1;
        };
    };
}Buttons;

extern Buttons b;

#endif