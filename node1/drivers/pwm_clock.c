#include "pwm_clock.h"


void pwm_init(void){
    DDRD |=(1<<PD5); //Set PD4 as output
    OCR1A = 0;
    TCCR1A = (1<<COM1A0) | (1<<WGM11) | (1<<WGM10); //Set toggle on compare match and set fast PWM mode
    TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS10); //Set fast PWM mode
}