/**
 * @file DHT11.h
 * @brief 温湿度传感器模块
 */
#ifndef __DHT11_H
#define __DHT11_H

#include "config.h"

typedef u8 Read_DHT11_Error_Flag;

#define DHT11_Port		GPIO_P3
#define DHT11_Data_Pin	P32

#define DHT11_Data_Clr()	(DHT11_Data_Pin = 1)
#define DHT11_Data_Set()	(DHT11_Data_Pin = 0)
#define DHT11_Data_Read_Bef()	(DHT11_Data_Pin = 1)  // 读取前拉高对应引脚（开漏输出）

typedef struct{
	u8 Hum_Data_Int;	// 湿度整数部分
	u8 Hum_Data_Dec;	// 湿度小数部分
	u8 Tem_Data_Int;	// 温度整数部分
	u8 Tem_Data_Dec;	// 温度小数部分
	u8 Check_Byte;		// 校验位（前四个数据相加得到校验位理论值）
}DHT11_DataPack_Typedef;

typedef enum{
	No_Error			= 0,
	OverTime_Error		= 1,
	Transmission_Error	= 2,
	DataOld_Error		= 3
}Read_DHT11_Error_Typedef;
extern Read_DHT11_Error_Flag Read_DHT11_Error;

extern DHT11_DataPack_Typedef DHT11_Data;

void DHT11_Init(void);

DHT11_DataPack_Typedef Read_DHT11(void);

#endif