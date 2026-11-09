#include <sam.h>
#include "IR.h"

#define IR_TERSKEL  1500
#define IR_LAV_TID  msecs(30)    
#define DODTID      seconds(3) 

static uint8_t ir_brutt = 0;
static uint64_t last_time = 0;

void adc_init(void) {
    PMC->PMC_PCER1 |= PMC_PCER1_PID37; // Skru på ADC klokke
    PMC->PMC_PCER0 |= (1u << ID_PIOA);

    PIOA->PIO_PDR = PIO_PDR_P16;   // skrur av PIO, gir pinne til adc
    PIOA->PIO_ODR = PIO_ODR_P16;   // input

    ADC->ADC_CHDR = 0xFFFF; // skrur av alle kanaler
    ADC->ADC_CHER = ADC_CHER_CH7; // Skrur på ADC kanal 7 / A0

    ADC->ADC_MR = ADC_MR_FREERUN_ON | ADC_MR_PRESCAL(10)| ADC_MR_STARTUP_SUT64; // lese kontinuerlig

    ADC->ADC_CR = ADC_CR_START; // starter ADC
    while (!(ADC->ADC_ISR & ADC_ISR_EOC7));  // vent på første gyldige måling
}

uint16_t adc_read(void) {
    return ADC->ADC_CDR[7];    // siste måling fra kanal 7 
}

uint8_t ir_skriv(uint8_t ir_brutt) {
    static uint64_t lav_siden = 0;          // 0 = strålen er hel
    uint16_t ir   = adc_read();
    uint64_t time = time_now();

    if (ir >= IR_TERSKEL) {                 // hel stråle -> nullstill
        lav_siden = 0;
        return ir_brutt;
    }
    if (lav_siden == 0) {                   // akkurat blitt lav, start klokka
        lav_siden = time;
        return ir_brutt;
    }
    if (time - lav_siden > IR_LAV_TID && time - last_time > DODTID) {
        ir_brutt++;
        last_time = time;
        printf("IR brutt: %d\n\r", ir_brutt);
    }
    return ir_brutt;
}