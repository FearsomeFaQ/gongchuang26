#ifndef __Control_H__
#define __Control_H__

#include "main.h"
#include "Res_pi.h"
#include "Motor.h"
#include "Move.h"
#include "LCD.h"



extern uint8_t Running_Flag;
extern uint8_t Car_Counter_Enable;
extern uint16_t Car_Counter_Times;
extern uint16_t Car_Counter;

void Control_Proc(void);

#endif

