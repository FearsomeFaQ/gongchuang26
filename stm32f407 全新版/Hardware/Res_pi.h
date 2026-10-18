#ifndef __Res_pi_H__
#define __Res_pi_H__

#include "main.h"
#include "usart.h"

extern uint8_t uart4_Rxbuff;
extern uint8_t RxFlag;
extern uint8_t PI_Data0;
extern int16_t PI_Data1;
extern int16_t PI_Data2;
extern uint8_t PI_RawData0[2];        //´æ´¢task_id×Ö·û
extern uint8_t PI_RawData1[4];        //´æ´¢param1×Ö·û
extern uint8_t PI_RawData2[4]; 

void PI_Init(void);
void PI_Receive_Data(uint8_t com_data);
void PI_Receive_Proc(void);
uint8_t char_2_num(uint8_t char_temp);
void GPIO_Check_State(void);

#endif
