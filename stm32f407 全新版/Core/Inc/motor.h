#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"
#include "usart.h"
#include "math.h"
#include "imu.h"
#include "pid.h"

extern uint8_t uart1_Rxbuff;
extern uint8_t Car_Stop_Flag;
extern uint16_t RxBuffer1[10];
extern uint8_t MotorStatus[6];
extern float motor_angle; 
extern float motor_speed; 
extern float TARGET_YAW;

void Car_Go_leri( float Tar_Y, float Speed, uint16_t Car_ACC);
void Motor_Clear_All(void);
void Clear_RxBuffer1(void);
void Motor_Init(void);
void Motor_Enable(uint8_t Motor_num);
void Motor_Disbale(uint8_t Motor_num);
void Motor_Reset(uint8_t Motor_num);
void Motor_Enable_All(void);
void Motor_Disable_All(void);
void Motor_Clear(uint8_t Motor_num);
void Car_Go_Angle(int32_t Tar_X, int32_t Tar_Y, int16_t Speed, uint16_t Car_ACC);
void car_turn_r(uint16_t Speed);
void Car_turn(float angle , float Speed, uint16_t Car_ACC);
void Car_Go_Straight( float Tar_Y, float Speed, uint16_t Car_ACC);
void Motor_SpeedMode(uint8_t Motor_num,int16_t Speed,uint8_t Acc);
void Motor_PositionMode1(uint8_t Motor_num,uint32_t Pulse,int16_t Speed,uint8_t Acc);
void Motor_Receive_Date(uint8_t com_date);
uint32_t Abs(int32_t temp);
void Motor_Run(void);
void Car_Go_Target(int32_t Tar_X, int32_t Tar_Y, int16_t Speed, uint16_t Car_ACC);
void Car_Go_Target_Pid(int32_t Tar_X, int32_t Tar_Y, int16_t Speed, uint16_t Car_ACC);
void Car_Servo_Move(int32_t W_X, int32_t W_Y,int16_t Speed);

void Read_Cur_Pos(uint8_t Motor_num);
void Read_Cur_V(uint8_t Motor_num);
	
void Car_Go_Target_new(int32_t Tar_X, int32_t Tar_Y, int16_t Speed, uint16_t Car_ACC);
void Motor_SpeedMode1(uint8_t Motor_num,int16_t Speed,uint16_t Acc);

void car_go_r(uint16_t Speed);
	void car_go_l(uint16_t Speed);
	float YawErrorProcess(float current, float target);
	
	uint8_t go(uint16_t Speed,uint16_t Acc);
	float calculate_motor_angle(int32_t motor_position);
	int32_t calculate_motor_position(uint8_t* data) ;
	
void ReceivePos(uint8_t RxData) ;
static uint8_t calc_checksum(uint8_t *data, uint8_t len);
void ReceiveV(uint8_t data);


extern uint8_t uart1_Rxbuff;
extern uint8_t Car_Stop_Flag;
extern uint8_t HuaGui_Stop_Flag;
extern uint16_t RxBuffer1[10];
extern uint8_t MotorStatus[6];

void Motor_Clear_All(void);
void Clear_RxBuffer(void);
void Motor_Init(void);
void Motor_Enable(uint8_t Motor_num);
void Motor_Disbale(uint8_t Motor_num);
void Motor_Reset(uint8_t Motor_num);
void Motor_Enable_All(void);
void Motor_Disbale_All(void);
void Motor_Clear(uint8_t Motor_num);

void Motor_SpeedMode(uint8_t Motor_num,int16_t Speed,uint8_t Acc);
void Motor_PositionMode1(uint8_t Motor_num,uint32_t Pulse,int16_t Speed,uint8_t Acc);
void Motor_PositionMode2(uint8_t Motor_num,uint32_t Pulse,int16_t Speed,uint8_t Acc,uint8_t Dir);
void Motor_Receive_Date(uint8_t com_date);
uint32_t Abs(int32_t temp);
void Motor_Run(void);

float Angle_Pid_A(float Tar_Angle,float Cur_Angle,uint8_t Speed_Limit);
float PosPid_Caculate(float Tar_pos,float Cur_pos,float dt);
uint8_t Car_Go_with_Pid(int16_t Tar_yaw,int16_t Tar_Y);

void Car_Stop(uint16_t Acc);
uint8_t Car_move_with_pid(int32_t Car_Speed_X,int32_t Car_Speed_Y,uint16_t Car_Acc);
void Car_Go_Target(int32_t Tar_X, int32_t Tar_Y, int16_t Speed, uint16_t Car_ACC);
//void Car_Go_Target_A(int32_t Tar_X, int32_t Tar_Y, int16_t Speed, uint16_t Car_ACC);
uint8_t Car_Turn(int16_t Tar_Yaw,uint16_t Speed_Limit,uint16_t Acc);
uint8_t Car_Calibration(int16_t Speed_Limit,uint16_t Acc);

void HuaGui_Move2_Gnd_1(int16_t Speed,uint8_t Acc);
void HuaGui_Init(void);
void HuaGui_Move2_Cam(int16_t Speed,uint8_t Acc);
void HuaGui_Move2_Cam_Low(int16_t Speed,uint8_t Acc);
void HuaGui_Move2_Tran(int16_t Speed,uint8_t Acc);
void HuaGui_Move2_Gnd(int16_t Speed,uint8_t Acc);
void HuaGui_Move2_ZaiWu(int16_t Speed,uint8_t Acc);
void HuaGui_Move2_Get(int16_t Speed,uint8_t Acc);
void HuaGui_Move2_Put(int16_t Speed,uint8_t Acc);
void HuaGui_Move2_Put_1(int16_t Speed,uint8_t Acc);


uint8_t ZaiWu_ctrl(uint8_t state);

uint8_t HuaTai_ctrl(uint8_t state);

#endif
