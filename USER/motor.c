#include "motor.h"

void PWM_GPIO(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);
}


// PWM ��ʱ������
void Pwm_Init(unsigned long Frequency, uint16_t TimerPeriod, float DC1,float DC2,float DC3,float DC4)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
  TIM_OCInitTypeDef  TIM_OCInitStructure;   
  uint16_t Channel1Pulse = 0,Channel2Pulse = 0,Channel3Pulse = 0,Channel4Pulse = 0;
    //((1+0 )/(60*1000000))*(1+TimerPeriod )=1/f
  //TimerPeriod = (120*1000000 / (Frequency*8)) - 1; 
  Channel1Pulse = (uint16_t) (((uint32_t) DC1 * (TimerPeriod - 1)) / 1000);   
  Channel2Pulse = (uint16_t) (((uint32_t) DC2 * (TimerPeriod - 1)) / 1000); 
  Channel3Pulse = (uint16_t) (((uint32_t) DC3 * (TimerPeriod - 1)) / 1000);   
  Channel4Pulse = (uint16_t) (((uint32_t) DC4 * (TimerPeriod - 1)) / 1000);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);     
  
  /* Time Base configuration */  
  TIM_TimeBaseStructure.TIM_Prescaler = (84*1000000)/(TimerPeriod*Frequency)-1;      //ʱ��Ԥ��Ƶ��
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;    // �Զ���װ�ؼĴ�����ֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;  
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;   //�ظ��Ĵ����������Զ�����pwmռ�ձ�          
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);  
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;  
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;    //ʹ�ܸ�ͨ�����

  //���岽��������ɲ���������ã��߼���ʱ�����еģ�ͨ�ö�ʱ����������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;    // //���û������������
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;  //ʹ�ܻ��������
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;   // //���������״̬??
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;  //�����󻥲������״̬??
  /* TIM1 counter enable */  
  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;    // //����ռ�ձ�ʱ��
  TIM_OC1Init(TIM5, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;    // //����ռ�ձ�ʱ��
  TIM_OC2Init(TIM5, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;    // //����ռ�ձ�ʱ��
  TIM_OC3Init(TIM5, &TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;    // //����ռ�ձ�ʱ��
  TIM_OC4Init(TIM5, &TIM_OCInitStructure);
  
  TIM_Cmd(TIM5, ENABLE); 
  /* TIM1 Main Output Enable */  
  TIM_CtrlPWMOutputs(TIM5, ENABLE);  
}


//PWMռ�ձ�����
void UpdataDuty(unsigned long Frequency,uint16_t TimerPeriod,float DC1,float DC2,float DC3,float DC4)  
{  
  TIM5->CCR1 =(int)(DC1*(TimerPeriod - 1) / 1000);
  TIM5->CCR2 =(int)(DC2*(TimerPeriod - 1) / 1000);
  TIM5->CCR3 =(int)(DC3*(TimerPeriod - 1) / 1000);
  TIM5->CCR4 =(int)(DC4*(TimerPeriod - 1) / 1000);
}
//PWM��ʼ��
void PWM_Init(void)
{
  PWM_GPIO();
  Pwm_Init(50,1000,0,0,0,0);
}
//PWMռ�ձ�����
void Updata_PWM_(float DC1,float DC2,float DC3,float DC4)
{
  UpdataDuty(50,1000,DC1,DC2,DC3,DC4);
}

void Set_Motor(int MotorA,int MotorB,int MotorC,int MotorD)
{
	
	if(MotorA>0)
	{
		MotorA1=0,			MotorA2=1;
		TIM5->CCR2=MotorA;
	}
	else
	{
		MotorA1=1,			MotorA2=0;
		TIM5->CCR2=-MotorA;
	}
	
/////////////////////////////	
	if(MotorB>0)
	{
		MotorB1=0,			MotorB2=1;
		TIM5->CCR1=MotorB;
	}
	else
	{
		MotorB1=1,			MotorB2=0;
		TIM5->CCR1=-MotorB;
	}
///////////////////////////////////////	
	
	if(MotorC>0)
	{
		MotorC1=0,			MotorC2=1;
		TIM5->CCR4=MotorC;
	}
	else
	{
		MotorC1=1,			MotorC2=0;
		TIM5->CCR4=-MotorC;
	}
/////////////////////////////	
	if(MotorD>0)
	{
		MotorD1=0,			MotorD2=1;
		TIM5->CCR3=MotorD;
	}
	else
	{
		MotorD1=1,			MotorD2=0;
		TIM5->CCR3=-MotorD;
	}
}
