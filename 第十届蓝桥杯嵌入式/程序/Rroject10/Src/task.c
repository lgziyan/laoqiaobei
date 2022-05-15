#include "task.h"

#define B1 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)
#define B2 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)
#define B3 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)
#define B4 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

uint8_t str_show[20];
double Max_Volt=2.4;
double Min_Volt=1.2;
int Upper_Led=1;
int Lower_Led=2;
uint8_t Main_Setting=1;
uint8_t Key_Scan_Line=2;
uint8_t Flag_Time_Count=0;
uint8_t Reverse=0;

/*R37ADC通道值获取*/
uint16_t GetR37_Value(void)
{
	uint16_t val=0;
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2,2);
	val=HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
	return val;
}

/*LED指示灯系列*/
void Close_Led(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}
void Led_Light(int Upper_Lower)
{
	switch(Upper_Lower)
	{
		case 1:
		{
			if(Reverse)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			break;
		}
		case 2:
		{
			if(Reverse)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			break;
		}
		case 3:
		{
			if(Reverse)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			break;
		}
		case 4:
		{
			if(Reverse)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_8|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_8|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			break;
		}
		case 5:
		{
			if(Reverse)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_8|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_8|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			break;
		}
		case 6:
		{
			if(Reverse)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_8|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_8|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			break;
		}
		case 7:
		{
			if(Reverse)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_8|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_8|GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			break;
		}
		case 8:
		{
			if(Reverse)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_8, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_8, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			}
			break;
		}
		default:break;
	}
}

void Control_Led(void)
{
	if(GetR37_Value()*3.3/4096>Max_Volt)
	{
		if(Flag_Time_Count)
		{
			Flag_Time_Count=0;
			Led_Light(Upper_Led);

		}
	}
	else if(GetR37_Value()*3.3/4096<Min_Volt)
	{
		if(Flag_Time_Count)
		{
			Flag_Time_Count=0;
			Led_Light(Lower_Led);
		}
	}
	else
	{
		Close_Led();
	}
}

/*数据界面和参数界面显示*/
void Display_Main(void)
{
	LCD_SetBackColor(White);
	LCD_DisplayStringLine(Line1,(unsigned char *)"        Main        ");
	sprintf((char*)str_show,"  Volt:%.2fV        ",GetR37_Value()*3.3/4096);
	LCD_DisplayStringLine(Line4,str_show);
	if(GetR37_Value()*3.3/4096>Max_Volt)
	{
		LCD_DisplayStringLine(Line6,(unsigned char *)"  Status:Upper      ");
	}
	else if(GetR37_Value()*3.3/4096<Min_Volt)
	{
		LCD_DisplayStringLine(Line6,(unsigned char *)"  Status:Lower      ");
	}
	else
	{
		LCD_DisplayStringLine(Line6,(unsigned char *)"  Status:Normal     ");
	}	
}
void Display_Setting(void)
{
	LCD_SetBackColor(White);
	//LCD_DisplayStringLine(Line0,(unsigned char *)"                    ");
	LCD_DisplayStringLine(Line1,(unsigned char *)"      Setting       ");
	if(Key_Scan_Line==2)
		LCD_SetBackColor(Green);
	else
		LCD_SetBackColor(White);
	sprintf((char*)str_show,"   Max Volt:%.1fV    ",Max_Volt);
	LCD_DisplayStringLine(Line2,str_show);
	if(Key_Scan_Line==4)
		LCD_SetBackColor(Green);
	else
		LCD_SetBackColor(White);	
	sprintf((char*)str_show,"   Min Volt:%.1fV    ",Min_Volt);
	LCD_DisplayStringLine(Line4,str_show);
	if(Key_Scan_Line==6)
		LCD_SetBackColor(Green);
	else
		LCD_SetBackColor(White);	
	sprintf((char*)str_show,"    Upper:LD%d       ",Upper_Led);
	LCD_DisplayStringLine(Line6,str_show);
	if(Key_Scan_Line==8)
		LCD_SetBackColor(Green);
	else
		LCD_SetBackColor(White);	
	sprintf((char*)str_show,"    Lower:LD%d       ",Lower_Led);
	LCD_DisplayStringLine(Line8,str_show);	
}

void Control_Display(void)
{
	if(Main_Setting)
	{
		Display_Main();
	}
	else
	{
		Display_Setting();
	}
}
/*按键扫描函数*/
//void Setting_Which_Line()
//{
//	switch(Key_Scan_Line)
//	{
//		case 2:
//		{
//			
//			break;
//		}
//		default:break;
//	}
//}

void Key_Scan(void)
{
	if(Main_Setting)
	{
		if(B1==RESET)
		{
			HAL_Delay(10);
			if(B1==RESET)
			{
				Main_Setting=!Main_Setting;
				LCD_Clear(White);
				while(!B1);
			}
		}
	}
	else
	{
		if(B1==RESET)
		{
			HAL_Delay(10);
			if(B1==RESET)
			{
				Main_Setting=!Main_Setting;
				LCD_Clear(White);
				while(!B1);
			}
		}
		else if(B2==RESET)
		{
			if(B2==RESET)
			{
				HAL_Delay(10);
				if(B2==RESET)
				{
					Key_Scan_Line=Key_Scan_Line+2;
					if(Key_Scan_Line>8)
						Key_Scan_Line=2;
					LCD_Clear(White);
					while(!B2);
				}
			}			
		}
		else if(B3==RESET)
		{
			HAL_Delay(10);
			if(B3==RESET)
			{
				if(Key_Scan_Line==2)
				{
					Max_Volt=Max_Volt+0.3;
					if(Max_Volt>3.3)
						Max_Volt=3.3;
				}
				else if(Key_Scan_Line==4)
				{
					Min_Volt=Min_Volt+0.3;
					if(Min_Volt>Max_Volt)
						Min_Volt=Max_Volt;					
				}
				else if(Key_Scan_Line==6)
				{
					Upper_Led=Upper_Led+1;
					if(Upper_Led>8)
						Upper_Led=1;
					if(Upper_Led==Lower_Led)
						Upper_Led=Upper_Led+1;
					if(Upper_Led>8)
						Upper_Led=1;					
				}
				else if(Key_Scan_Line==8)
				{
					Lower_Led=Lower_Led+1;
					if(Lower_Led>8)
						Lower_Led=1;
					if(Upper_Led==Lower_Led)
						Lower_Led=Lower_Led+1;
					if(Lower_Led>8)
						Lower_Led=1;					
				}
				LCD_Clear(White);
				while(!B3);
			}			
		}
		else if(B4==RESET)
		{
			HAL_Delay(10);
			if(B4==RESET)
			{
				if(Key_Scan_Line==2)
				{
					Max_Volt=Max_Volt-0.3;
					if(Max_Volt<Min_Volt)
						Max_Volt=Min_Volt;
				}
				else if(Key_Scan_Line==4)
				{
					Min_Volt=Min_Volt-0.3;
					if(Min_Volt<0)
						Min_Volt=0;					
				}
				else if(Key_Scan_Line==6)
				{
					Upper_Led=Upper_Led-1;
					if(Upper_Led<1)
						Upper_Led=8;
					if(Upper_Led==Lower_Led)
						Upper_Led=Upper_Led-1;
					if(Upper_Led<1)
						Upper_Led=8;					
				}
				else if(Key_Scan_Line==8)
				{
					Lower_Led=Lower_Led-1;
					if(Lower_Led<1)
						Lower_Led=8;
					if(Upper_Led==Lower_Led)
						Lower_Led=Lower_Led-1;
					if(Lower_Led<1)
						Lower_Led=8;				
				}
				LCD_Clear(White);
				while(!B4);
			}			
		}
	}
}

void Project10(void)
{
	Control_Led();
	Control_Display();
	Key_Scan();
}

