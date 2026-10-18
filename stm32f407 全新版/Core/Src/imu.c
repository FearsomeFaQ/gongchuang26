#include "imu.h"

static uint8_t RxBuffer[11];/*接收数据数组*/
static volatile uint8_t RxState = 0;/*接收状态标志位*/
static uint8_t RxIndex = 0;/*接受数组索引*/
float Yaw,gz,Last_Yaw;


void IMU_Init(void)
{	
	uint8_t unlock_register[] = {0xFF, 0xAA, 0x69, 0x88, 0xB5};
	HAL_UART_Transmit(&huart2,unlock_register,5,0xffff);
	
	HAL_Delay(1);

	uint8_t reset_z_axis[] = {0xFF, 0xAA, 0x76, 0x00, 0x00};
	HAL_UART_Transmit(&huart2, reset_z_axis, 5, 0xffff);  

	// HAL_Delay(1);
	
	// uint8_t set_output_200Hz[] = {0xFF, 0xAA, 0x03, 0x0B, 0x00};
	// HAL_UART_Transmit(&huart2, set_output_200Hz, 5, 0xffff);  
	
	// HAL_Delay(1);
	
	// uint8_t set_baudrate_115200[] = {0xFF, 0xAA, 0x04, 0x06, 0x00};
	// HAL_UART_Transmit(&huart2, set_baudrate_115200, 5, 0xffff);  
	
	HAL_Delay(1);
	
	uint8_t restart_device[] = {0xFF, 0xAA, 0x00, 0xFF, 0x00};
	HAL_UART_Transmit(&huart2, restart_device, 5, 0xffff);
}



/**
 * @brief       数据包处理函数
 * @param       串口接收的数据RxData
 * @retval      无
 */
void ReceiveData(uint8_t RxData)
{
	uint8_t i,sum=0;
	if (RxState == 0)	//等待包头
	{
		if (RxData == 0x55)	//收到包头
		{
			RxBuffer[RxIndex] = RxData;
			RxState = 1;
			RxIndex = 1;	//进入下一状态
		}
	}
	else if (RxState == 1)
	{
		if (RxData == 0x53)	/*判断数据内容，修改这里可以改变要读的数据内容，0x53为角度输出*/
		{
			RxBuffer[RxIndex] = RxData;
			RxState = 2;
			RxIndex = 2;	//进入下一状态
		}
		else if (RxData == 0x52)
		{
			RxBuffer[RxIndex] = RxData;
			RxState = 3;
			RxIndex = 2;	//进入下一状态
		}
	}
	else if (RxState == 2)	//接收数据
	{
		RxBuffer[RxIndex++] = RxData;
		if(RxIndex == 11)	//接收完成
		{
			for(i=0;i<10;i++)
			{
				sum = sum + RxBuffer[i];	//计算校验和
			}
			if(sum == RxBuffer[10])		//校验成功
			{
				/*计算数据，根据数据内容选择对应的计算公式*/
				
				Yaw = ((float) ( (int16_t) RxBuffer[7] << 8 | (int16_t) RxBuffer[6])) / 32768.0f * 180.0f;
				
				Last_Yaw = Yaw;
				
				if(Yaw - Last_Yaw > 180)
				{
					Yaw -= 360;
				}
				else if(Yaw - Last_Yaw < -180)
				{
					Yaw += 360;
				}
			}
			RxState = 0;
			RxIndex = 0;	//读取完成，回到最初状态，等待包头
		}
	}
	else if (RxState == 3)
	{
		RxBuffer[RxIndex++] = RxData;
		if(RxIndex == 11)	//接收完成
		{
			for(i=0;i<10;i++)
			{
				sum = sum + RxBuffer[i];	//计算校验和
			}
			if(sum == RxBuffer[10])		//校验成功
			{
				/*计算数据，根据数据内容选择对应的计算公式*/
				
				gz = ((int16_t) ((int16_t) RxBuffer[7] << 8 | (int16_t) RxBuffer[6])) / 32768.0f * 2000.0f;
			}
			RxState = 0;
			RxIndex = 0;	//读取完成，回到最初状态，等待包头
		}
	}
}




//void IMU_Updata(void)
//{
//	if(imu_flag == 1 && imu_data[0] == 0x55 && imu_data[1] == 0x53)
//	{
//		Yaw = (float)(((imu_data[8] << 8) | imu_data[7]) / 32768.0 * 180);//解析航向角数据
//		
//		if(Yaw - Last_Yaw > 180)
//	    {
//			Yaw -= 360;
//		}
//		else if(Yaw - Last_Yaw < -180)
//		{
//			Yaw += 360;//处理航向角的跳变问题
//		}
//		Last_Yaw = Yaw;                               //
//		imu_flag = 0;	
//}
//	else if(imu_flag == 1 && imu_data[0] == 0x55 && imu_data[1] == 0x52)
//	{
//		gz=(float)(((imu_data[8] << 8) | imu_data[7]) / 32768.0 * 2000);
//		imu_flag = 0;
//	}

//}

//void IMU_Proc(void)
//{
//	IMU_Start_Receive();
//    IMU_Updata();
//   float Yaw_g=Yaw+gz*0.005f;
//   	HAL_Delay(5);
//}



