#include "Move.h"

#define Jiazhua_Open_1 0
#define Jiazhua_Close_1 1

#define HuaTai_Front_1 0
#define HuaTai_Back_1 1

typedef enum
{
	Ready_State = 0,
	load_State0,
	load_State1,
	load_State2,
	load_State3,
	load_State4,
	load_State5,
	load_State6,
	End_State,
}Get_Loads_State;


//װ�����Ϻ���
uint8_t Get_Loads(int16_t HuaGui_Speed,uint8_t HuaGui_Acc,uint8_t task_num)
{
	uint8_t temp1,temp2;
	uint8_t Get_Loads_return = 0;
	static uint8_t Get_Counter = 1;         //װ�ؼ���
	//static uint8_t Flag = 0;
	static Get_Loads_State Get_loads_State = Ready_State;
	
	if (Get_loads_State == Ready_State)
	{
		if(task_num == 1)
		{
			Get_loads_State = load_State0;     // һ������̨ץȡ
		}
		else if(task_num == 2)
		{
			Get_loads_State = load_State1;     // ��������̨ץȡ
		}
	}
	else if(Get_loads_State == load_State0)
	{	Servo_SetAngel2(duoji_duan);

		HAL_Delay(500);
		HuaGui_Move2_Gnd(HuaGui_Speed,HuaGui_Acc);  //���콵��һ������̨ץȡ��
		Get_loads_State = load_State2;
	}
	else if(Get_loads_State == load_State1)
	{	
		Servo_SetAngel2(duoji_chang);

		HAL_Delay(500);

		HuaGui_Move2_Get(HuaGui_Speed,HuaGui_Acc);  //���콵����������̨ץȡ��

		Get_loads_State = load_State2;
	}
	else if(Get_loads_State == load_State2)
	{
		if(HuaGui_Stop_Flag == 1)
		{
			Servo_SetAngel1(JiaZhua_Close);
			HAL_Delay(500);
			HuaGui_Move2_Tran(HuaGui_Speed,HuaGui_Acc); //�ƶ������ȵ�
			HAL_Delay(500);
			Get_loads_State = load_State3;
		}
	}
	else if(Get_loads_State == load_State3)
	{
		temp1 = HuaTai_ctrl(HuaTai_Back_1);     // ��ǰ����
		if(temp1 == 1)
		{
			HuaGui_Move2_ZaiWu(HuaGui_Speed,HuaGui_Acc);
			HAL_Delay(500);
			Get_loads_State = load_State4;
		}
	}
	else if(Get_loads_State == load_State4)
	{
		//HuaGui_Move2_ZaiWu(HuaGui_Speed,HuaGui_Acc);   //���콵�����õ㣬�����ڻ�̨��תʱʵ�ֻ����½�
		Get_loads_State = load_State5;
	}
	else if(Get_loads_State == load_State5)
	{
		if(HuaGui_Stop_Flag == 1)  // ���쵽������̨
		{
			if(task_num == 1 && Get_Counter < 3)
			{
				Servo_SetAngel1(JiaZhua_Open);//35
			}
			else
			{
				Servo_SetAngel1(JiaZhua_Open);
			}
			HAL_Delay(200);
			HuaGui_Move2_Tran(HuaGui_Speed,HuaGui_Acc);  // �ƶ������ɵ�
			HAL_Delay(300);
			Get_loads_State = load_State6;
		}
	}
	else if(Get_loads_State == load_State6)
	{
		temp1 = HuaTai_ctrl(HuaTai_Front_1);  // �Ӻ���ǰ 
		temp2 = ZaiWu_ctrl(Get_Counter);   // �ƶ�����̨����һλ��	
		if(task_num == 2)
		{
			HuaGui_Move2_Cam(HuaGui_Speed,HuaGui_Acc);   // 
		}
		else if(task_num == 1 && Get_Counter < 3)
		{
			HuaGui_Move2_Gnd(HuaGui_Speed,HuaGui_Acc);   
		}
		else
		{
			HuaGui_Move2_Cam_Low(HuaGui_Speed,HuaGui_Acc);
		}

		if(temp1 == 1 && temp2 == 1)
		{
			Get_loads_State = End_State;
		}
	}
	else if(Get_loads_State == End_State)
	{
		Car_Go_Target(15,0,50,10);
		Get_Counter++;
		Get_Loads_return = 1;
		Get_loads_State = Ready_State;
	}
	if(Get_Counter > 3)  // ����֮������
	{
		Get_Counter = 1;
	}
	return Get_Loads_return;
}



typedef enum
{
	Ready_State_0 = 0,
	Ready_State_1,
	load_State0_1,
	load_State1_1,
	load_State2_1,
	load_State3_1,
	load_State4_0,
	load_State4_1,
	load_State5_1,
	End_State_1,
}Put_Loads_State;


//�������Ϻ���
uint8_t Put_Loads(int16_t HuaGui_Speed,uint8_t HuaGui_Acc,uint8_t task_num)
{
	uint8_t Put_Loads_return = 0;
	static Put_Loads_State Put_Loads_State = Ready_State_0;
	static uint8_t Put_Counter = 1;
	uint8_t temp1,temp2;
	if(Put_Loads_State == Ready_State_0)
	{
		HuaGui_Move2_Tran(HuaGui_Speed,HuaGui_Acc);
		HAL_Delay(500);
		Put_Loads_State = Ready_State_1;
	}
	else if(Put_Loads_State == Ready_State_1)   // ��̨��ת������̨�����콵�����õ�
	{
		temp1 = HuaTai_ctrl(HuaTai_Back_1);   // ��ǰ����
		if(temp1 == 1)
		 {
			 //temp2 = Servo_Ctrl_ZaiWu(Put_Counter);  // ��̨�ƶ�������̨
			 Put_Loads_State = load_State0_1;
			 HAL_Delay(100);
		 }
	}
	else if(Put_Loads_State == load_State0_1) // װ������
	{
		HuaGui_Move2_ZaiWu(HuaGui_Speed,HuaGui_Acc);
		Put_Loads_State = load_State1_1;
	}
	else if(Put_Loads_State == load_State1_1)
	{
		if(HuaGui_Stop_Flag == 1)
		{
			Servo_SetAngel1(JiaZhua_Close);
			HAL_Delay(300);
			Put_Loads_State = load_State2_1;
		}
	}
	else if(Put_Loads_State == load_State2_1)
	{
			HuaGui_Move2_Tran(HuaGui_Speed,HuaGui_Acc);  // �ƶ������ɵ�
			HAL_Delay(300);
			Put_Loads_State = load_State3_1;
	}
	else if(Put_Loads_State == load_State3_1)
	{
		temp1 = HuaTai_ctrl(HuaTai_Front_1);   // ��ת��̨
		if(Put_Counter < 3)
		{
			temp2 = ZaiWu_ctrl(Put_Counter);       // ��ת����̨
		}
		else
		{
			//Servo_SetAngel(Servo_ZaiWu,ZaiWu_1);
			HAL_Delay(250);
		}
		if(task_num == 1 &&( temp2 == 1 || Put_Counter == 3))
		{
			HuaGui_Move2_Gnd(HuaGui_Speed,HuaGui_Acc);  // �ƶ��������ŵ�
		}
		else if(task_num == 2 && ( temp2 == 1 || Put_Counter == 3))
		{
			//HuaGui_Move2_Put_1(HuaGui_Speed,HuaGui_Acc); // �ѵ�
			if(temp1 == 1)
			{
				Put_Loads_State = End_State_1;
			}
		}
		if(temp1 == 1 && task_num != 2)
		{
			Put_Loads_State = load_State4_0;
		}
	}
	else if(Put_Loads_State == load_State4_0)
	{
		if(HuaGui_Stop_Flag == 1)
		{
			Servo_SetAngel1(JiaZhua_Open);
			HAL_Delay(300);
			Put_Loads_State = load_State4_1;
		}
	}
	else if(Put_Loads_State == load_State4_1)
	{
		HuaGui_Move2_Cam_Low(HuaGui_Speed,HuaGui_Acc);
		Put_Loads_State = load_State5_1;
	}
	else if(Put_Loads_State == load_State5_1)
	{
		Put_Counter++;
		Put_Loads_State = End_State_1;
	}
	else if(Put_Loads_State == End_State_1)
	{
		Put_Loads_return = 1;        // ���һ��װ��
		Put_Loads_State = Ready_State_0;
	}
	if(Put_Counter > 3)
	{
		Put_Counter = 1;
	}
	return Put_Loads_return;
}


typedef enum
{
	download1 = 0,
	download2,
	download3
}DownLoads_State;

// uint8_t last_download(int16_t HuaGui_Speed,uint8_t HuaGui_Acc)
// {
// 	uint8_t ret = 0;
// 	static DownLoads_State DownLoads_State = download1;
// 	if(DownLoads_State == download1)
// 	{
// 		Servo_SetAngel(Servo_Turn,0);
// 		HAL_Delay(1000);
// 		HuaGui_Move2_Put_1(HuaGui_Speed,HuaGui_Acc);
// 		if(HuaGui_Stop_Flag == 1)
// 		{
// 			Servo_SetAngel(Servo_JiaZhua,JiaZhua_Open);
// 			HAL_Delay(300);
// 			DownLoads_State = download2;
// 		}
// 	}
// 	else if(DownLoads_State == download2)
// 	{
// 		HuaGui_Move2_Cam(HuaGui_Speed,HuaGui_Acc);
// 		if(HuaGui_Stop_Flag == 1)
// 		{
// 			Servo_SetAngel(Servo_Turn,180);
// 			HAL_Delay(1000);
// 			DownLoads_State = download3;
// 		}
// 	}
// 	else if(DownLoads_State == download3)
// 	{
// 		DownLoads_State = download1;
// 		ret = 1;
// 	}
// 	return ret;
// }

