#ifndef __MYTIME_H
#define __MYTIME_H

#include "config.h"

/*---------------- 隐藏使用的定时器 ----------------*/

#define Timer0_Int_Times 1000  // 定时器0的频率

#define My_Timer Timer0
#define My_Timer_Int_Times Timer0_Int_Times

/*---------------- 隐藏使用的定时器 ----------------*/

void MyTimer_Init(void);

#endif