/******************** (C) COPYRIGHT 2009 Hangzhou Hikvision Digital Technology Co.,Ltd ********************
* File Name              : systime.h
* Author                   : AF Application Team
* Date First Issued  : 02/06/2009
* Description        : This file includes the peripherals header files in the user application.
********************************************************************************
* History:
* 02/06/2009: V0.1
********************************************************************************

*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __GPIO_H
#define __GPIO_H
#include "sys.h"
/*
*********************************************************************************************************
*                                            LOCAL GPIO DEFINES FOR  FASTDOME BOARD
*********************************************************************************************************
*/                                                              /* -------------------- GPIOA PINS -------------------- */
#define KEY0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//��ȡ����2 
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//��ȡ����3 

#define KEY0_PRES 1	//KEY0����
#define KEY1_PRES	2	//KEY1����
#define KEY2_PRES	3	//KEY2����
#define KEY3_PRES 4	//KEY_3����

extern void GPIO_Configuration(void);
extern void GPIO_UserInit(void);
/* Private function prototypes -----------------------------------------------*/
//ϵͳʱ�ӳ�ʼ������
extern void RCC_Configuration(void);
//�ж�������ʼ������
extern void NVIC_Configuration(void);
//�ж�����
extern void EXTI_Configuration(void);
extern void  SYSTICK_Init(void);
extern void SYSTEMTICK_Isr(void);
u8 KEY_Scan(u8 mode);




/**
 *  @brief st_systime ϵͳʱ��
 */ 
typedef struct{
	unsigned long millisec;	
	unsigned long sec;		
}st_systime;
   
/* ȫ�ֱ�������*/
extern st_systime g_systime; //ϵͳʱ��

#endif

/******************* (C) COPYRIGHT 2009 Hangzhou Hikvision Digital Technology Co.,Ltd *****END OF FILE****/


