#ifndef __LCD_H__
#define __LCD_H__

#include "Res_pi.h"
#include "Motor.h"

void LCD_Task(uint16_t param1,uint16_t param2);
void LCD_Init(void);
void LCD_float_Task(float float1);

#endif
