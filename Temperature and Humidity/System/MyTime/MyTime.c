#include "MyTime.h"
#include "config.h"
#include "timer.h"

#include "DHT11.h"

static void MyTimer0_Init(void)
{
	TIM_InitTypeDef TIM_InitStructure;
	
	TIM_InitStructure.TIM_Mode		= TIM_16BitAutoReload;	// 16位自动重装
	TIM_InitStructure.TIM_Polity	= PolityLow;			// 低中断优先级
	TIM_InitStructure.TIM_Interrupt	= ENABLE;				// 允许中断
	TIM_InitStructure.TIM_ClkSource	= TIM_CLOCK_1T;			// 指定时钟源
	TIM_InitStructure.TIM_ClkOut	= DISABLE;				// 不输出高速脉冲
	TIM_InitStructure.TIM_Value		= 65536UL - (MAIN_Fosc / My_Timer_Int_Times);	// 初值
	TIM_InitStructure.TIM_Run		= ENABLE;				// 开始运行
	Timer_Inilize(My_Timer, &TIM_InitStructure);
}

void MyTimer_Init(void)
{
	MyTimer0_Init();
}

/********************* Timer0中断函数************************/
void timer0_int (void) interrupt TIMER0_VECTOR
{
	static u16 i = 0;
	
	i++;
	if(i>=My_Timer_Int_Times*10)  // 每10秒读取一次数据
	{
		i = 0;
		Read_DHT11_Error = DataOld_Error;  // 标志数据过旧，需要更新
	}
}