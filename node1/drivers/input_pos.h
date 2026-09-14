#include <avr/io.h>
#include <stdint.h>

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t slider;
} pos_t;

void adc_init(void);
uint8_t adc_read(uint8_t channel);
void pos_calibrate(void);
pos_t pos_read(void);
