/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "imu.h"
#include "motor.h"
#include "smp.h"
#include "string.h"
#include "stdio.h"
#include "motor.h"
#include "Control.h"
#include "Res_pi.h"
#include "Servo.h"
#include "IMU.h"
#include "LCD.h"

#include "tjc_usart_hmi.h"

#define FRAME_LENGTH 7

#define WHEEL_DIAMETER_CM 75.0f
#define PULSE_PER_ROUND 3200 // 每圈脉冲数



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

//uint8_t g_oledstring[30];
extern uint8_t g_usart2_receivedata;

extern int g_usart4_receivedata;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



volatile uint8_t rx_end_flag;
volatile uint8_t rx_len;
uint8_t rx_buff[200];
volatile uint8_t tx_end_flag=0;
volatile uint8_t tx_len;
uint8_t tx_buff[200];
float Send_Data1[20];



// 在UART4_APP中调用处理函数
void UART4_APP(void)
{  
    if(rx_end_flag == 1)
    {
			uint8_t tem;
        // 逐字节解析（假设这1字节是有效数据）
			HAL_UART_Transmit(&huart4,rx_buff,rx_len,200);
	   
        for(uint16_t i=0; i<rx_len; i++)
        {
					tem=rx_buff[i];
            PI_Receive_Data(tem);
					//printf("%c",rx_buff[i]);
            if(RxFlag) break;
					
        }
				
						//printf("%c",PI_Data0[1]);
				
				
      memset(rx_buff, 0, sizeof(rx_buff));
        rx_len = 0;
        rx_end_flag = 0;
        HAL_UART_Receive_DMA(&huart4, rx_buff, sizeof(rx_buff));
    }
}


//void USART6_APP(void)
//{   
//	if(tx_end_flag==1)
//	{
//		//Motor_SpeedMode(5,100,10);
//        HAL_UART_Transmit(&huart6,tx_buff,tx_len,200);
//        printf("\r\n");
//	    memset(tx_buff,0,tx_len);
//	    tx_len=0;
//	    tx_end_flag=0;
//	    HAL_UART_Receive_DMA(&huart6,tx_buff,200);
//	}
//}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_UART4_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_UART_Receive_IT(&huart2,&g_usart2_receivedata,1); 
	HAL_UART_Receive_IT(&huart3, RxBuffer, 1);	
	__HAL_UART_ENABLE_IT(&huart4,UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart4,rx_buff,14); 

	IMU_Init();
	Motor_Init();
	LCD_Init();
  Servo_Init();
  

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
			//printf("%d\r\n",g_usart4_receivedata);
		PI_Receive_Proc();
		//HAL_UART_Transmit(&huart4,(uint8_t*)"01",2,0xffff);
		Control_Proc();
			//printf("%d 111",PI_Data0);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	
  }
	
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


		int fputc(int ch,  FILE *f)
{
    while(HAL_UART_Transmit_IT(&huart4, (unsigned char *)&ch, 1)!=HAL_OK){};
    return ch;
}

int fgetc(FILE *f)
{
	uint8_t ch;
   HAL_UART_Receive(&huart4,&ch,1,0xffff);
	return ch;
}



//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//    if (htim->Instance == TIM6) { 
//        // 1. 先更新传感器数据（如获取当前yaw角）
//        ReceiveData(g_usart2_receivedata);
//			HAL_UART_Receive_IT(&huart2,&g_usart2_receivedata,1);
//        
//        // 2. 执行电机控制逻辑（包含PID计算和速度输出）
//        go(200);  
//    }
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
