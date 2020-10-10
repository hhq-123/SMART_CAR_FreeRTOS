#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "sys.h"



#define MotorA1	PCout(4)
#define MotorA2	PCout(5)

#define MotorB1	PCout(2)
#define MotorB2	PCout(3)

#define MotorC1	PBout(4)
#define MotorC2	PBout(5)

#define MotorD1	PCout(9)
#define MotorD2	PCout(8)

void PWM_Init(void);
void Updata_PWM_(float DC1,float DC2,float DC3,float DC4);
void Set_Motor(int MotorA,int MotorB,int MotorC,int MotorD);

#endif
