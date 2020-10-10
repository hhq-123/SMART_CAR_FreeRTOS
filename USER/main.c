#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "led.h"
#include "timer.h"
#include "key.h"
#include "beep.h"
#include "encoder.h"
#include "motor.h"

#include "string.h"
#include "malloc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/************************************************
 ALIENTEK ̽����STM32F407������ FreeRTOSʵ��13-1
 FreeRTOS���в���ʵ��-�⺯���汾
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ  

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		256  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define TASK1_TASK_PRIO		2
//�����ջ��С	
#define TASK1_STK_SIZE 		256  
//������
TaskHandle_t Task1Task_Handler;
//������
void task1_task(void *pvParameters);

//�������ȼ�
#define TASK2_TASK_PRIO		2
//�����ջ��С	
#define TASK2_STK_SIZE 		256  
//������
TaskHandle_t Task2Task_Handler;
//������
void task2_task(void *pvParameters);

//�������ȼ�
#define KEYPROCESS_TASK_PRIO 3
//�����ջ��С	
#define KEYPROCESS_STK_SIZE  256 
//������
TaskHandle_t Keyprocess_Handler;
//������
void Keyprocess_task(void *pvParameters);


//������Ϣ���е�����
#define KEYMSG_Q_NUM    1  		//������Ϣ���е�����  
#define MESSAGE_Q_NUM   4   	//�������ݵ���Ϣ���е����� 
QueueHandle_t Key_Queue;   		//����ֵ��Ϣ���о��
QueueHandle_t Message_Queue;	//��Ϣ���о��

//��ѯMessage_Queue�����е��ܶ���������ʣ���������
void check_msg_queue(void)
{
    u8 *p;
	u8 msgq_remain_size;	//��Ϣ����ʣ���С
    u8 msgq_total_size;     //��Ϣ�����ܴ�С
    
    taskENTER_CRITICAL();   //�����ٽ���
    msgq_remain_size=uxQueueSpacesAvailable(Message_Queue);//�õ�����ʣ���С
    msgq_total_size=uxQueueMessagesWaiting(Message_Queue)+uxQueueSpacesAvailable(Message_Queue);//�õ������ܴ�С���ܴ�С=ʹ��+ʣ��ġ�
	p=mymalloc(SRAMIN,20);	//�����ڴ�
	sprintf((char*)p,"Total Size:%d",msgq_total_size);	//��ʾDATA_Msg��Ϣ�����ܵĴ�С
	sprintf((char*)p,"Remain Size:%d",msgq_remain_size);	//��ʾDATA_Msgʣ���С
	myfree(SRAMIN,p);		//�ͷ��ڴ�
    taskEXIT_CRITICAL();    //�˳��ٽ���
}

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init(168);					//��ʼ����ʱ����
	uart_init(115200);     				//��ʼ������
	
	Encoder_Init();
	
	LED_Init();		        			//��ʼ��LED�˿�
	KEY_Init();							//��ʼ������
	BEEP_Init();						//��ʼ��������
	PWM_Init();
	
	Updata_PWM_(20,200,500,600);

//	TIM9_Int_Init(5000,16800-1);		//��ʼ����ʱ��9������500ms
//	TIM14_PWM_Init(500-1,84-1);       	//84M/84=1Mhz�ļ���Ƶ�ʼ�����500,PWMƵ��Ϊ1M/500=2Khz     
	
	my_mem_init(SRAMIN);            	//��ʼ���ڲ��ڴ��
	
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
	
	//������Ϣ����
    Key_Queue=xQueueCreate(KEYMSG_Q_NUM,sizeof(u8));        //������ϢKey_Queue
    Message_Queue=xQueueCreate(MESSAGE_Q_NUM,USART_REC_LEN); //������ϢMessage_Queue,��������Ǵ��ڽ��ջ���������
	
    //����TASK1����
    xTaskCreate((TaskFunction_t )task1_task,             
                (const char*    )"task1_task",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler);   
		
		//����TASK2����								
		xTaskCreate((TaskFunction_t )task2_task,             
                (const char*    )"task2_task",           
                (uint16_t       )TASK2_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK2_TASK_PRIO,        
                (TaskHandle_t*  )&Task2Task_Handler); 
								
		//����Keyprocess����
    xTaskCreate((TaskFunction_t )Keyprocess_task,     
                (const char*    )"keyprocess_task",   
                (uint16_t       )KEYPROCESS_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )KEYPROCESS_TASK_PRIO,
                (TaskHandle_t*  )&Keyprocess_Handler); 
								
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//task1������
void task1_task(void *pvParameters)
{
	u8 key,i=0;
    BaseType_t err;
	while(1)
	{
		key=KEY_Scan(0);            	//ɨ�谴��
        if((Key_Queue!=NULL)&&(key))   	//��Ϣ����Key_Queue�����ɹ�,���Ұ���������
        {
            err=xQueueSend(Key_Queue,&key,10);
            if(err==errQUEUE_FULL)   	//���Ͱ���ֵ
            {
                printf("����Key_Queue���������ݷ���ʧ��!\r\n");
            }
        }
        i++;
        if(i%10==0) check_msg_queue();//��Message_Queue���е�����
        if(i==50)
        {
            i=0;
            LED0=!LED0;
        }
        vTaskDelay(10);                           //��ʱ10ms��Ҳ����10��ʱ�ӽ���	
	}
}

void task2_task(void *pvParameters)
{
	int temp=0;  
	
	while(1)
	{
		temp = (short)TIM3 -> CNT;
		TIM3 -> CNT=0;
		printf("%d\n", temp);
		delay_ms(10);
	}
}


//Keyprocess_task����
void Keyprocess_task(void *pvParameters)
{
	u8 num,key;
	while(1)
	{
        if(Key_Queue!=NULL)
        {
            if(xQueueReceive(Key_Queue,&key,portMAX_DELAY))//������ϢKey_Queue
            {
                switch(key)
                {
                    case WKUP_PRES:		//KEY_UP����LED1
                        //LED1=!LED1;
                        break;
                    case KEY2_PRES:		//KEY2���Ʒ�����
                        BEEP=!BEEP;
                        break;
                    case KEY0_PRES:		//KEY0ˢ��LCD����
                        num++;
                        break;
                }
            }
        } 
		vTaskDelay(10);      //��ʱ10ms��Ҳ����10��ʱ�ӽ���	
	}
}

