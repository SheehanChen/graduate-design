/******************** (C) COPYRIGHT 2009 Hangzhou Hikvision Digital Technology Co.,Ltd ********************
* File Name              : gpio.c
* Author                 : AF Application Team
* Date First Issued  : 02/06/2009
* Description        : This file includes the peripherals header files in the user application.
********************************************************************************
* History:
* 02/06/2009: V0.1
********************************************************************************
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
//#include "stm32f10x_lib.h"
#include "gpio.h"
#include <stdio.h>
#include "stm32f10x.h"
#include "delay.h"
#include "exti.h"

st_systime g_systime = {0, 0};//系统时间

/*******************************************************************************
* Function Name   : SYSTICK_init
* Description     : 
*                
* Input            :
* Output          : None												
* Return          : None
*******************************************************************************/
void SYSTICK_Init(void)
{
	/* 时钟频率为24MHz的单片机 */
	/* SysTick end of count event each 1ms with input clock equal to 3MHz (HCLK/8, default) */
	SysTick->LOAD=3000;
//	SysTick_SetReload(3000);
//	#if 0
//	/* 时钟频率为36MHz的单片机 */
//	else if(g_mcu_clk == MCU_HCLK_36)
//	{
//		printf("\r\nSysTick_SetReload(4500)");
//		/* SysTick end of count event each 1ms with input clock equal to 3MHz (HCLK/8, default) */
//		SysTick_SetReload(4500);
//	}
//	#endif
	/* Enable SysTick interrupt */
//	SysTick_ITConfig(ENABLE);
//	SysTick->CTRL|=0x0001;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	/* Enable the SysTick Counter */
//	SysTick_CounterCmd(SysTick_Counter_Enable);
	

}

void SYSTEMTICK_Isr(void)
{
	static u16 millisec  = 0;
	
	g_systime.millisec++;

	if(millisec++ >= 1000)
	{
		millisec = 0;	
		g_systime.sec++;
	} 


}


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure_1;
	GPIO_InitTypeDef  GPIO_InitStructure_2;
	GPIO_InitTypeDef  GPIO_InitStructure_3;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	//OLED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOB1,2,3,4
	PBout(8) = 0;
/*------------------------------------GPIOB12-15 keyinput--------------------------------------------*/	
//	GPIO_InitTypeDef  GPIO_InitStructure_1;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 //使能PB,PE端口时钟
	GPIO_InitStructure_1.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	 
 	GPIO_InitStructure_1.GPIO_Mode = GPIO_Mode_IPU; 		 //推挽输出
	GPIO_InitStructure_1.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure_1);	  //初始化GPIOB15,12,13,14

/*-------------------------------------外部中断PB0-----------------------------------------------------*/
	//按键初始化函数
// 	GPIO_InitTypeDef GPIO_InitStructure_2;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO ,ENABLE);	 //使能PB,PE端口时钟
	GPIO_InitStructure_2.GPIO_Pin = GPIO_Pin_1;	 
 	GPIO_InitStructure_2.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	GPIO_InitStructure_2.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure_2);	  //初始化



/*-------------------------------------时钟PB0-----------------------------------------------------*/
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟
	GPIO_InitStructure_3.GPIO_Pin = GPIO_Pin_6;	 //SCK 
	GPIO_InitStructure_3.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure_3.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA,&GPIO_InitStructure_3); 

	GPIO_InitStructure_3.GPIO_Pin = GPIO_Pin_7;	 //DOUT 
	GPIO_InitStructure_3.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	  
	GPIO_InitStructure_3.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz 
	GPIO_Init(GPIOA, &GPIO_InitStructure_3);



} 

/*******************************************************************************
* Function Name  : void GPIO_UserInit(void)
* Description    : Init  The IO PORT
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_UserInit(void)
{
   GPIO_Configuration();
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
		
	/* RCC system reset */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (SUCCESS == HSEStartUpStatus)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 

		/* PCLK1 = HCLK */
		RCC_PCLK1Config(RCC_HCLK_Div1);

		/* 时钟频率为24MHz的单片机 */
		/* PLLCLK = 8MHz / 2 * 6 = 24 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_6);
		
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)					
		{
		}
	}
		
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);
//	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);
//	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PC,D,G端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);
	/*Enable  ADC1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); 
}

//void EXTI_Configuration(void)
//{
//    EXTI_InitTypeDef  EXTI_InitStructure;						

//  	
//}
/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
	
//	NVIC_InitTypeDef NVIC_InitStructure;
	#ifdef  VECT_TAB_RAM  							
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  	
	#else  

	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); 
	#endif
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	  			

	
	/* Set SysTick interrupt vector Preemption Priority to 1 */
//	NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 2, 0);
	NVIC_SetPriority(SysTick_IRQn, 1);
	
//	NVIC_SetPriority(SysTick_IRQn, n);
//n=0x00~0x03 设置Systick为抢占优先级0
//n=0x04~0x07 设置Systick为抢占优先级1
//n=0x08~0x0B 设置Systick为抢占优先级2
//n=0x0C~0x0F 设置Systick为抢占优先级3 

}

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	u8 res;
	if(mode)key_up=1;  //支持连按		  
	while(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0))
	{
		delay_ms(10);//去抖动 
//		if(KEY0==0)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//		else if(KEY2==0)return KEY2_PRES;
//		else if(KEY3==0)return KEY3_PRES;
		if(KEY0==0)res = 1;
		else if(KEY1==0)res = 2;
		else if(KEY2==0)res = 3;
		else if(KEY3==0)res = 4;
		while(KEY0==0&&KEY1==0&&KEY2==0&&KEY3==0)break;
	}  
	
 	return res;//
}

/******************* (C) COPYRIGHT 2009 Hangzhou Hikvision Digital Technology Co.,Ltd *****END OF FILE****/

