#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "sam.h"

void adc_init(void);
uint16_t adc_read(void);
void ir_skriv(void);

#endif