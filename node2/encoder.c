#include "encoder.h"


void encoder_init(void){
    //Klokke
    PMC->PMC_PCER1 = (1u << (ID_TC6 - 32)); // Enables the correspondig peripheral clock.

    //Gi PC25 og PC26 til periferi B
    PIOC->PIO_ABSR |= PIO_PC25B_TIOA6 | PIO_PC26B_TIOB6; //Velg B
    PIOC->PIO_PDR |= PIO_PC25B_TIOA6 | PIO_PC26B_TIOB6; //slå på GPIO-styring

    //Kanal 0 teller fra XC0 (dekoderen), capture modus
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0;

    //Skru på kvadratdekodereren, posisjonsmåling, kanter på A og B
    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN | TC_BMR_EDGPHA;
    // Steg 6: Slå på og start telleren
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

int32_t encoder_read(void)
{
    return (int32_t)TC2->TC_CHANNEL[0].TC_CV;   // signert: bakover gir negative tall
}
