  #include "zhuazi.h"
	
	

	

	
void SetAngle(uint16_t angle)
{
	
  uint32_t pulse = 340 + (uint32_t)(angle * 1290.0f / 270.0f);
  
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);
	
	
		HAL_Delay(1);
}


void zhua()
{
	
	
uint16_t pwmVal=340;
	
	SetAngle(90);
		
  HAL_Delay(1000);
  
  SetAngle(30);
  
  
  HAL_Delay(1000);
  
  SetAngle(120);
  
  HAL_Delay(5000);
  
    while (pwmVal < 1600)
  {
    pwmVal++;
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, pwmVal);
    HAL_Delay(1);
  }
  while (pwmVal > 340)
  {
    pwmVal--;
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, pwmVal);
    HAL_Delay(1);
  }
	 
}
