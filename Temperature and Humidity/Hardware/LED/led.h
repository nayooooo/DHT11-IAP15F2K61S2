#ifndef __LED_H
#define __LED_H

#include "config.h"

#define LED_Port GPIO_P1
#define LED_Pin P1

#define LED_ALL LED_Pin
sbit LED0 = P1^0;
sbit LED1 = P1^1;
sbit LED2 = P1^2;
sbit LED3 = P1^3;
sbit LED4 = P1^4;
sbit LED5 = P1^5;
sbit LED6 = P1^6;
sbit LED7 = P1^7;
#define LED(N) LED##N

#define LED_QUENTITY_MAX (8)

typedef enum{
	LED_ON = 0,
	LED_OFF = 1,
	LED_ALL_ON = 0X00,
	LED_ALL_OFF = 0XFF
}LED_State_8_Typedef;

void LED_GPIO_Config(void);

#endif