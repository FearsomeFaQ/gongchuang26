#include "motor.h"
#include "stdio.h"
#include "math.h"

#include "Control.h"

uint8_t uart1_Rxbuff;
uint8_t Send_Data[20];
uint8_t MotorStatus[6] = {0};
uint16_t RxBuffer1[10] = {0};
uint8_t Car_Stop_Flag = 0; 
uint8_t MotorStatu[6] = {0};      // ֧ܺսλ݆˽
float Motor_Angle = 0;


#define WHEEL_DIAMETER_CM 75.0f
#define PULSE_PER_ROUND 3200  // 每圈脉冲数
#define M_PI 3.14159


/**************
接收电机返回数据
Rxstate
0   接收数据头针  0x01(左前轮) || 0x05(滑轨)
1   接收数据信息
2   接受数据尾针  0x6B
**************/
void Motor_Receive_Date(uint8_t com_date)
{
    uint8_t i;
    static uint8_t RxState = 0;
    static uint8_t Rxcounter = 0;

    // Step 1: Start receiving data based on motor ID
    if (RxState == 0 && (com_date == 0x01 || com_date == 0x05 || com_date == 0x03 || com_date == 0x04)) 
    {
        RxState = 1;
        RxBuffer1[Rxcounter++] = com_date;  
    }

    // Step 2: Continue receiving data
    else if (RxState == 1)
    {
        RxBuffer1[Rxcounter++] = com_date;  

        if (Rxcounter >= 10 || com_date == 0x6B)  
        {
            RxState = 2;
        }
    }

    // Step 3: Handle received data
    if (RxState == 2)
    {
        // Ensure that the last byte is the expected termination byte (0x6B)
        if (RxBuffer1[Rxcounter - 1] == 0x6B)
        {
            uint8_t Motor_id = RxBuffer1[0];  
            RxState = 0;                      
            Rxcounter = 0;                    

            // Check if we received the correct position reached command
            if (RxBuffer1[1] == 0xFD && RxBuffer1[2] == 0x9F) 
            {
                // Handle different motor IDs
                if (Motor_id == 0x01)
                {
                    MotorStatus[1] = 1;  
                }
                else if (Motor_id == 0x02)
                {
                    MotorStatus[2] = 1; 
				}
				else if (Motor_id == 0x03)
                {
                    MotorStatus[3] = 1;  
                }
                else if (Motor_id == 0x04)
                {
                    MotorStatus[4] = 1;  
                }
                else if (Motor_id == 0x05)
                {
                    HuaGui_Stop_Flag = 1;  
                }

                // Check if at least two motors have reached their target positions
                uint8_t Motor_Count = 0;
                for (i = 1; i <= 4; i++)
                {
                    if (MotorStatus[i] == 1)
                    {
                        Motor_Count++;
                    }
                }

                // If two or more motors are done, set Car_Stop_Flag
                if (Motor_Count >= 2)
                {
                    Motor_Count = 0;
                    Car_Stop_Flag = 1;

                    // Reset motor status for all motors
                    for (i = 1; i <= 4; i++)
                    {
                        MotorStatus[i] = 0;
                    }
                }

                // Clear the buffer after processing
                Clear_RxBuffer();
            }
						// 解析电机返回角度
						else if(RxBuffer1[1] == 0x36)
						{
							Motor_Angle = (float)((RxBuffer1[3] << 24 | RxBuffer1[4] << 16 | RxBuffer1[5] << 8 | RxBuffer1[6])*360.0/65536.0);
							if(RxBuffer1[2] == 0x01)
							{
								Motor_Angle = -Motor_Angle;
							}
							Clear_RxBuffer();
						}
            else  // If the data doesn't match expected command
            {
                Clear_RxBuffer();
            }
        }
        else  // If the last byte isn't 0x6B, reset the state machine
        {
            RxState = 0;
            Rxcounter = 0;
            Clear_RxBuffer();  // 
        }
    }
}

void Clear_RxBuffer(void)
{
    for (uint8_t i = 0; i < 10; i++)
    {
        RxBuffer1[i] = 0x00;  
    }
}

uint32_t Abs(int32_t temp)
{
	if(temp >= 0)
	{
		return temp;
	}
	else
	{
		return -temp;
	}
}
void Motor_Enable(uint8_t Motor_num)
{
	Send_Data[0] = Motor_num;
	Send_Data[1] = 0xF3;
	Send_Data[2] = 0xAB;
	Send_Data[3] = 0x01;
	Send_Data[4] = 0x00;
	Send_Data[5] = 0x6B;
	HAL_UART_Transmit(&huart1,Send_Data,6,1000);
	HAL_Delay(100);
}

// ֧ܺʧŜ
void Motor_Disbale(uint8_t Motor_num)
{
	Send_Data[0] = Motor_num;
	Send_Data[1] = 0xF3;
	Send_Data[2] = 0xAB;
	Send_Data[3] = 0x00;
	Send_Data[4] = 0x00;
	Send_Data[5] = 0x6B;
	HAL_UART_Transmit(&huart1,Send_Data,6,1000);
	HAL_Delay(100);
}

// ֹ֧ܺͣת֯
void Motor_Reset(uint8_t Motor_num)
{
	Send_Data[0] = Motor_num;
	Send_Data[1] = 0xFE;
	Send_Data[2] = 0x98;
	Send_Data[3] = 0x00;
	Send_Data[4] = 0x6B;
	HAL_UART_Transmit(&huart1,Send_Data,5,1000);
	HAL_Delay(100);
}

void Motor_Enable_All(void)
{
	Motor_Enable(1);
	Motor_Enable(2);
	Motor_Enable(3);
	Motor_Enable(4);
	Motor_Enable(5);
	Motor_Enable(6);
	Motor_Enable(7);
}

void Motor_Disable_All(void)
{
	Motor_Disbale(1);
	Motor_Disbale(2);
	Motor_Disbale(3);
	Motor_Disbale(4);
	Motor_Disbale(5);
	Motor_Disbale(6);
	Motor_Disbale(7);
}

void Clear_RxBuffer1(void)
{
    for (uint8_t i = 0; i < 10; i++)
    {
        RxBuffer1[i] = 0x00;  
    }
}

// ݫձǰλ׃އ׈ȥ£
void Motor_Clear(uint8_t Motor_num)
{
	Send_Data[0] = Motor_num;
	Send_Data[1] = 0x0A;
	Send_Data[2] = 0X6D;
	Send_Data[3] = 0X6B;
	HAL_UART_Transmit(&huart1,Send_Data,4,1000);
	HAL_Delay(100);
}

void Motor_Clear_All(void)
{
	Motor_Clear(1);
	Motor_Clear(2);
	Motor_Clear(3);
	Motor_Clear(4);
	Motor_Clear(5);
	Motor_Clear(6);
	Motor_Clear(7);
	
}

void Motor_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  HAL_UART_Receive_IT(&huart1, &uart1_Rxbuff, 1);              //ޡ˸һٶؖޚ

	HAL_Delay(100);
	Motor_Enable_All();
	Motor_Clear_All();
	
}




void Read_Cur_Pos(uint8_t Motor_num)
{
	Send_Data[0] = Motor_num;
	Send_Data[1] = 0x36;
	Send_Data[2] = 0x6B;
	HAL_UART_Transmit(&huart1,Send_Data,3,1000);
	HAL_Delay(10);
}

void Read_Cur_V(uint8_t Motor_num)
{
	Send_Data[0] = Motor_num;
	Send_Data[1] = 0x35;
	Send_Data[2] = 0x6B;
	HAL_UART_Transmit(&huart1,Send_Data,3,1000);
	HAL_Delay(10);
}

void Motor_Run(void)
{
      Send_Data[0] = 0x00;
      Send_Data[1] = 0xFF;
      Send_Data[2] = 0x66;
      Send_Data[3] = 0x6B;
      HAL_UART_Transmit(&huart1, Send_Data, 4, 1000);
      HAL_Delay(10);
}













//速度模式

void Motor_SpeedMode(uint8_t Motor_num,int16_t Speed,uint8_t Acc)
{
	uint8_t Direction;
	uint16_t Speed_Set = Abs(Speed);
	Send_Data[0] = Motor_num;
	Send_Data[1] = 0xF6;
	if(Motor_num == 2 ||Motor_num == 3 )
	{
		if(Speed >= 0)
		{
			Direction = 1;
		}
		else
		{
			Direction = 0;
		}
	}
	else
	{
		if(Speed >= 0)
		{
			Direction = 1;
		}
		else
		{
			Direction = 0;
		}
	}
	Send_Data[2] = Direction;
	Send_Data[3] = Speed_Set >> 8;
	Send_Data[4] = (Speed_Set << 8) >> 8;
	Send_Data[5] = Acc;
	Send_Data[6] = 0x00;
	Send_Data[7] = 0x6B;
	HAL_UART_Transmit(&huart1,Send_Data,8,1000);
	HAL_Delay(1);
}


// 相对位置模式
void Motor_PositionMode1(uint8_t Motor_num,uint32_t Pulse,int16_t Speed,uint8_t Acc)
{
	uint8_t Direction;
	uint16_t Speed_Set = Abs(Speed);
	Send_Data[0] = Motor_num;
	Send_Data[1] = 0xFD;
	if(Motor_num == 1||Motor_num ==4)
	{
		if(Speed >= 0)
		{
			Direction = 0;
		}
		else
		{
			Direction = 1;
		}
	}
	else
	{
		if(Speed >= 0)
		{
			Direction = 1;
		}
		else
		{
			Direction = 0;
		}
	}
	Send_Data[2] = Direction;
	Send_Data[3] = Speed_Set >> 8;
	Send_Data[4] = (Speed_Set << 8) >> 8;
	Send_Data[5] = Acc;
	Send_Data[6] = Pulse >> 24;
	Send_Data[7] = (Pulse <<8) >> 24;
	Send_Data[8] = (Pulse <<16) >>24;
	Send_Data[9] = (Pulse << 24) >> 24;
	Send_Data[10] = 0x00;
	Send_Data[11] = 0x00;
	Send_Data[12] = 0x6B;
	HAL_UART_Transmit(&huart1,Send_Data,13,1000);
	
	
  HAL_Delay(10);
	

}

// 绝对位置模式
void Motor_PositionMode2(uint8_t Motor_num,uint32_t Pulse,int16_t Speed,uint8_t Acc,uint8_t Dir)
{
	uint16_t Speed_Set = Abs(Speed);
	Send_Data[0] = Motor_num;
	Send_Data[1] = 0xFD;
	
	Send_Data[2] = Dir;
	Send_Data[3] = Speed_Set >> 8;
	Send_Data[4] = (Speed_Set << 8) >> 8;
	Send_Data[5] = Acc;
	Send_Data[6] = Pulse >> 24;
	Send_Data[7] = (Pulse <<8) >> 24;
	Send_Data[8] = (Pulse <<16) >>24;
	Send_Data[9] = (Pulse << 24) >> 24;
	Send_Data[10] = 0x01;
	Send_Data[11] = 0x00;            //不启用多机同步
	Send_Data[12] = 0x6B;
	HAL_UART_Transmit(&huart1,Send_Data,13,1000);

//    if (Motor_num == 5)
//    {
//      uint8_t i = 0;
//      HuaGui_Stop_Flag = 0;
//      for (i = 0; i < 4; i++)
//      {
//        RxBuffer1[i] = 0x00; 
//      }
//      HuaGui_Stop_Flag = 0;
//    }
}

/*
--------------------------------------------------------小车运动函数--------------------------------------------------------
*/

//左前1 右前2 左后3 右后4
void car_go_r(uint16_t Speed){

  Motor_SpeedMode(2,Speed,150);
	Motor_SpeedMode(4,Speed,150);

}

void car_go_l(uint16_t Speed){

Motor_SpeedMode(1,Speed,150);
	Motor_SpeedMode(3,Speed,150);

}

void car_turn_l(uint16_t Speed){

Motor_SpeedMode(1,-Speed,0);
	Motor_SpeedMode(2,Speed,0);
	
	Motor_SpeedMode(3,Speed,0);
	
	Motor_SpeedMode(4,-Speed,0);

}

// 目标偏航角（回正到0°）
float TARGET_YAW=0;

// PID参数（需根据实际系统整定）
#define Kp  1.0f   // 比例系数
#define Ki  0.01f  // 积分系数（小值避免累积过量）
#define Kd  0.5f   // 微分系数

// 全局变量
float current_yaw = 0;    // 当前偏航角
float yaw_error = 0.0f;      // 偏差
float yaw_error_sum = 0.0f;  // 偏差积分（累积）
float last_yaw_error = 0.0f; // 上一次偏差（用于计算微分）
float yaw_output = 0.0f;     // PID输出（控制量）



uint8_t go(uint16_t Speed,uint16_t Acc) {
	uint8_t ret = 0;
	static uint8_t Temp_State = 0;
	if(Temp_State == 0)
	{
		Temp_State = 1;
		
	}
	else if(Temp_State == 1)
	{		
		// 1. 获取当前yaw角（需从传感器实时更新，如在定时器中断中调用）
		current_yaw = Yaw;
		
		// 2. 计算处理后的偏差
		yaw_error = YawErrorProcess(current_yaw, TARGET_YAW);
	
		// 3. 积分项（限制积分累积，避免超调）
		yaw_error_sum += yaw_error;
		// 积分限幅（根据系统调整，防止积分饱和）
		if (yaw_error_sum > 500.0f) yaw_error_sum = 500.0f;
		if (yaw_error_sum < -500.0f) yaw_error_sum = -500.0f;
	
		// 4. 微分项（当前偏差 - 上一次偏差，反映变化率）
		float error_diff = yaw_error - last_yaw_error;
		last_yaw_error = yaw_error;  // 保存当前偏差，用于下一次计算
	
		// 5. 计算PID输出
		yaw_output = Kp * yaw_error + Ki * yaw_error_sum + Kd * error_diff;
	
		// 6. 输出限幅（避免控制量过大导致动作剧烈）
		if (yaw_output > 100.0f) yaw_output = 100.0f;
		if (yaw_output < -100.0f) yaw_output = -100.0f;
			
		
		Motor_SpeedMode(1,Speed + yaw_output,Acc);
		Motor_SpeedMode(3,Speed + yaw_output,Acc);
		
		Motor_SpeedMode(2,Speed - yaw_output,Acc);
		Motor_SpeedMode(4,Speed - yaw_output,Acc);

	
	
		
		if(Car_Counter_Enable == 0)        
		{
			Motor_SpeedMode(1,0,Acc);
			Motor_SpeedMode(2,0,Acc);
			Motor_SpeedMode(3,0,Acc);
			Motor_SpeedMode(4,0,Acc);
			Motor_Run();
			Temp_State = 0;
			ret = 1;
		}
	}
	return ret;
    
}

float YawErrorProcess(float current, float target) {
    float error = current - target;
    // 若偏差绝对值超过180°，取反方向的小偏差
    if (error > 180.0f) {
        error -= 360.0f;
    } else if (error < -180.0f) {
        error += 360.0f;
    }
    return error;
}
//相对位置闭环 12800脉冲 4圈 1米 x,y:沿x,y轴移动的距离
void Car_Go_Target(int32_t Tar_X, int32_t Tar_Y, int16_t Speed, uint16_t Car_ACC)
{
      int32_t V1, V2;
      V2 = Speed * Abs(Tar_Y - Tar_X) / (1.4 * sqrt(Tar_X * Tar_X + Tar_Y * Tar_Y));
      V1 = Speed * Abs(Tar_Y + Tar_X) / (1.4 * sqrt(Tar_X * Tar_X + Tar_Y * Tar_Y));

      if (Tar_X + Tar_Y >= 0)
      {
            Motor_PositionMode1(1, Tar_X + Tar_Y, V1, Car_ACC);
            Motor_PositionMode1(4, Tar_X + Tar_Y, V1, Car_ACC);
      }
      else
      {
            Motor_PositionMode1(1, -(Tar_X + Tar_Y), -V1, Car_ACC);
            Motor_PositionMode1(4, -(Tar_X + Tar_Y), -V1, Car_ACC);
      }
      if (Tar_Y - Tar_X >= 0)
      {
            Motor_PositionMode1(2, Tar_Y - Tar_X, V2, Car_ACC);
            Motor_PositionMode1(3, Tar_Y - Tar_X, V2, Car_ACC);
      }
      else
      {
            Motor_PositionMode1(2, Tar_X - Tar_Y, -V2, Car_ACC);
            Motor_PositionMode1(3, Tar_X - Tar_Y, -V2, Car_ACC);
      }
      Motor_Run();
}







uint8_t Car_Turn(int16_t Tar_Yaw,uint16_t Speed_Limit,uint16_t Acc)
{
	uint8_t ret = 0;
	static float Motor_Kp;   // last 1.67
	static uint8_t Temp_State = 0;
	static float Temp_Yaw;
	static float Last_Yaw;
	if(Temp_State == 0)
	{
		Temp_State = 1;
		if(Tar_Yaw >= 90 || Tar_Yaw <= -90)
		{
			Temp_Yaw = Last_Yaw +Tar_Yaw;
			Last_Yaw += Tar_Yaw;
			Motor_Kp = 1.90;   // last 1.48
		}
		else
		{
			Temp_Yaw = Yaw + Tar_Yaw;       // 角度纠偏
			Motor_Kp = 3.1;
		}
	}
	else if(Temp_State == 1)
	{		
		float Yaw_Error = Yaw - Temp_Yaw;       //Yaw来自陀螺仪		
		Yaw_Error *= Motor_Kp;                  
		if(Yaw_Error >= Speed_Limit)            //输出限幅
		{
			Yaw_Error = Speed_Limit;
		}
		else if(Yaw_Error <= -Speed_Limit)
		{
			Yaw_Error = -Speed_Limit;
		}
		Motor_SpeedMode(1,Yaw_Error,Acc);
		Motor_SpeedMode(2,-Yaw_Error,Acc);
		Motor_SpeedMode(3,Yaw_Error,Acc);
		Motor_SpeedMode(4,-Yaw_Error,Acc);
		Motor_Run();
		
		if(Abs(Yaw-Temp_Yaw) <= 0.8)        //角度容忍度
		{
			Motor_SpeedMode(1,0,Acc);
			Motor_SpeedMode(2,0,Acc);
			Motor_SpeedMode(3,0,Acc);
			Motor_SpeedMode(4,0,Acc);
			Motor_Run();
			Temp_State = 0;
			ret = 1;
		}
	}
	return ret;
}

//车身校准
uint8_t Car_Calibration(int16_t Speed_Limit,uint16_t Acc)
{
	static uint8_t Temp_State = 0;
  static float Temp_Yaw = 0;
  uint8_t ret = 0;
                       //Yaw来自陀螺仪
	if(Temp_State == 0)
	{
		Temp_State = 1;
		Temp_Yaw = Yaw;
	}
	else if(Temp_State == 1)
	{
		uint8_t Temp = 0;
		if(Temp_Yaw <= 10 && Temp_Yaw >= -10)
		{
			Temp = Car_Turn(-Temp_Yaw,Speed_Limit,Acc);
		}
		if(Temp_Yaw <= 100 && Temp_Yaw >= 80)
		{
			Temp = Car_Turn(-Temp_Yaw+90,Speed_Limit,Acc);
		}
		if(Temp_Yaw <= -80 && Temp_Yaw >= -100)
		{
			Temp = Car_Turn(Temp_Yaw+90,Speed_Limit,Acc);
		}
		if(Temp_Yaw <= 190 && Temp_Yaw >= 170)
		{
			Temp = Car_Turn(-Temp_Yaw+180,Speed_Limit,Acc);
		}
		if(Temp_Yaw <= -170 && Temp_Yaw >= -190)
		{
			Temp = Car_Turn(Temp_Yaw+180,Speed_Limit,Acc);
		}
		if(Temp_Yaw <= 280 && Temp_Yaw >= 260)
		{
			Temp = Car_Turn(-Temp_Yaw+270,Speed_Limit,Acc);
		}
		if(Temp_Yaw <= -260 && Temp_Yaw >= -280)
		{
			Temp = Car_Turn(Temp_Yaw+270,Speed_Limit,Acc);
		}
		if(Temp == 1)
		{
			ret = 1;
			Temp_State = 0;
			Temp_Yaw = 0;
		}
	}
	return ret;
}

uint8_t Car_move_with_pid(int32_t Car_Speed_X,int32_t Car_Speed_Y,uint16_t Car_Acc)
{
	uint8_t ret = 0;
	int32_t Speed1 = 0,Speed2 = 0,Speed3 = 0,Speed4 = 0;
	if(Car_Speed_X == 0 && Car_Speed_Y == 0)
	{
		Motor_SpeedMode(1,0,0);
		Motor_SpeedMode(2,0,0);
		Motor_SpeedMode(3,0,0);
		Motor_SpeedMode(4,0,0);
		Motor_Run();
		ret = 1;
	}
	if(Car_Speed_X != 0)
	{
		Speed1 =  Car_Speed_X;
		Speed2 = -Car_Speed_X;
		Speed3 = -Car_Speed_X;
		Speed4 =  Car_Speed_X;
	}
	if(Car_Speed_Y != 0) // 向Y轴正半轴移动
	{
		Speed1 = Car_Speed_Y;
		Speed2 = Car_Speed_Y;
		Speed3 = Car_Speed_Y;
		Speed4 = Car_Speed_Y;
	}
	Motor_SpeedMode(1,Speed1,Car_Acc);
	Motor_SpeedMode(2,Speed2,Car_Acc);
	Motor_SpeedMode(3,Speed3,Car_Acc);
	Motor_SpeedMode(4,Speed4,Car_Acc);
	Motor_Run();
	return ret;
}



/*------------------------------------------------滑轨05----------------------------------------------------*/

#define Camera_Pulse 3000       // cw 物块相机位
#define Camera_Low_Pulse 1000   // CCW 圆环相机位
#define Ground_Pulse 6700       // ccw 一层 放置
#define Transition_Pulse 5300   // cw 旋转高度点
#define Get_loads_Pulse  500   // 二层抓取点
#define Put_loads_Pulse  3000   // 车载物圆盘放置点
#define Put_loads_Pulse_1  700
#define CW 0  //顺时针
#define CCW 1

uint8_t HuaGui_Stop_Flag = 1;  //滑轨停止标志位

//滑轨初始化
void HuaGui_Init(void)
{
	HuaGui_Move2_Tran(80,40);
}

//物块相机位
void HuaGui_Move2_Cam(int16_t Speed,uint8_t Acc)
{
	Motor_PositionMode2(5,Camera_Pulse,Speed,Acc,CW);
}

// 圆环相机位
void HuaGui_Move2_Cam_Low(int16_t Speed,uint8_t Acc)
{
	Motor_PositionMode2(5,Camera_Low_Pulse,Speed,Acc,CCW);
}

//滑轨过渡点
void HuaGui_Move2_Tran(int16_t Speed,uint8_t Acc)
{
	Motor_PositionMode2(5,Transition_Pulse,Speed,Acc,CW);
}

//滑轨降到一层抓取 放置点
void HuaGui_Move2_Gnd(int16_t Speed,uint8_t Acc)
{
	Motor_PositionMode2(5,Ground_Pulse,Speed,Acc,CCW);
}

//滑轨降到最低抓取 放置点
void HuaGui_Move2_Gnd_1(int16_t Speed,uint8_t Acc)
{
	Motor_PositionMode2(5,Ground_Pulse,Speed,Acc,CCW);
}

//载物台抓取、放置点
void HuaGui_Move2_ZaiWu(int16_t Speed,uint8_t Acc)
{
	Motor_PositionMode2(5,Put_loads_Pulse,Speed,Acc,CW);
}

//二层抓取点
void HuaGui_Move2_Get(int16_t Speed,uint8_t Acc)
{
	Motor_PositionMode2(5,Get_loads_Pulse,Speed,Acc,CW);
}

//滑轨降至放置点
void HuaGui_Move2_Put(int16_t Speed,uint8_t Acc)
{
	Motor_PositionMode2(5,Put_loads_Pulse,Speed,Acc,CW);  //转向
}

// 二次堆叠放置点
void HuaGui_Move2_Put_1(int16_t Speed,uint8_t Acc)
{
	Motor_PositionMode2(5,Put_loads_Pulse_1,Speed,Acc,CCW);
}


/*------------------------------------------------载物台06----------------------------------------------------*/

#define ZaiWu_1 3334 //3200/3*2
#define ZaiWu_2 1667
#define ZaiWu_3  0
#define Speed_zai 50//旋转速度
#define Acc_zai 10
#define CW 0  //顺时针
#define CCW 1

//滑轨初始化
void Zaiwu_Init(void)
{
	Motor_PositionMode2(6,ZaiWu_1,Speed_zai,10,CW);
}

// 载物台控制函数
uint8_t ZaiWu_ctrl(uint8_t state)
{
	uint8_t ret = 0;
	if(state == 1)           // 初始位置到第二位置 
	{
		Motor_PositionMode2(6,ZaiWu_2,Speed_zai,Acc_zai,CW);
		ret = 1;
		
	}
	else if(state == 2)       // 第二位置到第三位置 
	{
		Motor_PositionMode2(6,ZaiWu_3,Speed_zai,Acc_zai,CW);	
		ret = 1;
		
	}
	else if(state == 3)      //第三位置返回初始位置 
	{
		Motor_PositionMode2(6,ZaiWu_1,Speed_zai,Acc_zai,CW);
		ret = 1;
	}
	return ret;
}

/*------------------------------------------------滑台07----------------------------------------------------*/

#define Hua_1 0
#define Hua_2 1600
#define Speed_hua 50//旋转速度
#define Acc_hua 10
#define CW 0  //顺时针
#define CCW 1

//滑台控制函数
uint8_t HuaTai_ctrl(uint8_t state)
{
	uint8_t ret = 0;
	if(state == 0)       //滑台从后往前
	{
		Motor_PositionMode2(6,ZaiWu_3,Speed_hua,Acc_hua,CW);
			HAL_Delay(1000);
			ret = 1;
		
	}
	else if(state == 1)   //滑台从前往后
	{
		Motor_PositionMode2(6,ZaiWu_3,Speed_hua,Acc_hua,CCW);	
		HAL_Delay(1000);
			ret = 1;
	}
	
	return ret;
}	
