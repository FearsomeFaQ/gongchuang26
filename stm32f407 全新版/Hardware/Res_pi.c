#include "Res_pi.h"
#include "gpio.h"
/*
������ݮ�ɴ�������
���ݸ�ʽ��@{task_id:02d}!{param1:+04d}|{param2:+04d}#
@01!+020|-010#
*/

uint8_t uart4_Rxbuff;          //���ݻ�����
uint8_t PI_RawData0[2];        //�洢task_id�ַ�
uint8_t PI_RawData1[4];        //�洢param1�ַ�
uint8_t PI_RawData2[4];        //�洢param2�ַ�
uint8_t RxFlag = 0;            //���ݴ�����־λ

uint8_t PI_Data0 = 0;          //task_id
int16_t PI_Data1;              //param1
int16_t PI_Data2;              //param2

// void PI_Init(void)
// {
// 	__HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);                 //�����ж�
//   HAL_UART_Receive_IT(&huart4, &uart4_Rxbuff,1);
// }

uint8_t rxState = 0;
uint8_t rxCounter = 0;
//���պ���
void PI_Receive_Data(uint8_t com_data)
{
	uint8_t i;
	if(rxState == 0 && com_data == '@' && RxFlag == 0)           //��������ͷ��
	{
		for(i = 0;i < 4;i++)
		{
			
			PI_RawData1[i] = 0x00;                              //��������
			PI_RawData2[i] = 0x00;
		}
		for(i = 0;i < 2;i++)
		{
			PI_RawData0[i] = 0x00;                            //��������
		}
		rxState = 1;
		rxCounter = 0;
	}
	else if(rxState == 1)                                  //����task_id
	{
		if(com_data == '!')
		{
			rxState = 2;
			rxCounter = 0;
		}
		else
		{
			PI_RawData0[rxCounter++] = com_data;
			//printf("111");
		}
	}
	
	
	else if(rxState == 2)                                //����param1
	{
		if(com_data == '|')
		{
			rxState = 3;
			rxCounter = 0;
		}
		else
		{
			PI_RawData1[rxCounter++] = com_data;
		}
	}
	else if(rxState == 3)                            //����param2
	{
		if(com_data == '#')
		{
			rxState = 0;
			rxCounter = 0;
			RxFlag = 1;
		}
		else
		{
			PI_RawData2[rxCounter++] = com_data;
		}
	}
}

uint8_t char_2_num(uint8_t char_temp)
{
	uint8_t temp;
	switch(char_temp)
	{
		case '0':
			temp = 0;
			break;
		case '1':
			temp = 1;
			break;
		case '2':
			temp = 2;
			break;
		case '3':
			temp = 3;
			break;
		case '4':
			temp = 4;
			break;
		case '5':
			temp = 5;
			break;
		case '6':
			temp = 6;
			break;
		case '7':
			temp = 7;
			break;
		case '8':
			temp = 8;
			break;
		case '9':
			temp = 9;
			break;
	}
	return temp;
}

// �������ݴ���
void PI_Receive_Proc(void)
{
	if(RxFlag == 1)
	{
		PI_Data0 = char_2_num(PI_RawData0[0])*10+char_2_num(PI_RawData0[1]);
		if(PI_RawData1[0] == '+')
		{
			PI_Data1 = char_2_num(PI_RawData1[1])*100 + char_2_num(PI_RawData1[2])*10 +char_2_num(PI_RawData1[3]);
		}
		else if(PI_RawData1[0] == '-')
		{
			PI_Data1 = -(char_2_num(PI_RawData1[1])*100 + char_2_num(PI_RawData1[2])*10 +char_2_num(PI_RawData1[3]));
		}
		if(PI_RawData2[0] == '+')
		{
			PI_Data2 = char_2_num(PI_RawData2[1])*100 + char_2_num(PI_RawData2[2])*10 +char_2_num(PI_RawData2[3]);
		}
		else if(PI_RawData2[0] == '-')
		{
			PI_Data2 = -(char_2_num(PI_RawData2[1])*100 + char_2_num(PI_RawData2[2])*10 +char_2_num(PI_RawData2[3]));
		}
		RxFlag = 0;
	}
}

void GPIO_Check_State(void)
{
	 static GPIO_PinState lastState = GPIO_PIN_RESET;
	 GPIO_PinState currentState = HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6);

	if (lastState != currentState)
	{
        
        uint8_t data[] = "99";
        HAL_UART_Transmit(&huart4, data, sizeof(data) - 1, HAL_MAX_DELAY);
   }
	
	lastState = currentState;

}
	






