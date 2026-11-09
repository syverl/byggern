#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "sam.h"
#include "time.h"

void adc_init(void);
uint16_t adc_read(void);
uint8_t ir_skriv(uint8_t ir_brutt);

#endif