#include "BSP_TIME.h"
//f= 84M/arr*psc*
#define psc(Hz,Arr)  (84*1000000/(Hz*(Arr+1))-1)
void  BSP_TIME_InitConfig(void)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);   //PCLK1=42MHz,TIM5 clk =84MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);   //PCLK1=42MHz,TIM5 clk =84MHz
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);

  //TIM5 摩擦轮	A1,A0	两路PWM波两个拨弹电机 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);	 	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM8);	 
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_5;//|GPIO_Pin_6|GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	 	  
	
	TIM_TimeBaseStructure.TIM_Period = 2499;    
	TIM_TimeBaseStructure.TIM_Prescaler = psc(400,2499);
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);		
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse =0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);	
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);			 
	
  TIM_ARRPreloadConfig(TIM5,ENABLE);		

  TIM_Cmd(TIM8,ENABLE);  
	TIM_Cmd(TIM5,ENABLE);	

}
