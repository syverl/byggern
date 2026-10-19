#include <stdint.h>
#include "sam.h"
#include "can.h"

void PWM_Init(void);
void servo_set_us(uint32_t us);
void handle_joystick(CanMsg *msg);