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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
  	EXTI_GenerateSWInterrupt(EXTI_Line1);
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1); 

  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ���ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
 
}

//�ⲿ�ж�0������� 
void EXTI1_IRQHandler(void)
{
	
	unsigned char a;
//	PBout(8) = !PBout(8);
//	LCD_CLS_line_area(0,recpos[nonpos]-4,32);
	
	a = KEY_Scan(1);
//-------------------����1�Ĺ���------------------
	if(a == 1)
	{
		if(nonpos<3 && page == 1)
		{
			LCD_rec_clear(0);//���4-20��
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
//-------------------����2�Ĺ���------------------
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
//-------------------����3�Ĺ���------------------
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
//-------------------����4�Ĺ���------------------
	else if(a == 4)
	{
//		nonpos = 1;
		if(nonpos == 3)
		{
			LCD_CLS();
			//�ر������ɼ�ģ��
			LCD_Print(30, 4, "���PWM����",TYPE16X16,TYPE8X16);
			LCD_Print(30, 24, "�����ŷ�",TYPE16X16,TYPE8X16);
			LCD_Print(30, 48, "AD�����ɼ�",TYPE16X16,TYPE8X16);
			Draw_BMP(4,48,nonside);
			nonpos = 3;
		}
		else if(nonpos == 2)
		{
			LCD_CLS();
			//�ر����巢����ʱ��
//			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//�������־λ
//      TIM_Cmd(TIM2, DISABLE);  //ʧ�ܣ�������ʹ�ܣ�			
			TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE); //ʹ��ָ����TIM3�ж�,��������ж�
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
			LCD_Print(30, 4, "���PWM����",TYPE16X16,TYPE8X16);
			LCD_Print(30, 24, "�����ŷ�",TYPE16X16,TYPE8X16);
			LCD_Print(30, 48, "AD�����ɼ�",TYPE16X16,TYPE8X16);
			Draw_BMP(4,24,nonside);		
	    nonpos = 2;
		}
		else if(nonpos == 1)
		{
			LCD_CLS();
			//clode PWWM Timer
			TIM_SetCompare2(TIM3,1000);
//			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//�������־λ
//      TIM_Cmd(TIM3, DISABLE);  //ʧ�ܣ�������ʹ�ܣ�
//			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			pwm = 110;
			LCD_Print(30, 4, "���PWM����",TYPE16X16,TYPE8X16);
			LCD_Print(30, 24, "�����ŷ�",TYPE16X16,TYPE8X16);
			LCD_Print(30, 48, "AD�����ɼ�",TYPE16X16,TYPE8X16);
			Draw_BMP(4,4,nonside);			
			nonpos = 1;
		}
		page = 1;
	}
	
	
	
//-------------------����жϱ�־λ---------------
		EXTI_ClearITPendingBit(EXTI_Line1);
}
 
