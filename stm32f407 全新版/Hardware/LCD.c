#include "LCD.h"

uint8_t LCD_Send_Data1[19] = {0x74,0x30,0x2E,0x74,0x78,0x74,0x3D,0x22,0x00,0x00,0x00,0x2B,0x00,0x00,0x00,0x22,0xFF,0xFF,0xFF};
uint8_t LCD_Send_Data_float[17] = {0x78,0x30,0x2E,0x76,0x61,0x6C,0x3D,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0xFF,0xFF,0xFF};

int yawint;

void LCD_Init(void)
{
	HAL_UART_Transmit(&huart3,LCD_Send_Data1,19,1000);
}	

//LCD·¢ËÍÈÎÎñ
void LCD_Task(uint16_t param1,uint16_t param2)
{
	if(param1 >= 100 && param1 <= 999)
	{
		LCD_Send_Data1[8] = param1/100 + '0';
		LCD_Send_Data1[9] = (param1/10)%10 + '0';
		LCD_Send_Data1[10] = param1%10 + '0';
	}
	if(param2 >= 100 && param2 <= 999)
	{
		LCD_Send_Data1[12] = param2/100 + '0';
		LCD_Send_Data1[13] = (param2/10)%10 + '0';
		LCD_Send_Data1[14] = param2%10 + '0';
	}
	HAL_UART_Transmit(&huart3,LCD_Send_Data1,19,1000);
}


void LCD_float_Task(float float1)
{
		if(float1<0) {LCD_Send_Data_float[7] ='-';float1=-float1;}
		else LCD_Send_Data_float[7]='0';
		LCD_Send_Data_float[8] = float1/100 + '0';
		LCD_Send_Data_float[9] = (uint8_t)(float1/10)%10 + '0';
		LCD_Send_Data_float[10] =(uint8_t)(float1)%10 + '0';
		
		
		yawint=(float1-(int)(float1))*1000;
	
		LCD_Send_Data_float[11] = yawint/100 + '0';
		LCD_Send_Data_float[12] =(yawint/10)%10 + '0';
		LCD_Send_Data_float[13] =(yawint)%10 + '0';
		
		//HAL_UART_Transmit(&huart6,LCD_Send_Data_float,17,1000);
}
