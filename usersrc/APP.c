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
//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue 200


extern u8 page;

void Get_Maopi()
{
	Weight_Maopi = Read_HX711();	
} 
void Get_Weight()
{
	Weight_Shiwu = Read_HX711();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//获取净重
	if(Weight_Shiwu > 0)			
	{	
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/GapValue); 	//计算实物的实际重量
																		
																		
		if(Weight_Shiwu > 5000)		//超重报警
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
	//	Flag_ERROR = 1;				//负重报警
	}
	
}
void APP_adc()
{
	u8 buffer[7];
	page = 2;
	LCD_CLS();
	LCD_Print(0, 22, "质量为",TYPE16X16,TYPE8X16);
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
	//该程序使用TM1发0.5s为间隔的脉冲。
	LCD_Print(10, 30, "脉冲正在发生",TYPE16X16,TYPE8X16);
	TIM2_Int_Init(99,24000);	//2400000/24000=100Hz
	page = 2;
}
void APP_PWM()
{
	LCD_CLS();
	LCD_Print(10, 30, "PWM低电平占11%",TYPE16X16,TYPE8X16);
	//该程序默认发出为周期为20ms占空比50%的PWM可以使用
	TIM3_PWM_Init(999,0);	//2400000/1000=2.4kHz
	TIM_SetCompare2(TIM3,110);
	page = 2;
	
}


