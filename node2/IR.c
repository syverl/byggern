#include <sam.h>
#include "IR.h"

static uint8_t ir_brutt = 0;

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

void ir_skriv(void) {
    uint16_t ir = adc_read();

    if (!ir_brutt && ir < 1500) { //IR-stråle går lav, verdien har falt under 1,2 V
        ir_brutt = 1; //markerer bruddet
        printf("IR er brutt!\n\r");
    }
    if (ir > 2350) { //IR-ståle går høy igjen, verdien er høyere enn 1,9 V
        ir_brutt = 0; //klar til å registrere nytt brudd
    }
}