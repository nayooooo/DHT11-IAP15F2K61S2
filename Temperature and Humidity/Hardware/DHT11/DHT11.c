/**
 * @file DHT11.c
 * @brief ��ʪ�ȴ�����ģ��
 */
#include "DHT11.h"
#include "config.h"
#include "delay.h"
#include "GPIO.h"

#include "led.h"

DHT11_DataPack_Typedef DHT11_Data;

// ��DHT11�����־
// 0->�޴���	(No_Error)
// 1->��ʱ		(OverTime_Error)
// 2->�������	(Transmission_Error)
// 3->���ݹ���	(DataOld_Error)
Read_DHT11_Error_Flag Read_DHT11_Error = 0;

void DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	delay_ms(1000);  // �ȴ�1s��Խ�����ȶ�״̬
	
	GPIO_InitStructure.Pin = DHT11_Data_Pin;
	GPIO_InitStructure.Mode = GPIO_OUT_OD;  // ��©��������ں����Ķ�ȡDHT11
	GPIO_Inilize(DHT11_Port, &GPIO_InitStructure);
	
	DHT11_Data_Clr();
}

// �������Ϳ�ʼ�ź�
static void Start_DHT11(void)
{
	DHT11_Data_Clr();
	DHT11_Data_Set();
	delay_ms(18);		// ������������18ms����֤DHT11��⵽��ʼ�ź�
	DHT11_Data_Clr();
}

// ��ȡDHT11���͵����ݵ�һ���ֽ�
// ��֪��Ϊʲô��ʱֱ���˳�
// ��������Ϊ��������ݶ���0��
static u8 Read_DHT11_Part(void)
{
	u8 i;
	u8 temp;
	u8 Overtime_Flag;  // ��ʱ��ǣ�������DHT11������
	u8 Read_Byte = 0X00;  // ��ȡ������
	
	for(i=0; i<8; i++)
	{
		Overtime_Flag = 0;
		while(!DHT11_Data_Pin && ++Overtime_Flag);  // 1bit��ʼ
		if(Overtime_Flag == 0) break;  // ��ʱ�˳�
		
		// ��ʱ30us
		Delay10us();
		Delay10us();
		Delay10us();
		if(DHT11_Data_Pin)
		{
			temp = 1;
			Overtime_Flag = 0;
			while(DHT11_Data_Pin && ++Overtime_Flag);  // ֱ���ߵ�ƽ����
		}
		else temp = 0;
		if(Overtime_Flag == 0) break;  // ��ʱ�˳�
		Read_Byte <<= 1;
		Read_Byte |= temp;
	}
	return Read_Byte;
}

// ��ȡһ��DHT11
DHT11_DataPack_Typedef Read_DHT11(void)
{
	u8 i;
	u8 Check_Byte;
	u8 Overtime_Flag;  // ��ʱ��ǣ�������DHT11������
	DHT11_DataPack_Typedef DHT11_Data = {0, 0, 0, 0, 0};
	u8 *pByte = (u8*)&DHT11_Data;
	
	Read_DHT11_Error = No_Error;  // ���ö�DHT11�����־
	
	Start_DHT11();
	
	// ��ʱ20~40us
	Delay10us();
	Delay10us();
	Delay10us();
	Delay10us();
	// ��ȡDHT11����Ӧ�ź�
	DHT11_Data_Read_Bef();  // ������Ϊ����
	if(!DHT11_Data_Pin)  // DHT11��Ӧ
	{
		Overtime_Flag = 0;
		while(!DHT11_Data_Pin && ++Overtime_Flag);  // �ж�DHT11����Ӧ�źŵĵ͵�ƽ�Ƿ����
		if(Overtime_Flag == 0)  // ��ʱ
			Read_DHT11_Error = OverTime_Error;
		Overtime_Flag = 0;
		while(DHT11_Data_Pin && ++Overtime_Flag);  // �ж�DHT11����Ӧ�źŵĸߵ�ƽ�Ƿ����
		if(Overtime_Flag == 0)  // ��ʱ
			Read_DHT11_Error = OverTime_Error;
		
		// ��ʼ��ȡ��Ϣ
		for(i=0; i<8; i++)
		{
			*pByte = Read_DHT11_Part();
			pByte++;
		}
	}
	
	// �ж��Ƿ���ִ������
	Check_Byte = DHT11_Data.Hum_Data_Int + DHT11_Data.Hum_Data_Dec +\
				DHT11_Data.Tem_Data_Int + DHT11_Data.Tem_Data_Dec;
	if(DHT11_Data.Check_Byte!=Check_Byte)  // �������
		Read_DHT11_Error = Transmission_Error;
	
	return DHT11_Data;
}