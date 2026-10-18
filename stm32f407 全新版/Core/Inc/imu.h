#ifndef __IMU_H__
#define __IMU_H__

#include "usart.h"/*串口功能头文件*/

#define RXBUFFSIZE 50 /*接收缓冲区，储存imu或串口通信数据*/

extern uint8_t rx_buffer[RXBUFFSIZE];
extern uint8_t imu_data[RXBUFFSIZE];
extern uint8_t imu_flag;
extern float Yaw;

void IMU_Init(void);
void IMU_Updata(void);
void IMU_Proc(void);/*函数声明*/

void ReceiveData(uint8_t RxData);

#endif
