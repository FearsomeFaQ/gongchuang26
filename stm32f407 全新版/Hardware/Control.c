#include "Control.h"

/*
������ƺ���
����1��С���ƶ�xy ����2��С��ת��
*/

uint8_t Running_Flag = 0;    //���������־λ

uint8_t Car_Counter_Enable = 0;  // ��ʱ��־
uint16_t Car_Counter_Times = 0;
uint16_t Car_Counter = 0;

uint16_t Car_Speed = 200; //300
uint16_t Car_Acc = 100;   //150
uint16_t Car_Speed_Slow = 100;
uint16_t Car_Acc_Slow = 75;
uint16_t Car_Turn_Speed_Limit = 300;  // last 50
uint16_t HuaGui_Speed = 450;
uint16_t HuaGui_Acc = 250;

uint8_t Task_Flag[100] = {0};         //���task_id
int16_t Task_Data1[100] = {0};        //���param1
int16_t Task_Data2[100] = {0};        //���param2

void Control_Proc(void)
{
	if(PI_Data0 != 0)
	{
		Task_Flag[PI_Data0] = 1;         
		Task_Data1[PI_Data0] = PI_Data1;
		Task_Data2[PI_Data0] = PI_Data2;
		
		PI_Data0 = 0;
		PI_Data1 = 0;
		PI_Data2 = 0;
		
		Running_Flag = 1;                //����������Ӧλ�ô������
	}
	
	if(Task_Flag[1])                  //С���ƶ�(x,y)cm   �޸���������
	{
		
		static uint8_t temp = 0;
		Car_Counter_Times = 1250;  // ��ʱʱ��2.5��
		if(temp == 0)
		{
			Car_Counter_Enable = 1;
			
			Car_Go_Target(Task_Data1[1]*128,Task_Data2[1]*128,Car_Speed,Car_Acc);
			temp = 1;
		}
		else if(temp == 1)
		{
			if(Car_Stop_Flag == 1 || Car_Counter_Enable == 0)
			{
				//Motor_Clear_All();
				Running_Flag = 0;
				temp = 0;
				Task_Flag[1] = 0;
				Car_Counter_Enable = 0;
				Car_Counter = 0;
				
				HAL_UART_Transmit(&huart4,(uint8_t*)"01",2,0xffff);
			}
		}
	}
	
	else if(Task_Flag[2])                   //ת�� ����
	{
		static uint8_t temp = 0;
		
		if(temp == 0)
		{
			if(Car_Turn(Task_Data1[2],Car_Turn_Speed_Limit,200) == 1)          //ת�����
			{
				temp = 1;
			}
			
			
		}
		else if(temp == 1)
		{
			Running_Flag = 0;
			temp = 0;
			Task_Flag[2] = 0;
				
			HAL_UART_Transmit(&huart4,(uint8_t*)"02",2,0xffff);
		}
	}
	if(Task_Flag[3])                //С���ƶ�(x,y)mm
	{
		static uint8_t temp = 0;
		Car_Counter_Times = 1000;     //2s
		if(temp == 0)
		{
			Car_Counter_Enable = 1;
			Car_Go_Target(Task_Data1[3]*13,Task_Data2[3]*13,Car_Speed_Slow,Car_Acc_Slow);

			
			temp = 1;             
		}
		else if(temp == 1)
		{
			if(Car_Stop_Flag == 1 || Car_Counter_Enable == 0)
			{
				//Motor_Clear_All();
				Running_Flag = 0;
				temp = 0;
				Task_Flag[3] = 0;
				Car_Counter_Enable = 0;
				Car_Counter = 0;
					
				HAL_UART_Transmit(&huart4,(uint8_t *)"03",2,0xffff);
			}
		}
	}
	if(Task_Flag[4])        // ִ��ץȡ����
	{
		static uint8_t temp = 0;
		
		if(temp == 0)
		{
			if(Get_Loads(HuaGui_Speed,HuaGui_Acc,Task_Data1[4]) == 1)
			{
				temp = 1;
			}
		}
		else if(temp == 1)
		{
			Running_Flag = 0;
			temp = 0;
			Task_Flag[4] = 0;
			
			HAL_UART_Transmit(&huart4,(uint8_t *)"04",2,0xffff);
		}
	}
	if(Task_Flag[5])      // ִ�з������� 
	{
		static uint8_t temp = 0;
		
		if(temp == 0)
		{
			if(Put_Loads(HuaGui_Speed,HuaGui_Acc,Task_Data1[5]) == 1)
			{
				temp = 1;
			}
		}
		else if(temp == 1)
		{
			Running_Flag = 0;
			temp = 0;
			Task_Flag[5] = 0;
			
			HAL_UART_Transmit(&huart4,(uint8_t *)"05",2,0xffff);
		}
	}
	if(Task_Flag[6])   // ��������߶�
	{
		static uint8_t temp = 0;
		if(temp == 0)
		{
			if(Task_Data1[6] == 1)  // �������
			{
				HuaGui_Move2_Cam(HuaGui_Speed,HuaGui_Acc);
			}
			else if(Task_Data1[6] == 2)
			{
				HuaGui_Move2_Cam_Low(HuaGui_Speed,HuaGui_Acc);  // ����Բ��
			}
			temp = 1;
		}
		else if(temp == 1)
		{
			Running_Flag = 0;
			temp = 0;
			Task_Flag[6] = 0;
			
			HAL_UART_Transmit(&huart4,(uint8_t *)"06",2,0xffff);
		}
	}
	if(Task_Flag[7])   // У׼���� ����
	{
		static uint8_t temp = 0;
		if(temp == 0)
		{
			if(Car_Calibration(Car_Turn_Speed_Limit,Car_Acc_Slow) == 1)
			{
				temp = 1;
			}
		}
		else if(temp == 1)
		{
			Running_Flag = 0;
			temp = 0;
			Task_Flag[7] = 0;
			
			HAL_UART_Transmit(&huart4,(uint8_t *)"07",2,0xffff);
		}
	}
	if(Task_Flag[8])
	{
		static uint8_t temp = 0;
		if(temp == 0)
		{
			if(Car_move_with_pid(Task_Data1[8],Task_Data2[8],15) == 1)
			{
				temp = 1;
			}
		}
		else if(temp == 1)
		{
			temp = 0;
			Task_Flag[8] = 0;
		}
	}
	if(Task_Flag[9])     //LCD
	{
		static uint8_t temp = 0;
		if(temp == 0)
		{
//			LCD_Task(Task_Data1[9],Task_Data2[9]);
//			temp = 1;
		}
		else if(temp == 1)
		{
			Task_Flag[9] = 0;
			temp = 0;
		}
	}
	if(Task_Flag[10])     //
	{
		static uint8_t temp = 0;
		if(temp == 0)
		{
//			if(Car_Go_with_Pid(Task_Data1[10],Task_Data2[10]) == 1)
//			{
//				temp = 1;
//			}
		}
		else if(temp == 1)
		{
			Task_Flag[10] = 0;
			temp = 0;
			
			HAL_UART_Transmit(&huart4,(uint8_t *)"10",2,0xffff);
		}
	}
	if(Task_Flag[11])     //
	{
		// static uint8_t temp = 0;
		// if(temp == 0)
		// {
		// 	if(Task_Data1[11])
		// 	{
		// 		Servo_SetAngel(Servo_HuaTai,HuaTai_Front);
		// 		Servo_SetAngel(Servo_ZaiWu,ZaiWu_1);
		// 		HAL_Delay(100);
		// 		HuaGui_Move2_Gnd_1(400,250);
		// 		temp = 1;
		// 	}
		// 	else if(Task_Data2[11])
		// 	{
		// 		HuaGui_Move2_Tran(300,150);
		// 		HAL_Delay(250);
		// 		Servo_SetAngel(Servo_HuaTai,HuaTai_Back);
		// 		Servo_SetAngel(Servo_ZaiWu,45);
		// 		temp = 1;
		// 	}	
		// }
		// else if(temp == 1)
		// {
		// 	Task_Flag[11] = 0;
		// 	temp = 0;
			
		// 	HAL_UART_Transmit(&huart4,(uint8_t *)"11",2,0xffff);
		// }
	}
	if(Task_Flag[12])     //
	{
		// static uint8_t temp = 0;
		// if(temp == 0)
		// {
		// 	if(last_download(HuaGui_Speed,HuaGui_Acc) == 1)
		// 	{
		// 		temp = 1;
		// 	}
		// }
		// else if(temp == 1)
		// {
		// 	Task_Flag[12] = 0;
		// 	temp = 0;
			
		// 	HAL_UART_Transmit(&huart4,(uint8_t *)"12",2,0xffff);
		// }
	}
		if(Task_Flag[13])     //ͬɫ
	{
		HAL_UART_Transmit(&huart4,(uint8_t*)"13",2,0xffff);
	}
			if(Task_Flag[14])     //��ɫ
	{
		HAL_UART_Transmit(&huart4,(uint8_t*)"14",2,0xffff);
	}
	else if(Task_Flag[15])                   //pidֱ��
	{
		static uint8_t temp = 0;
		
		Car_Counter_Times = 1250;  // ��ʱʱ��2.5��
		
		if(temp == 0)
		{
			Car_Counter_Enable = 1;
			if(go(Car_Speed,Car_Acc) == 1)       //pid
			{
				temp = 1;
			}
			
			
		}
		else if(temp == 1)
		{
			if(Car_Stop_Flag == 1 || Car_Counter_Enable == 0)
			{
				//Motor_Clear_All();
				Running_Flag = 0;
				temp = 0;
				Task_Flag[15] = 0;
				Car_Counter_Enable = 0;
				Car_Counter = 0;
				
				HAL_UART_Transmit(&huart4,(uint8_t*)"15",2,0xffff);
			}
		}
	}
	if(Task_Flag[16])     
	{
		Task_Flag[16] = 0;
		Running_Flag = 0;
		HAL_Delay(1000);
		HAL_UART_Transmit(&huart4,(uint8_t*)"test",5,0xffff);
	}
}

//@01!+050|+000#  צ�Ӵ�ֱ������
//����01Ȼ����
//@13!+000|+000# ͬɫ
//����13Ȼ����
//@01!+069|+000#  ���� 50+69=119
//����01Ȼ����
//@07!+000|+000# У������
//����07Ȼ����
//@06!+001|+000# ������������
//����06Ȼ����
//@01!+X|+Y# ���ζ���
//����01Ȼ����




