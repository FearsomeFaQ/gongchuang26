#ifndef __PID_h__
#define __PID_h__

#include "stdio.h"
#include "math.h"

typedef struct
{
	float kp;
	float ki;
	float kd;
	float prev_error;
	float integral;
} PID_Controller;

void Pid_Init(PID_Controller *pid,float kp,float ki,float kd);
float Pid_Calculate(PID_Controller *pid,float tar_val,float cur_val,float dt);

#endif
