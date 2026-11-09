#define BAUD 9600
#define F_CPU 84000000
#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "time.h"
#include "uart.h"
#include "can.h"
#include "PWM.h"
#include "IR.h"
#include "encoder.h"
#include "solenoid.h"

void game_play(void);