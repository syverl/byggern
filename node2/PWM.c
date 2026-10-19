#include "PWM.h"

void PWM_Init(void){
    PMC->PMC_PCER1 = 0x10; //Setter PWM_id til høy (den er 36)

    PIOB->PIO_PDR = PIO_PB13; //periferi styrer pinnen
    PIOB->PIO_ABSR |= PIO_PB13; //skal styre pinnen (periferi B)

    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84); //CLKA = 1 MHz

    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[1].PWM_CPRD = 20000;
    PWM->PWM_CH_NUM[1].PWM_CDTY = 1500;

    PWM->PWM_ENA = PWM_ENA_CHID1;
}

void servo_set_us(uint32_t us) {
    if (us < 900) us = 900;   // sikkerhetsgrense
    if (us > 2100) us = 2100;
    PWM->PWM_CH_NUM[1].PWM_CDTYUPD = us; // oppdateres ved neste periode
}

static int32_t servo_pos = 1500;   // husker posisjonen mellom meldinger og starter i midten

void handle_joystick(CanMsg *msg) {
    int32_t dx = (int32_t)msg->byte[2] - 50;   // -50..50

    if (dx > -5 && dx < 5) dx = 0;              // dødsone rundt midten, trekker fra 50 fra 0-100, effektivt flytter sone fra -50 til 50

    servo_pos += dx / 2;                        //flytter pos med mengde som er prop med utslaget på joystick. pos_ny = pos_gammel + dx/2

    if (servo_pos < 900)  servo_pos = 900;      //legger posisjonen mellom duty cycle 0.9ms og 2.1ms
    if (servo_pos > 2100) servo_pos = 2100;

    servo_set_us(servo_pos); // sender den nye posisjonen til pwm. oppdateres hvert 20_ms periode.
}
