#ifndef __TASK_H
#define __TASK_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "gpio.h"
#include "lcd.h"
#include <stdio.h>	 

uint16_t GetR37_Value(void);
void Close_Led(void);
void Display_Main(void);
void Display_Setting(void);
void Key_Scan(void);
void Led_Light(int Upper_Lower);
void Control_Led(void);
void Control_Display(void);
void Project10(void);	 
extern uint8_t Flag_Time_Count;	 
extern uint8_t Reverse;
#ifdef __cplusplus
}
#endif
#endif
