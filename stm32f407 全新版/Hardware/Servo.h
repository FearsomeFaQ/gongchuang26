#ifndef __Servo_H__
#define __Servo_H__

#include "tim.h"
#include "Motor.h"

typedef enum
{
	Servo_JiaZhua = 1,
	Servo_ZaiWu,
	Servo_HuaTai,
	Servo_Turn
}Servo_N;

#define JiaZhua_Open  50
#define JiaZhua_Close 125

#define duoji_chang 1500
#define duoji_duan 1000



void Servo_Init(void);
void Servo_SetAngel1(uint16_t angle);
void Servo_SetAngel2(uint16_t angle);
void Servo_Callback(void);
float Caculate_Angle(uint8_t Last_Angle,uint8_t Target_Angle,uint16_t tf,uint16_t t);
uint8_t Servo_Ctrl_JiaZhua(uint8_t state);

#endif
