#include <stdint.h>
#include "sam.h"
#include "can.h"
#include "solenoid.h"
#include "encoder.h"
void PWM_Init(void);
void servo_set_us(uint32_t us);
void handle_joystick(CanMsg *msg);
void handle_joystick_vel(CanMsg *msg);
void motor_init(void);
void motor_set(int32_t speed);