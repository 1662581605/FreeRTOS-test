/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 工程模版
  *********************************************************************
  * @attention
  *
  * 实验平台:野火 STM32全系列开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
/* 开发板硬件bsp头文件 */
#include "bsp_led.h"
#include "bsp_usart.h"


/***************************** 任务句柄 **************************/
/* 
*任务句柄是一个指针，用于指向一个任务，当任务创建好后，它就具有了一个任务句柄
*以后我们想要操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
*这个句柄可以为NULL。
*/
/* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle;
/* LED任务句柄 */
static TaskHandle_t LED_Task_Handle;

/******************************内核对象句柄*************************/
/*
*信号量，消息队列，时间标志组，软件定时器这些都属于内核的对象，要想使用这些内核
*对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，和后续
*我们可以通过这个句柄操作这些内核对象。
*
*内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
*任务间的事件等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数来完成的
*/

/************************** 全局变量的声明 *****************************/
/*
*当我们在写应用程序的时候，可能需要用到一些全局变量。
*/
/* AppTaskCreate任务堆栈 */
static StackType_t AppTaskCreate_Stack[128];
/* LED任务堆栈 */
static StackType_t LED_Task_Stack[128];

/* AppTaskCreate 任务控制块 */
static StackType_t AppTaskCreate_TCB;
/* LED 任务控制块 */
static StackType_t LED_Task_TCB;

/* 空闲任务任务堆栈 */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* 定时器任务堆栈 */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/* 空闲任务控制块 */
static StaticTask_t Idle_Task_TCB;  
/* 定时器任务控制块 */
static StaticTask_t Timer_Task_TCB;
 
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);/* 用于创建任务 */

static void LED_Task(void* pvParameters);/* LED_Task任务实现 */

static void BSP_Init(void);/* 用于初始化板载相关资源 */

/**
  * 使用了静态分配内存，以下这两个函数是由用户实现，函数在task.c文件中有引用
  * 当且仅当 configSUPPORT_STATIC_ALLOCATION 这个宏定义为 1 的时候才有效
  */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
                                    StackType_t **ppxTimerTaskStackBuffer, 
                                    uint32_t *pulTimerTaskStackSize);

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
                                   StackType_t **ppxIdleTaskStackBuffer, 
                                   uint32_t *pulIdleTaskStackSize);
/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化 
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/

int main(void)
{	
    /* 开发板硬件初始化 */
    BSP_Init();
    printf("这是一个[野火]-STM32全系列开发板-FreeRTOS-静态创建单任务!\r\n");
    /* 创建 AppTaskCreate 任务 */
    AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t    )AppTaskCreate,     //任务函数
                                                            (const char*    )"AppTaskCreate",       //任务名称
                                                            (uint32_t       )128,   //任务堆栈大小
                                                            (void*          )NULL,              //传递给任务函数的参数
                                                            (UBaseType_t    )3,     //任务优先级
                                                            (StackType_t*   )AppTaskCreate_Stack,   //任务堆栈
                                                            (StaticTask_t*  )&AppTaskCreate_TCB);   //任务控制块   
}
/********************************END OF FILE****************************/
