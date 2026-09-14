#include "../config.h"
#include "input_pos.h"
#include <util/delay.h>

#define ADC_ADDR 0x1400

static volatile uint8_t *adc = (uint8_t *)ADC_ADDR;


// --------------------------------------------------
// Initialize external memory interface for ADC
// --------------------------------------------------
void adc_init(void)
{
    // Enable external memory interface
    MCUCR |= (1 << SRE);

    // Wait states
    MCUCR  |= (1 << SRW10);
    EMCUCR |= (1 << SRW11);

    // Release required PORTC pins
    SFIOR |= (1 << XMM2);
}


// --------------------------------------------------
// Read one ADC channel
// --------------------------------------------------
uint8_t adc_read(uint8_t channel)
{
    // Start conversion
    *adc = 0x04;

    // Wait until conversion is finished
    _delay_us(200);

    // MAX156 returns channels sequentially.
    // Read and discard values until desired channel.
    uint8_t value = 0;

    for (uint8_t i = 0; i <= channel; i++)
    {
        value = *adc;
    }

    return value;
}


// --------------------------------------------------
// Calibrate joystick
// --------------------------------------------------
void pos_calibrate(void)
{
    // Leave empty for now.
    // Can later store center/min/max values.
}


// --------------------------------------------------
// Read all joystick/slider positions
// --------------------------------------------------
pos_t pos_read(void)
{
    pos_t pos;

    // Start one conversion
    *adc = 0xAA;
    _delay_us(200);

    // Read sequential ADC results
    pos.x      = *adc;
    pos.y      = *adc;
    pos.slider = *adc;

    return pos;
}