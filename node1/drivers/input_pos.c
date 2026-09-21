#include "input_pos.h"
#include "SRAM.h"   // xmem_write() / xmem_read() - deres eksisterende XMEM-driver

#ifndef F_CPU
#define F_CPU 4915200UL   // ATmega162 klokkefrekvens - må stemme med resten av prosjektet
#endif


#include <avr/io.h>

#define ADC_ADDR  ((volatile uint8_t *)0x1000)

uint8_t adc_is_busy(void)
{
    return !(PIND & (1 << PD4));   // BUSY er aktiv lav
}

void adc_max156_init(void)
{
    DDRD &= ~(1 << PD4);           // PD4 som inngang
}

void adc_max156_convert_and_read(uint8_t values[ADC_NUM_CHANNELS])
{
    *ADC_ADDR = 0x00;              // starter konvertering
    uint8_t raw[ADC_NUM_CHANNELS];
    while (adc_is_busy());                 // vent til BUSY går høy (ferdig)

    for (uint8_t i = 0; i < ADC_NUM_CHANNELS; i++) {
        raw[i] = *ADC_ADDR;
    }
    values[0] = (uint8_t)100*(raw[0])/255;
    values[1] = (uint8_t)100*(raw[1])/255;
    values[2] = (uint8_t)100*(raw[2]-71)/(242-71);
    values[3] = (uint8_t)100*(raw[3]-71)/(242-71);
}