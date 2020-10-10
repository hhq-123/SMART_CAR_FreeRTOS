#include "encoder.h"

int Encoder[4];


void Encoder_Init(void)
{
	TIM2_Encoder_Config();
	TIM3_Encoder_Config();
	TIM4_Encoder_Config();
	TIM8_Encoder_Config();
}

/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void TIM2_Encoder_Config(void)//https://www.cnblogs.com/linxw-blog/p/12653743.html
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;      

    //PA5 ch1  A,PB3 ch2 B 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure); 
          
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;//GPIO_Mode_IN;//GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);

    TIM_DeInit(TIM2);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = ENCODER_CNT*4;  
    TIM_TimeBaseStructure.TIM_Prescaler = 0; 
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);              

    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge); 
    TIM_ICStructInit(&TIM_ICInitStructure); 
    TIM_ICInitStructure.TIM_ICFilter = 6;   
    TIM_ICInit(TIM2, &TIM_ICInitStructure); 

    TIM_ClearFlag(TIM2, TIM_FLAG_Update); 
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
    TIM2->CNT = 0;//10000
    TIM_Cmd(TIM2, ENABLE);  
}

/**************************************************************************
函数功能：把TIM3初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void TIM3_Encoder_Config(void)//https://www.cnblogs.com/linxw-blog/p/12653743.html
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;      

    //PA6 ch1  A,PA7 ch2 B 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;        
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;//GPIO_Mode_IN;//GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                           

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);

    TIM_DeInit(TIM3);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = ENCODER_CNT*4;  
    TIM_TimeBaseStructure.TIM_Prescaler = 0; 
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              

    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge); 
    TIM_ICStructInit(&TIM_ICInitStructure); 
    TIM_ICInitStructure.TIM_ICFilter = 6;   
    TIM_ICInit(TIM3, &TIM_ICInitStructure); 

    TIM_ClearFlag(TIM3, TIM_FLAG_Update); 
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
    TIM3->CNT = 0;//10000
    TIM_Cmd(TIM3, ENABLE);  
}

/**************************************************************************
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void TIM4_Encoder_Config(void)//https://www.cnblogs.com/linxw-blog/p/12653743.html
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;      

    //PD12 ch1  A,PD13 ch2 B 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;        
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;//GPIO_Mode_IN;//GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);                           

    GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);

    TIM_DeInit(TIM4);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = ENCODER_CNT*4;  
    TIM_TimeBaseStructure.TIM_Prescaler = 0; 
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              

    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge); 
    TIM_ICStructInit(&TIM_ICInitStructure); 
    TIM_ICInitStructure.TIM_ICFilter = 6;   
    TIM_ICInit(TIM4, &TIM_ICInitStructure); 

    TIM_ClearFlag(TIM4, TIM_FLAG_Update); 
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); 
    TIM4->CNT = 0;//10000
    TIM_Cmd(TIM4, ENABLE);  
}

/**************************************************************************
函数功能：把TIM8初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void TIM8_Encoder_Config(void)//https://www.cnblogs.com/linxw-blog/p/12653743.html
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;      

    //PA0 ch1  A,PA1 ch2 B 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;        
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;//GPIO_Mode_IN;//GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);                           

    GPIO_PinAFConfig(GPIOC,GPIO_PinSource0,GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource1,GPIO_AF_TIM8);

    TIM_DeInit(TIM8);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = ENCODER_CNT*4;  
    TIM_TimeBaseStructure.TIM_Prescaler = 0; 
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);              

    TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge); 
    TIM_ICStructInit(&TIM_ICInitStructure); 
    TIM_ICInitStructure.TIM_ICFilter = 6;   
    TIM_ICInit(TIM8, &TIM_ICInitStructure); 

    TIM_ClearFlag(TIM8, TIM_FLAG_Update); 
    TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE); 
    TIM8->CNT = 0;//10000
    TIM_Cmd(TIM8, ENABLE);  
}

/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
void Read_Encoder(void)
{
	Encoder[0]= (short)TIM2 -> CNT;  TIM2 -> CNT=0;//后左轮
	Encoder[1]= (short)TIM3 -> CNT;  TIM3 -> CNT=0;//后右轮
	Encoder[2]= (short)TIM4 -> CNT;  TIM4 -> CNT=0;//前右轮
	Encoder[3]= (short)TIM8 -> CNT;  TIM8 -> CNT=0;//前左轮
}
