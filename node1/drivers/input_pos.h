#ifndef input_pos_h
#define input_pos_h
#endif

#include <stdint.h>
#include <avr/io.h>

#ifndef ADC_H
#define ADC_H
#endif

#include <stdint.h>

#define ADC_NUM_CHANNELS 4

void adc_max156_init(void);
void adc_max156_convert_and_read(uint8_t values[ADC_NUM_CHANNELS]);

