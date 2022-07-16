#include "led.h"
#include "config.h"
#include "GPIO.h"

void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.Pin = GPIO_Pin_All;
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	GPIO_Inilize(LED_Port, &GPIO_InitStructure);
}