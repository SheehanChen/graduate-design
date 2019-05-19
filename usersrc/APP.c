#include "stm32f10x.h"
#include "app.h"
#include "OLED.h"
#include "timer.h"
#include "hx711.h"
#include "string.h"
#include "stdio.h"

//unsigned int maichong = 100;
unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
long Weight_Shiwu = 0;
unsigned char flag = 0;
u8 Flag_ERROR = 0;
//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
#define GapValue 200


extern u8 page;

void Get_Maopi()
{
	Weight_Maopi = Read_HX711();	
} 
void Get_Weight()
{
	Weight_Shiwu = Read_HX711();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//��ȡ����
	if(Weight_Shiwu > 0)			
	{	
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/GapValue); 	//����ʵ���ʵ������
																		
																		
		if(Weight_Shiwu > 5000)		//���ر���
		{
			Flag_ERROR = 1;	
		}
		else
		{
			Flag_ERROR = 0;
		}
	}
	else
	{
		Weight_Shiwu = 0;
	//	Flag_ERROR = 1;				//���ر���
	}
	
}
void APP_adc()
{
	u8 buffer[7];
	page = 2;
	LCD_CLS();
	LCD_Print(0, 22, "����Ϊ",TYPE16X16,TYPE8X16);
	LCD_Print(90, 22, "g",TYPE16X16,TYPE8X16);
	Get_Maopi();
	while(1)
	{
		Get_Weight();
		if(KEY3==0)
		{
			break;
		}
//		if(Flag_ERROR == 0)
		if(1)
		{
			sprintf((char*)buffer,"%d",(int)Weight_Shiwu);
			LCD_Print(48, 22, buffer,TYPE16X16,TYPE8X16);			
		}
		if(KEY3==0)
		{
			break;
		}
	}
//	page = 2;
}
void APP_maichong()
{
	LCD_CLS();
	//�ó���ʹ��TM1��0.5sΪ��������塣
	LCD_Print(10, 30, "�������ڷ���",TYPE16X16,TYPE8X16);
	TIM2_Int_Init(99,24000);	//2400000/24000=100Hz
	page = 2;
}
void APP_PWM()
{
	LCD_CLS();
	LCD_Print(10, 30, "PWM�͵�ƽռ11%",TYPE16X16,TYPE8X16);
	//�ó���Ĭ�Ϸ���Ϊ����Ϊ20msռ�ձ�50%��PWM����ʹ��
	TIM3_PWM_Init(999,0);	//2400000/1000=2.4kHz
	TIM_SetCompare2(TIM3,110);
	page = 2;
	
}


