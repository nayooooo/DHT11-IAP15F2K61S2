#include "MyTime.h"
#include "config.h"
#include "timer.h"

#include "DHT11.h"

static void MyTimer0_Init(void)
{
	TIM_InitTypeDef TIM_InitStructure;
	
	TIM_InitStructure.TIM_Mode		= TIM_16BitAutoReload;	// 16λ�Զ���װ
	TIM_InitStructure.TIM_Polity	= PolityLow;			// ���ж����ȼ�
	TIM_InitStructure.TIM_Interrupt	= ENABLE;				// �����ж�
	TIM_InitStructure.TIM_ClkSource	= TIM_CLOCK_1T;			// ָ��ʱ��Դ
	TIM_InitStructure.TIM_ClkOut	= DISABLE;				// �������������
	TIM_InitStructure.TIM_Value		= 65536UL - (MAIN_Fosc / My_Timer_Int_Times);	// ��ֵ
	TIM_InitStructure.TIM_Run		= ENABLE;				// ��ʼ����
	Timer_Inilize(My_Timer, &TIM_InitStructure);
}

void MyTimer_Init(void)
{
	MyTimer0_Init();
}

/********************* Timer0�жϺ���************************/
void timer0_int (void) interrupt TIMER0_VECTOR
{
	static u16 i = 0;
	
	i++;
	if(i>=My_Timer_Int_Times*10)  // ÿ10���ȡһ������
	{
		i = 0;
		Read_DHT11_Error = DataOld_Error;  // ��־���ݹ��ɣ���Ҫ����
	}
}