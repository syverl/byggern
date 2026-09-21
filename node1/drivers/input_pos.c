#include "input_pos.h"
#include "SRAM.h"   // xmem_write() / xmem_read() - deres eksisterende XMEM-driver

#ifndef F_CPU
#define F_CPU 4915200UL   // ATmega162 klokkefrekvens - må stemme med resten av prosjektet
#endif

#include <util/delay.h>

// Beregnet konverteringstid: t_CONV = (9 * N * 2) / f_CLK
// Med N = 4 kanaler og f_CLK = 2.4576 MHz gir dette ca. 29.3 us.
// Vi legger på margin for klokkeusikkerhet og tWBD/tBRD-forsinkelser.
#define ADC_CONV_DELAY_US  35

void adc_max156_start_conversion(void)
{
    xmem_write(0x00, ADC_BASE_ADDRESS);
}

void adc_max156_read_all(uint8_t *buffer)
{
    for (uint8_t i = 0; i < ADC_NUM_CHANNELS; i++) {
        buffer[i] = xmem_read(ADC_BASE_ADDRESS);
    }
}

void adc_max156_convert_and_read(uint8_t *buffer)
{
    adc_max156_start_conversion();
    _delay_us(ADC_CONV_DELAY_US);
    adc_max156_read_all(buffer);
}