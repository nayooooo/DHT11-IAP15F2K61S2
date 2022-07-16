#include "sgm.h"
#include "config.h"
#include "GPIO.h"
#include "delay.h"

/*----------- ����������� -----------*/

// λѡ��
u8 code Tabel_WE[] = {
// 0	 1	   2	 3	   4	 5	   6	 7
0XFE, 0XFD, 0XFB, 0XF7, 0XEF, 0XDF, 0XBF, 0X7F,
};

// ��ѡ��
u8 code Tabel_DU[] = {
// 0	 1	   2	 3	   4	 5	   6	 7	   8	 9
0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F,
};

sbit DU = P2^6;  // ����ܶ�ѡ����λ
sbit WE = P2^7;  // �����λѡ����λ
#define SGM_Control_Port	GPIO_P2
#define SGM_Control_DU_Pin	P26
#define SGM_Control_WE_Pin	P27

#define SGM_Data_Port	GPIO_P0
#define SGM_Pin			P0

/*----------- ����������� -----------*/

void SGM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.Pin = GPIO_Pin_All;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(SGM_Data_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = SGM_Control_DU_Pin|SGM_Control_WE_Pin;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(SGM_Control_Port, &GPIO_InitStructure);
	
	DU = 0; WE = 0;
}

// �����ѡ
static void SGM_Clear_DU(void)
{
	SGM_Pin = 0X00;
	DU = 1;
	DU = 0;
}

// ���λѡ
static void SGM_Clear_WE(void)
{
	SGM_Pin = 0XFF;
	WE = 1;
	WE = 0;
}

// ���ö�ѡ
static void SGM_Set_DU(u8 du)
{
	SGM_Pin = du;
	DU = 1;
	DU = 0;
}

// ����λѡ
static void SGM_Set_WE(u8 we)
{
	SGM_Pin = we;
	WE = 1;
	WE = 0;
}

// ����������ʾ
void SGM_Clear(void)
{
	SGM_Clear_WE();
}

// �����������ʾһλ
void SGM_Set_One(u8 we, u8 du)
{
	SGM_Clear_WE();
	SGM_Set_DU(du);
	SGM_Set_WE(we);
}

// �����������ʾһλ����
void SGM_Set_Num(u8 we, u8 du)
{
	SGM_Set_One(Tabel_WE[we], Tabel_DU[du]);
}

// �����������ʾһλ���֣����㣩
void SGM_Set_Num_Point(u8 we, u8 du)
{
	SGM_Set_One(Tabel_WE[we], Tabel_DU[du] | 0X80);
}

// �������ʾ����
void SGM_Display_Nums(u32 num)
{
	u8 i = SGM_DIGIT-1;
	
	if(num==0)  // num==0���ֱ������ѭ��
	{
		SGM_Set_Num(i, num);
		return;
	}
	while(num && i)
	{
		SGM_Set_Num(i, num%10);
		num /= 10;
		i--;
	}
}

// �����ָ��λ����ʾ���֣�ָ�����ֵ����һλ��
void SGM_Appoint_Display_Nums(u32 num, u8 addr)
{
	u8 i = addr;
	
	if(num==0)  // num==0���ֱ������ѭ��
	{
		SGM_Set_Num(i, num);
		return;
	}
	while(num && i)
	{
		SGM_Set_Num(i, num%10);
		num /= 10;
		i--;
	}
}

// �����ָ��λ����ʾ���֣�С������ָ���������ֵ����һλ��
void SGM_Appoint_Display_Eecimal(u32 Int_part, u8 Fra_Part, u8 addr)
{
	u8 i = addr;
	
	if(Fra_Part==0)  // Fra_Part==0���ֱ������ѭ��
	{
		SGM_Set_Num(i, Fra_Part);
		i--;
		goto Fra_Part_Out;
	}
	
	// ��ʾС������
	while(Fra_Part && i)
	{
		SGM_Set_Num(i, Fra_Part%10);
		Fra_Part /= 10;
		i--;
	}
Fra_Part_Out:
	
	// ��ʾ�������ֵĸ�λ
	// ���Int_part==0����ִ���·�����Int_part==0
	SGM_Set_Num_Point(i, Int_part%10);
	if(Int_part==0) return;
	Int_part /= 10;
	i--;
	// ��ʾ�������ֵ�����λ
	while(Int_part && i)
	{
		SGM_Set_Num(i, Int_part%10);
		Int_part /= 10;
		i--;
	}
}