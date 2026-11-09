#include "solenoid.h"
#define SOL_PIN        PIO_PB20     // A11
#define SOL_PULSE_MS 80
#define TICKS_PER_S  84000000ULL 

void solenoid_init(void)
{
    PMC->PMC_PCER0 |= (1u << ID_PIOB);
    PIOB->PIO_SODR = SOL_PIN;     // høy = AV (aktiv lav). Sett før den blir utgang
    PIOB->PIO_PER  = SOL_PIN;     // PIO styrer pinnen
    PIOB->PIO_OER  = SOL_PIN;     // utgang
}

// Kalles hver gang en joystick-melding kommer
void solenoid_update(uint8_t btn)
{
    static uint8_t  prev_btn = 0;
    static uint8_t  active   = 0;
    static uint64_t t_start  = 0;

    if (btn && !prev_btn && !active) {          // nytt trykk: skyt én gang
        PIOB->PIO_CODR = SOL_PIN;               // ,

        t_start = time_now();
        active  = 1;
    }
    prev_btn = btn;

    if (active && (time_now() - t_start) > (TICKS_PER_S * SOL_PULSE_MS / 1000)) {
        PIOB->PIO_SODR = SOL_PIN;               // av igjen
        active = 0;
    }
}