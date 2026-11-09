#include "game.h"



void game_play(void){
    SystemInit();
    PWM_Init();
    WDT->WDT_MR = WDT_MR_WDDIS;
    uart_init(F_CPU, BAUD);
    printf("Node 2 startet\n\r");
    motor_init();   
    encoder_init();
    time_spinFor(msecs(5000));
    can_init((CanInit){ .phase2 = 3, .phase1 = 7, .propag = 2, .sjw = 0, .brp = 20 }, 0);
    printf("CAN_BR = 0x%08lX\n\r", CAN0->CAN_BR);
    servo_set_us(1500);
    adc_init();
    printf("ADC klar\n\r");    
    CanMsg m;
    uint8_t ir_brutt = 0;
    solenoid_init();
    uint8_t forrige = 0;                // antall brudd sist vi sa fra til node1
    while (1) {
    if (can_rx(&m)) {
        if (m.id == 0x10) {
            handle_joystick_vel(&m);
        } else if (m.id == 0x31) {      // game over fra node1
            motor_set(0);
            ir_brutt = 0;
            forrige = 0;
        }
    }
    ir_brutt = ir_skriv(ir_brutt);
    if (ir_brutt != forrige) {          // nytt brudd -> si fra til node1 (den teller liv)
        forrige = ir_brutt;
        can_tx((CanMsg){ .id = 0x30, .length = 1, .byte = { ir_brutt } });
    }
    }
}

   
