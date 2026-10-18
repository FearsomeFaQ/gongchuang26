#include "stm32f4xx_hal.h"
#include <string.h>
#include "motor.h"

#define RXBUFFSIZE4 128
uint8_t uart4_RxBuff[RXBUFFSIZE4];
uint32_t W_X=0;
uint32_t W_Y=0;
/* DMA接收完成回调 */
//uint8_t Rx_buf[50];   //接收缓冲区
//uint16_t Rx_long=0;    //接受数据的长度
//uint8_t Buf_long =50;   //接收缓冲区大小
void SMP_Init(void)
{
	//ʹŜࠕАא׏
	__HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);
	//ߪǴԮࠚޓ˕
	HAL_UART_Receive_DMA(&huart4,uart4_RxBuff,RXBUFFSIZE4);
	//Zסއ׈ȥ£
 
}

	
	
	


