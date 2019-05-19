#include "exti.h"
//#include "key.h"
#include "delay.h"
#include "gpio.h"
#include "OLED.h"
#include "APP.h"
#include "stdio.h"
#include "string.h"

u8 nonpos = 1;
extern u8 nonside[];
u8 page = 1;
u8 recpos[4] = {0,4,24,48};
unsigned int pwm = 110;
unsigned char buffer[5];
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//外部中断0服务程序
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
  	EXTI_GenerateSWInterrupt(EXTI_Line1);
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1); 

  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
 
}

//外部中断0服务程序 
void EXTI1_IRQHandler(void)
{
	
	unsigned char a;
//	PBout(8) = !PBout(8);
//	LCD_CLS_line_area(0,recpos[nonpos]-4,32);
	
	a = KEY_Scan(1);
//-------------------按键1的功能------------------
	if(a == 1)
	{
		if(nonpos<3 && page == 1)
		{
			LCD_rec_clear(0);//清除4-20列
			nonpos++;
			Draw_BMP(4,recpos[nonpos],nonside);	
		}
		if(page == 2 && nonpos == 1 && pwm > 100)
		{
			pwm-=10;
			sprintf((char*)buffer,"%d",pwm/10);
			LCD_Print(98, 30, buffer,TYPE16X16,TYPE8X16);
			TIM_SetCompare2(TIM3,pwm);
			LCD_Print(115, 30,"%",TYPE16X16,TYPE8X16);
		}	
	}
//-------------------按键2的功能------------------
	else if(a == 2)
	{
		if (nonpos>1 && page ==1)
		{
			LCD_rec_clear(0);
			nonpos--;
			Draw_BMP(4,recpos[nonpos],nonside);				
		}
		if(page == 2 && nonpos == 1 && pwm < 990)
		{
			pwm+=10;
			sprintf((char*)buffer,"%d",pwm/10);
			LCD_Print(98, 30, buffer,TYPE16X16,TYPE8X16);
			TIM_SetCompare2(TIM3,pwm);
			LCD_Print(115, 30,"%",TYPE16X16,TYPE8X16);
		}	
	}
//-------------------按键3的功能------------------
	else if(a == 3)
	{
		if(nonpos == 3)
		{
			APP_adc();
		}
		else if(nonpos == 2)
		{
			APP_maichong();
		}
		else if(nonpos == 1)
		{
			APP_PWM();
		}
	}
//-------------------按键4的功能------------------
	else if(a == 4)
	{
//		nonpos = 1;
		if(nonpos == 3)
		{
			LCD_CLS();
			//关闭质量采集模块
			LCD_Print(30, 4, "电机PWM调速",TYPE16X16,TYPE8X16);
			LCD_Print(30, 24, "脉冲电磁阀",TYPE16X16,TYPE8X16);
			LCD_Print(30, 48, "AD质量采集",TYPE16X16,TYPE8X16);
			Draw_BMP(4,48,nonside);
			nonpos = 3;
		}
		else if(nonpos == 2)
		{
			LCD_CLS();
			//关闭脉冲发生定时器
//			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//再清除标志位
//      TIM_Cmd(TIM2, DISABLE);  //失能（函数外使能）			
			TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE); //使能指定的TIM3中断,允许更新中断
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
			LCD_Print(30, 4, "电机PWM调速",TYPE16X16,TYPE8X16);
			LCD_Print(30, 24, "脉冲电磁阀",TYPE16X16,TYPE8X16);
			LCD_Print(30, 48, "AD质量采集",TYPE16X16,TYPE8X16);
			Draw_BMP(4,24,nonside);		
	    nonpos = 2;
		}
		else if(nonpos == 1)
		{
			LCD_CLS();
			//clode PWWM Timer
			TIM_SetCompare2(TIM3,1000);
//			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//再清除标志位
//      TIM_Cmd(TIM3, DISABLE);  //失能（函数外使能）
//			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			pwm = 110;
			LCD_Print(30, 4, "电机PWM调速",TYPE16X16,TYPE8X16);
			LCD_Print(30, 24, "脉冲电磁阀",TYPE16X16,TYPE8X16);
			LCD_Print(30, 48, "AD质量采集",TYPE16X16,TYPE8X16);
			Draw_BMP(4,4,nonside);			
			nonpos = 1;
		}
		page = 1;
	}
	
	
	
//-------------------清除中断标志位---------------
		EXTI_ClearITPendingBit(EXTI_Line1);
}
 
