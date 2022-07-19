/**
 * @file DHT11.c
 * @brief 温湿度传感器模块
 */
#include "DHT11.h"
#include "config.h"
#include "delay.h"
#include "GPIO.h"

#include "led.h"

DHT11_DataPack_Typedef DHT11_Data;

// 读DHT11错误标志
// 0->无错误	(No_Error)
// 1->超时		(OverTime_Error)
// 2->传输错误	(Transmission_Error)
// 3->数据过旧	(DataOld_Error)
Read_DHT11_Error_Flag Read_DHT11_Error = 0;

void DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	delay_ms(1000);  // 等待1s以越过不稳定状态
	
	GPIO_InitStructure.Pin = DHT11_Data_Pin;
	GPIO_InitStructure.Mode = GPIO_OUT_OD;  // 开漏输出，便于后续的读取DHT11
	GPIO_Inilize(DHT11_Port, &GPIO_InitStructure);
	
	DHT11_Data_Clr();
}

// 主机发送开始信号
static void Start_DHT11(void)
{
	DHT11_Data_Clr();
	DHT11_Data_Set();
	delay_ms(18);		// 拉低总线至少18ms，保证DHT11检测到起始信号
	DHT11_Data_Clr();
}

// 读取DHT11发送的数据的一个字节
// 不知道为什么超时直接退出
// 可能是因为后面的数据都是0吧
static u8 Read_DHT11_Part(void)
{
	u8 i;
	u8 temp;
	u8 Overtime_Flag;  // 超时标记，避免因DHT11而卡死
	u8 Read_Byte = 0X00;  // 读取的数据
	
	for(i=0; i<8; i++)
	{
		Overtime_Flag = 0;
		while(!DHT11_Data_Pin && ++Overtime_Flag);  // 1bit开始
		if(Overtime_Flag == 0) break;  // 超时退出
		
		// 延时30us
		Delay10us();
		Delay10us();
		Delay10us();
		if(DHT11_Data_Pin)
		{
			temp = 1;
			Overtime_Flag = 0;
			while(DHT11_Data_Pin && ++Overtime_Flag);  // 直到高电平结束
		}
		else temp = 0;
		if(Overtime_Flag == 0) break;  // 超时退出
		Read_Byte <<= 1;
		Read_Byte |= temp;
	}
	return Read_Byte;
}

// 读取一次DHT11
DHT11_DataPack_Typedef Read_DHT11(void)
{
	u8 i;
	u8 Check_Byte;
	u8 Overtime_Flag;  // 超时标记，避免因DHT11而卡死
	DHT11_DataPack_Typedef DHT11_Data = {0, 0, 0, 0, 0};
	u8 *pByte = (u8*)&DHT11_Data;
	
	Read_DHT11_Error = No_Error;  // 重置读DHT11错误标志
	
	Start_DHT11();
	
	// 延时20~40us
	Delay10us();
	Delay10us();
	Delay10us();
	Delay10us();
	// 读取DHT11的响应信号
	DHT11_Data_Read_Bef();  // 主机设为输入
	if(!DHT11_Data_Pin)  // DHT11响应
	{
		Overtime_Flag = 0;
		while(!DHT11_Data_Pin && ++Overtime_Flag);  // 判断DHT11的响应信号的低电平是否结束
		if(Overtime_Flag == 0)  // 超时
			Read_DHT11_Error = OverTime_Error;
		Overtime_Flag = 0;
		while(DHT11_Data_Pin && ++Overtime_Flag);  // 判断DHT11的响应信号的高电平是否结束
		if(Overtime_Flag == 0)  // 超时
			Read_DHT11_Error = OverTime_Error;
		
		// 开始读取信息
		for(i=0; i<5; i++)
		{
			*pByte = Read_DHT11_Part();
			pByte++;
		}
	}
	
	// 判断是否出现传输错误
	Check_Byte = DHT11_Data.Hum_Data_Int + DHT11_Data.Hum_Data_Dec +\
				DHT11_Data.Tem_Data_Int + DHT11_Data.Tem_Data_Dec;
	if(DHT11_Data.Check_Byte!=Check_Byte)  // 传输错误
		Read_DHT11_Error = Transmission_Error;
	
	return DHT11_Data;
}
