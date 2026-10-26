#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "time.h"
#include "uart.h"
#include "can.h"
#include "PWM.h"
#include "IR.h"
/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"
#define BAUD 9600
#define F_CPU 84000000
int main()
{
    SystemInit();
    PWM_Init();
    WDT->WDT_MR = WDT_MR_WDDIS;
    uart_init(F_CPU, BAUD);
    printf("Node 2 startet\n\r");
    time_spinFor(msecs(5000));
    can_init((CanInit){ .phase2 = 3, .phase1 = 7, .propag = 2, .sjw = 0, .brp = 20 }, 0);
    printf("CAN_BR = 0x%08lX\n\r", CAN0->CAN_BR);
    servo_set_us(1500);
    adc_init();
    printf("ADC klar\n\r");    
    CanMsg m;
    uint8_t ir_brutt = 0;   
    while (1) {
    if (can_rx(&m)) {
        if (m.id == 0x10) {
            handle_joystick(&m);
        }
    }
    ir_skriv();
}
   
}