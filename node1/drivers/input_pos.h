#ifndef input_pos_h
#define input_pos_h

#include <stdint.h>

// Ekstern adresse til MAX156 på XMEM-bussen
#define ADC_BASE_ADDRESS   0x1400

// Antall kanaler MAX156 har (hardwired mode konverterer alle samtidig)
#define ADC_NUM_CHANNELS   4

// Trigger konvertering av alle 4 kanaler.
// I hardwired mode (MODE = GND) er dataverdien som skrives irrelevant;
// det er selve WR-pulsen som starter konverteringen.
void adc_max156_start_conversion(void);

// Leser ut resultatet for kanal 0-3, i rekkefølge, inn i buffer.
// Må kalles minst ADC_CONV_DELAY_US etter adc_max156_start_conversion().
void adc_max156_read_all(uint8_t *buffer);

// Full syklus: start konvertering, vent fast tid, les ut alle kanaler.
// buffer må ha plass til ADC_NUM_CHANNELS (4) bytes.
void adc_max156_convert_and_read(uint8_t *buffer);

#endif // ADC_MAX156_H