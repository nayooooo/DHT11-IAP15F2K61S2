#include "config.h"
#include "delay.h"
#include "led.h"
#include "sgm.h"
#include "DHT11.h"
#include "MyTime.h"

/**
 * @22.1184MHz
 */

void main()
{
	LED_GPIO_Config();
	SGM_GPIO_Config();
	DHT11_Init();
	MyTimer_Init();
	
	EA = 1;  // 开启总中断
	
	LED_ALL = LED_ALL_OFF;
	DHT11_Data = Read_DHT11();
	
	while(1)
	{
		if(Read_DHT11_Error==DataOld_Error)  // 数据过旧，更新数据
		{
			LED(7) = LED_OFF;
			DHT11_Data = Read_DHT11();
			LED(7) = LED_ON;
		}
		
		if(Read_DHT11_Error==No_Error)  // 没有错误
		{
			SGM_Set_One(~0X80, 0X39);  // C
			SGM_Set_One(~0X40, 0X63);  // o
			SGM_Appoint_Display_Eecimal(DHT11_Data.Tem_Data_Int, DHT11_Data.Tem_Data_Dec, 5);
		}
		else Read_DHT11_Error = DataOld_Error;
		
		LED(7) = LED_ON;
	}
}