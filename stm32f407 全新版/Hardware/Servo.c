#include "Servo.h"
#include "math.h"



//��צ
uint16_t t_zhua;
float JiaZhua_theta;
uint16_t tf_zhua = (JiaZhua_Close - JiaZhua_Open)*4;
int8_t JiaZhua_Flag;




void Servo_Init(void)
{
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);

	Servo_SetAngel1(90);
}


void Servo_SetAngel1(uint16_t angle)
{
	uint32_t pulse = 340 + (uint32_t)(angle * 1290.0f / 270.0f);
  
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);
	  
	  
		  HAL_Delay(1);
}

void Servo_SetAngel2(uint16_t angle)
{
	uint32_t pulse = 340 + (uint32_t)(angle * 1290.0f / 270.0f);
  
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse);
	  
	  
		  HAL_Delay(1);
}


//��צ���ƺ��� 0����צ�� 1����צ�ر�
// uint8_t Servo_Ctrl_JiaZhua(uint8_t state)
// {
// 	uint8_t ret = 0;
// 	static uint8_t time = 0;
// 	if(state == 0)       //��צ��
// 	{
// 		JiaZhua_Flag = -1;
// 		if(time == 0)
// 		{
// 			t_zhua = 0;
// 			time = 1;
// 		}
// 		if(JiaZhua_theta == JiaZhua_Open)
// 		{
// 			JiaZhua_Flag = 0;
// 			time = 0;
// 			Servo_SetAngel(JiaZhua_Open);
// 			ret = 1;
// 		}
// 	}
// 	else if(state == 1)   //��צ�պ�
// 	{
// 		JiaZhua_Flag = 1;
// 		if(time == 0)
// 		{
// 			t_zhua = 0;
// 			time = 1;
// 		}
// 		if(JiaZhua_theta == JiaZhua_Close)
// 		{
// 			JiaZhua_Flag = 0;
// 			time = 0;
// 			Servo_SetAngel(JiaZhua_Close);
// 			ret = 1;
// 		}
// 	}
// 	return ret;
// }

// �������Ƕ�
// float Caculate_Angle(uint8_t Last_Angle,uint8_t Target_Angle,uint16_t tf,uint16_t t)
// {
// 	float current_theta;
// 	current_theta = Last_Angle + 3/pow(tf,2)*(Target_Angle - Last_Angle)*pow(t,2) - 2/pow(tf,3)*(Target_Angle - Last_Angle)*pow(t,3);
// 	return current_theta;
// }



//�Ӽ��ٻص�����
// void Servo_Callback(void)
// {
// 	if(JiaZhua_Flag == 1)              //��צ�պ�
// 	{           
// 		if(t_zhua <= tf_zhua)
// 		{
// 			JiaZhua_theta = Caculate_Angle(JiaZhua_Open,JiaZhua_Close,tf_zhua,t_zhua);
// 			Servo_SetAngel(JiaZhua_theta);
// 			t_zhua += 2;
// 		}
// 	}
// 	else if(JiaZhua_Flag == -1)        //��צ��
// 	{
// 		if(t_zhua <= tf_zhua)
// 		{
// 			JiaZhua_theta = Caculate_Angle(JiaZhua_Close,JiaZhua_Open,tf_zhua,t_zhua);
// 			Servo_SetAngel(JiaZhua_theta);
// 			t_zhua += 2;
// 		}
// 	}
	
//  }





