/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 ����ģ��
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32ȫϵ�п����� 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
/* ������Ӳ��bspͷ�ļ� */
#include "bsp_led.h"
#include "bsp_usart.h"


/***************************** ������ **************************/
/* 
*��������һ��ָ�룬����ָ��һ�����񣬵����񴴽��ú����;�����һ��������
*�Ժ�������Ҫ�������������Ҫͨ�������������������������������Լ�����ô
*����������ΪNULL��
*/
/* ���������� */
static TaskHandle_t AppTaskCreate_Handle;
/* LED������ */
static TaskHandle_t LED_Task_Handle;

/******************************�ں˶�����*************************/
/*
*�ź�������Ϣ���У�ʱ���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
*���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬�ͺ���
*���ǿ���ͨ��������������Щ�ں˶���
*
*�ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
*�������¼��ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ�������ɵ�
*/

/************************** ȫ�ֱ��������� *****************************/
/*
*��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
*/
/* AppTaskCreate�����ջ */
static StackType_t AppTaskCreate_Stack[128];
/* LED�����ջ */
static StackType_t LED_Task_Stack[128];

/* AppTaskCreate ������ƿ� */
static StackType_t AppTaskCreate_TCB;
/* LED ������ƿ� */
static StackType_t LED_Task_TCB;

/* �������������ջ */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* ��ʱ�������ջ */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

/* ����������ƿ� */
static StaticTask_t Idle_Task_TCB;  
/* ��ʱ��������ƿ� */
static StaticTask_t Timer_Task_TCB;
 
/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void LED_Task(void* pvParameters);/* LED_Task����ʵ�� */

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

/**
  * ʹ���˾�̬�����ڴ棬�������������������û�ʵ�֣�������task.c�ļ���������
  * ���ҽ��� configSUPPORT_STATIC_ALLOCATION ����궨��Ϊ 1 ��ʱ�����Ч
  */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
                                    StackType_t **ppxTimerTaskStackBuffer, 
                                    uint32_t *pulTimerTaskStackSize);

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
                                   StackType_t **ppxIdleTaskStackBuffer, 
                                   uint32_t *pulIdleTaskStackSize);
/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/

int main(void)
{	
    /* ������Ӳ����ʼ�� */
    BSP_Init();
    printf("����һ��[Ұ��]-STM32ȫϵ�п�����-FreeRTOS-��̬����������!\r\n");
    /* ���� AppTaskCreate ���� */
    AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t    )AppTaskCreate,     //������
                                                            (const char*    )"AppTaskCreate",       //��������
                                                            (uint32_t       )128,   //�����ջ��С
                                                            (void*          )NULL,              //���ݸ��������Ĳ���
                                                            (UBaseType_t    )3,     //�������ȼ�
                                                            (StackType_t*   )AppTaskCreate_Stack,   //�����ջ
                                                            (StaticTask_t*  )&AppTaskCreate_TCB);   //������ƿ�   
}
/********************************END OF FILE****************************/
