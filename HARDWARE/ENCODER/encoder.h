#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>	 

#define ENCODER_CNT 0

extern int Encoder[4];

void Encoder_Init(void);
void TIM2_Encoder_Config(void);
void TIM3_Encoder_Config(void);
void TIM4_Encoder_Config(void);
void TIM8_Encoder_Config(void);
void Read_Encoder(void);
#endif
