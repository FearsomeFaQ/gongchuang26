		#include "Delay.h"


void delay(float juli,float	sudu,int jiasudu)
{
    int t=juli*1.0f/sudu*1.0f;
	HAL_Delay(t*1000);	
	
}

void delay1(float angle,float sudu,int jiasudu)
{
	int s=angle*165.0f;
    int t=s*1.0f/sudu*1.0f*23.6f;
	HAL_Delay(t*1000*60);	
	
}



