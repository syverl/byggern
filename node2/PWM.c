#include "PWM.h"

#define MOTOR_PWM_FREQ   20000u                          // 20 kHz, over hørbart område
#define MOTOR_CPRD       (84000000u / MOTOR_PWM_FREQ)    // = 4200 ticks per periode
#define MOTOR_MAX_DUTY   100 
#define REF_MIN  -2900.0f
#define REF_MAX   2900.0f
#define T  0.02f  //Siden vi sender hvert 20ms
static float referanse = 0;
static float sum_e = 0;
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
    // printf("raw: %u  dx: %ld\r\n", msg->byte[2], (long)dx);

    if (dx > -5 && dx < 5) dx = 0;              // dødsone rundt midten, trekker fra 50 fra 0-100, effektivt flytter sone fra -50 til 50

    servo_pos += dx / 2;                        //flytter pos med mengde som er prop med utslaget på joystick. pos_ny = pos_gammel + dx/2

    if (servo_pos < 900)  servo_pos = 900;      //legger posisjonen mellom duty cycle 0.9ms og 2.1ms
    if (servo_pos > 2100) servo_pos = 2100;

    servo_set_us(servo_pos); // sender den nye posisjonen til pwm. oppdateres hvert 20_ms periode.
}
void handle_joystick_vel(CanMsg *msg) {
    int32_t dx = (int32_t)msg->byte[2] - 50;   // -50..50
    int32_t dx2 = (int32_t)msg->byte[3] - 50;   // -50..50

    if (dx > -5 && dx < 5) dx = 0;              // dødsone rundt midten, trekker fra 50 fra 0-100, effektivt flytter sone fra -50 til 50

    servo_pos = 12*dx+1500;                        //flytter pos med mengde som er prop med utslaget på joystick. pos_ny = pos_gammel + dx/2

    if (servo_pos < 900)  servo_pos = 900;      //legger posisjonen mellom duty cycle 0.9ms og 2.1ms
    if (servo_pos > 2100) servo_pos = 2100;

    servo_set_us(servo_pos); // sender den nye posisjonen til pwm. oppdateres hvert 20_ms periode.

    if (dx2 > -5 && dx2 < 5) dx2 = 0;

    // 2) Raskere referanse: /10 var altfor tregt (20+ s over banen)
    referanse += (float)(-dx2) * 3.0f;      //

    // 3) Hold referansen innenfor banen (sett grensene til det du har målt)
    if (referanse < REF_MIN) referanse = REF_MIN;
    if (referanse > REF_MAX) referanse = REF_MAX;

    static float y_prev = 0.0f;              // husker forrige posisjon
    float y    = (float)encoder_read();
    float fart = (y - y_prev) / T;           // tellinger per sekund
    y_prev = y;

    float e = referanse - y;

    float K_p = 0.55f;
    float K_i = 0.045f;
    float K_d = 0.02f;                      // NYTT: demping, juster denne

    float u = K_p * e + K_i * sum_e - K_d * fart;   // NYTT: - K_d * fart

    // 5) Friksjonskompensasjon
    if (e >  30.0f) u += 10.0f;
    if (e < -30.0f) u -= 10.0f;

    // 6) Anti-windup
    if (u < 100.0f && u > -100.0f) {
        sum_e += e * T;
    }
    if (sum_e >  500.0f) sum_e =  500.0f;    // NYTT: tak på integralet
    if (sum_e < -500.0f) sum_e = -500.0f;

    // 7) Skriv ut sjeldnere, så printf ikke forsinker regulatoren
    // static uint8_t cnt = 0;
    // if (++cnt >= 10) {
    //     cnt = 0;
    //     // printf("u: %d  ref: %ld  enc: %ld\r\n", (int)u, (long)referanse, (long)encoder_read());
    // }

    motor_set((int32_t)u);
    solenoid_update(!msg->byte[4]);
    // static uint8_t prev = 0;
    // if (msg->byte[4] != prev) {
    //     // printf("btn: %u\r\n", msg->byte[4]);
    //     prev = msg->byte[4];
    // }
}


void motor_set(int32_t speed)
{
    if (speed >  MOTOR_MAX_DUTY) speed =  MOTOR_MAX_DUTY;
    if (speed < -MOTOR_MAX_DUTY) speed = -MOTOR_MAX_DUTY;

    if (speed >= 0) {
        PIOC->PIO_CODR = PIO_PC23;    // én retning
    } else {
        PIOC->PIO_SODR  = PIO_PC23;    // andre retning
        speed = -speed;
    } 

    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = (uint32_t)speed * MOTOR_CPRD / 100;
}
void motor_init(void){
    PMC->PMC_PCER1 = (1u << (36 - 32));
     // PB12 -> PWMH0 (periferi B)
    PIOB->PIO_ABSR |= PIO_PB12;
    PIOB->PIO_PDR   = PIO_PB12;

    // PC23 -> DIR, vanlig GPIO-utgang
    PIOC->PIO_PER = PIO_PC23;    // PIO styrer pinnen
    PIOC->PIO_OER = PIO_PC23;    // utgang
    PIOC->PIO_CODR = PIO_PC23;   // start lav

    // PWM kanal 0: MCK som klokke, CPOL slik at CDTY = høy-tid
    PWM->PWM_CH_NUM[0].PWM_CMR  = PWM_CMR_CPRE_MCK | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[0].PWM_CPRD = MOTOR_CPRD;
    PWM->PWM_CH_NUM[0].PWM_CDTY = 0;                     // stillestående ved start

    PWM->PWM_ENA = PWM_ENA_CHID0;                        // slå på kanal 0
}


    // PMC->PMC_PCER0 |= (1u << ID_PIOB);
    // PIOB->PIO_SODR = SOL_PIN;     // høy = AV (aktiv lav). Sett før den blir utgang