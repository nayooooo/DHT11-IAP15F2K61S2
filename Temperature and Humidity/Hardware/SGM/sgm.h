#ifndef __SGM_H
#define __SGM_H

#include "config.h"

#define SGM_DIGIT 8  // 数码管位数

void SGM_GPIO_Config(void);

void SGM_Clear(void);
void SGM_Set_One(u8 we, u8 du);
void SGM_Set_Num(u8 we, u8 du);
void SGM_Set_Num_Point(u8 we, u8 du);
void SGM_Display_Nums(u32 num);
void SGM_Appoint_Display_Nums(u32 num, u8 addr);
void SGM_Appoint_Display_Eecimal(u32 Int_part, u8 Fra_Part, u8 addr);

#endif