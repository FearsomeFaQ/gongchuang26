#include "PID.h"


// 初始化PID控制器
void Pid_Init(PID_Controller *pid,float kp,float ki,float kd)
{
	pid -> kp = kp;
	pid -> ki = ki;
	pid -> kd = kd;
	pid -> integral = 0;
	pid -> prev_error = 0;
}

// 计算PID输出控制量
float Pid_Calculate(PID_Controller *pid,float tar_val,float cur_val,float dt)
{
	float error = tar_val - cur_val;
	
	// 计算积分和微分
	pid -> integral += error*dt;
	float derivative = (error - pid -> prev_error) / dt;
	
	// 计算输出
	float output = pid ->kp * error + pid -> ki * pid ->integral + pid -> kd * derivative;
	
	// 保存前一次误差
	pid ->prev_error = error;
	
	return output;
}

