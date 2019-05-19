/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.0.3
* Date               : 09/22/2008
* Description        : Main program body
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "OLED.h"
#include "gpio.h"
#include "delay.h"
#include "exti.h"

extern u8 nonside[];
extern u8 nonpos;



//串口初始函数
void USARTTEST_Configuration(void);
/* Private functions ---------------------------------x------------------------*/
void Dly_ms(int ms)
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<500;jj++);  //16MHz--1ms
     //for(jj=0;jj<4006;jj++);  //48MHz--1ms 
     //for(jj=0;jj<5341;jj++);    //64MHz--1ms  
}


/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
	
	RCC_Configuration();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	NVIC_Configuration();
  EXTIX_Init();
	GPIO_UserInit();
	SYSTICK_Init();
  delay_init();
	LCD_Init();
 // AFIO->MAPR |= 1<<25;
/*-----------------------初始化-----------------------*/
	LCD_CLS();
	delay_ms(10);
	LCD_Print(30, 4, "电机PWM调速",TYPE16X16,TYPE8X16);
	LCD_Print(30, 24, "脉冲电磁阀",TYPE16X16,TYPE8X16);
	LCD_Print(30, 48, "AD质量采集",TYPE16X16,TYPE8X16);
	Draw_BMP(4,4,nonside);
//	Draw_BMP(4,24,nonside);
//	Draw_BMP(4,48,nonside);
/*-----------------------end---------------------------*/	
	while(1)
	{
//		LCD_Fill(0xFF);//填充
//		delay_ms(1000);
//		LCD_CLS();
//		LCD_Print(30, 4, "电机PWM调速",TYPE16X16,TYPE8X16);
//		LCD_Print(30, 24, "脉冲电磁阀",TYPE16X16,TYPE8X16);
//		LCD_Print(30, 48, "AD质量采集",TYPE16X16,TYPE8X16);
//		Draw_BMP(4,4,nonside);
//		Draw_BMP(4,24,nonside);
//		Draw_BMP(4,48,nonside);
//		
//		delay_ms(1000);
//		LCD_CLS();//清除全部数据
//		delay_ms(1000);
		
	}

}


/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
