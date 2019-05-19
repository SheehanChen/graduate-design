#include "delay.h"
#include "stm32f10x.h"
#include "hx711.h"

unsigned long Read_HX711(void)  //读HX711芯片输出的数据。 
{ 
	unsigned long val = 0; 
	unsigned char i = 0; 

	GPIO_SetBits(GPIOA,GPIO_Pin_7);	   //DOUT=1 
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);	   //SCK=0 
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7));   //等待DOUT=0  
	delay_us(1); 
	for(i=0;i<24;i++) 
	{ 
		GPIO_SetBits(GPIOA,GPIO_Pin_6);	   //SCK=1 
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);	   //SCK=0 
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))   //DOUT=1 
		val++; 
		delay_us(1); 
	} 
	GPIO_SetBits(GPIOA,GPIO_Pin_6); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(GPIOA,GPIO_Pin_6); 
	delay_us(1);  
	return val; 	
} 


