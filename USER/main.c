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
 ALIENTEK 探索者STM32F407开发板 FreeRTOS实验13-1
 FreeRTOS队列操作实验-库函数版本
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

extern u8  TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值  

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		256  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define TASK1_TASK_PRIO		2
//任务堆栈大小	
#define TASK1_STK_SIZE 		256  
//任务句柄
TaskHandle_t Task1Task_Handler;
//任务函数
void task1_task(void *pvParameters);

//任务优先级
#define TASK2_TASK_PRIO		2
//任务堆栈大小	
#define TASK2_STK_SIZE 		256  
//任务句柄
TaskHandle_t Task2Task_Handler;
//任务函数
void task2_task(void *pvParameters);

//任务优先级
#define KEYPROCESS_TASK_PRIO 3
//任务堆栈大小	
#define KEYPROCESS_STK_SIZE  256 
//任务句柄
TaskHandle_t Keyprocess_Handler;
//任务函数
void Keyprocess_task(void *pvParameters);


//按键消息队列的数量
#define KEYMSG_Q_NUM    1  		//按键消息队列的数量  
#define MESSAGE_Q_NUM   4   	//发送数据的消息队列的数量 
QueueHandle_t Key_Queue;   		//按键值消息队列句柄
QueueHandle_t Message_Queue;	//信息队列句柄

//查询Message_Queue队列中的总队列数量和剩余队列数量
void check_msg_queue(void)
{
    u8 *p;
	u8 msgq_remain_size;	//消息队列剩余大小
    u8 msgq_total_size;     //消息队列总大小
    
    taskENTER_CRITICAL();   //进入临界区
    msgq_remain_size=uxQueueSpacesAvailable(Message_Queue);//得到队列剩余大小
    msgq_total_size=uxQueueMessagesWaiting(Message_Queue)+uxQueueSpacesAvailable(Message_Queue);//得到队列总大小，总大小=使用+剩余的。
	p=mymalloc(SRAMIN,20);	//申请内存
	sprintf((char*)p,"Total Size:%d",msgq_total_size);	//显示DATA_Msg消息队列总的大小
	sprintf((char*)p,"Remain Size:%d",msgq_remain_size);	//显示DATA_Msg剩余大小
	myfree(SRAMIN,p);		//释放内存
    taskEXIT_CRITICAL();    //退出临界区
}

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);					//初始化延时函数
	uart_init(115200);     				//初始化串口
	
	Encoder_Init();
	
	LED_Init();		        			//初始化LED端口
	KEY_Init();							//初始化按键
	BEEP_Init();						//初始化蜂鸣器
	PWM_Init();
	
	Updata_PWM_(20,200,500,600);

//	TIM9_Int_Init(5000,16800-1);		//初始化定时器9，周期500ms
//	TIM14_PWM_Init(500-1,84-1);       	//84M/84=1Mhz的计数频率计数到500,PWM频率为1M/500=2Khz     
	
	my_mem_init(SRAMIN);            	//初始化内部内存池
	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	
	//创建消息队列
    Key_Queue=xQueueCreate(KEYMSG_Q_NUM,sizeof(u8));        //创建消息Key_Queue
    Message_Queue=xQueueCreate(MESSAGE_Q_NUM,USART_REC_LEN); //创建消息Message_Queue,队列项长度是串口接收缓冲区长度
	
    //创建TASK1任务
    xTaskCreate((TaskFunction_t )task1_task,             
                (const char*    )"task1_task",           
                (uint16_t       )TASK1_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,        
                (TaskHandle_t*  )&Task1Task_Handler);   
		
		//创建TASK2任务								
		xTaskCreate((TaskFunction_t )task2_task,             
                (const char*    )"task2_task",           
                (uint16_t       )TASK2_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TASK2_TASK_PRIO,        
                (TaskHandle_t*  )&Task2Task_Handler); 
								
		//创建Keyprocess任务
    xTaskCreate((TaskFunction_t )Keyprocess_task,     
                (const char*    )"keyprocess_task",   
                (uint16_t       )KEYPROCESS_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )KEYPROCESS_TASK_PRIO,
                (TaskHandle_t*  )&Keyprocess_Handler); 
								
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//task1任务函数
void task1_task(void *pvParameters)
{
	u8 key,i=0;
    BaseType_t err;
	while(1)
	{
		key=KEY_Scan(0);            	//扫描按键
        if((Key_Queue!=NULL)&&(key))   	//消息队列Key_Queue创建成功,并且按键被按下
        {
            err=xQueueSend(Key_Queue,&key,10);
            if(err==errQUEUE_FULL)   	//发送按键值
            {
                printf("队列Key_Queue已满，数据发送失败!\r\n");
            }
        }
        i++;
        if(i%10==0) check_msg_queue();//检Message_Queue队列的容量
        if(i==50)
        {
            i=0;
            LED0=!LED0;
        }
        vTaskDelay(10);                           //延时10ms，也就是10个时钟节拍	
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


//Keyprocess_task函数
void Keyprocess_task(void *pvParameters)
{
	u8 num,key;
	while(1)
	{
        if(Key_Queue!=NULL)
        {
            if(xQueueReceive(Key_Queue,&key,portMAX_DELAY))//请求消息Key_Queue
            {
                switch(key)
                {
                    case WKUP_PRES:		//KEY_UP控制LED1
                        //LED1=!LED1;
                        break;
                    case KEY2_PRES:		//KEY2控制蜂鸣器
                        BEEP=!BEEP;
                        break;
                    case KEY0_PRES:		//KEY0刷新LCD背景
                        num++;
                        break;
                }
            }
        } 
		vTaskDelay(10);      //延时10ms，也就是10个时钟节拍	
	}
}

