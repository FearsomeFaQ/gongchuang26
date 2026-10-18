#ifndef __Move_H__
#define __Move_H__

#include "Motor.h"
#include "Servo.h"
#include "Control.h"

uint8_t Get_Loads(int16_t HuaGui_Speed,uint8_t HuaGui_Acc,uint8_t task_num);
uint8_t Put_Loads(int16_t HuaGui_Speed,uint8_t HuaGui_Acc,uint8_t task_num);
uint8_t last_download(int16_t HuaGui_Speed,uint8_t HuaGui_Acc);

#endif
