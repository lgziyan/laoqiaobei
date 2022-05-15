#include "task.h"
#include "gpio.h"
#include "adc.h"
#include "rtc.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>
#include "i2c_hal.h"
#include "usart.h"
#include "dma.h"

#define B1 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)
#define B2 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)
#define B3 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)
#define B4 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

#define Volt (getADCR37()*3.3/4096)

uint8_t strs_show[20];
uint8_t strs_send[20];
uint8_t Recive_str[50];

float k=0.1;
uint8_t send_one_time=0;
uint8_t led_enble=0;
uint8_t send_volt_time[]={0,0,0};
uint8_t temp_volt_time[]={0,0,0};
uint8_t data_setting=1;
uint8_t led1_on=1;
uint8_t star_counting=0;
uint8_t which_position_change=1;

extern DMA_HandleTypeDef hdma_usart1_rx;
/***********关于led的函数*********/
void OC_PD2(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}

void close_leds(void)
{
	GPIOC->ODR=0X0000ff00;
	OC_PD2();
}

void control_leds(void)
{
	if((led_enble)&&(Volt>3.3f*k))
	{
		star_counting=1;
		if(led1_on)
		{
			GPIOC->ODR=0X0000fe00;
			OC_PD2();
		}
		else
		{
			close_leds();			
		}
		
	}
	else
	{
		star_counting=0;
		close_leds();
	}
}

/***********关于ADCR37的函数*********/
uint16_t getADCR37(void)
{
	uint16_t r37 =0;
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2,2);
	r37= HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
	return r37;
}

/***********RTC的实时时钟函数*********/
void Set_RTCtime(uint8_t hours,uint8_t minutes,uint8_t seconds)
{
	RTC_TimeTypeDef sTime = {0};
	sTime.Hours = hours;
  sTime.Minutes = minutes;
  sTime.Seconds = seconds;
  sTime.SubSeconds = 0;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
}

void RTC_TimeShow(void)
{
	

  RTC_DateTypeDef date;
  HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN);
	
  RTC_TimeTypeDef stimestructureget;
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh-mm-ss */
  sprintf((char *)strs_show, "  T:%02d-%02d-%02d     ", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
	LCD_DisplayStringLine(Line5,strs_show);

}

/***********界面显示函数*********/
void display_data(void)
{
	sprintf((char *)strs_show, "  V1:%.2fV          ",Volt);
	LCD_DisplayStringLine(Line2,strs_show);
	sprintf((char *)strs_show, "  k:%.1f          ",k);
	LCD_DisplayStringLine(Line3,strs_show);	
	if(led_enble)
	{
		LCD_DisplayStringLine(Line4,(uint8_t*)"  LED:ON            ");	
	}
	else
	{
		LCD_DisplayStringLine(Line4,(uint8_t*)"  LED:OFF           ");
	}
	RTC_TimeShow();
}

void display_setting(void)
{
	LCD_DisplayStringLine(Line2,(uint8_t*)"      Setting       ");
	LCD_ClearLine(Line3);
	LCD_ClearLine(Line4);
  sprintf((char *)strs_show, "     %02d-%02d-%02d       ",temp_volt_time[0],temp_volt_time[1],temp_volt_time[2]);
	LCD_DisplayStringLine(Line5,strs_show);	
	
}
void display(void)
{
	if(data_setting)
	{
		display_data();
	}
	else
	{
		display_setting();
	}
}

/***********EEPROM读写函数*********/
void x24c02_write(uint8_t address,uint8_t infor)
{
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	I2CSendByte(address);
	I2CWaitAck();
	I2CSendByte(infor);
	I2CWaitAck();
	I2CStop();
}

uint8_t x24c02_read(uint8_t address)
{
	uint8_t val=0;
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	I2CSendByte(address);
	I2CWaitAck();
	I2CStop();
	I2CStart();
	I2CSendByte(0xa1);
	I2CWaitAck();
	val= I2CReceiveByte();
	I2CStop();
	return val;
}
///***********串口收发部分函数*********/
//void HAL_UART_IdleCpltCallback(UART_HandleTypeDef *huart)
//{
//	uint8_t num=0;
//	num=50-__HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
//	if(num==5)
//	{
//		if((Recive_str[0]=='k')&&(Recive_str[1]=='0')&&(Recive_str[2]=='.')&&(('1'<=Recive_str[3])&&(Recive_str[3]<='9'))&&(Recive_str[4]=='\n'))
//		{
//			HAL_UART_Transmit_DMA(&huart1,change_ok_flag,strlen((char*)change_ok_flag));
//		}
//	}
//	HAL_UART_Receive_DMA(&huart1,Recive_str,50);
//}

void time_to_send_volt(void)
{
	RTC_DateTypeDef date;
  HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN);
  RTC_TimeTypeDef stimestructureget;
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh-mm-ss */
  switch(send_one_time)//避免多次输出的情况
	{
		case 0:
		{
			if((stimestructureget.Hours==send_volt_time[0])&&(stimestructureget.Minutes==send_volt_time[1])&&(stimestructureget.Seconds==send_volt_time[2]))//避免多次输出
			{
				sprintf((char *)strs_send, "%.2f+%.1f+%02d%02d%02d\r\n", Volt,k,stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);	
				HAL_UART_Transmit_DMA(&huart1,strs_send,strlen((char*)strs_send));
				send_one_time=1;
			}
			break;
		}
		case 1:
		{
			if(send_volt_time[2]!=59)
			{
				if(stimestructureget.Seconds==send_volt_time[2]+1)
					send_one_time=0;
			}
			else
			{
					if(stimestructureget.Seconds==0)
					send_one_time=0;
			}
			break;
		}
		default:break;	
   }

}

/***********按键扫描函数*********/
void change_send_time(uint8_t pos)
{
	uint8_t temp=0;
	switch(pos)
	{
		case 1:{
		temp=temp_volt_time[0]/10;
		if(++temp==10)
			temp=0;
		}
		temp_volt_time[0]=temp*10+temp_volt_time[0]%10;
		break;
		case 2:{
		temp=temp_volt_time[0]%10;
		if(++temp==10)
			temp=0;
		}
		temp_volt_time[0]=temp+temp_volt_time[0]/10*10;
		break;
		case 3:{
		temp=temp_volt_time[1]/10;
		if(++temp==10)
			temp=0;
		}
		temp_volt_time[1]=temp*10+temp_volt_time[1]%10;
		break;
		case 4:{
		temp=temp_volt_time[1]%10;
		if(++temp==10)
			temp=0;
		}
		temp_volt_time[1]=temp+temp_volt_time[1]/10*10;
		break;
		case 5:{
		temp=temp_volt_time[2]/10;
		if(++temp==10)
			temp=0;
		}
		temp_volt_time[2]=temp*10+temp_volt_time[2]%10;
		break;
		case 6:{
		temp=temp_volt_time[2]%10;
		if(++temp==10)
			temp=0;
		}
		temp_volt_time[2]=temp+temp_volt_time[2]/10*10;
		break;
		default:break;
	}
}
void key_scan(void)
{
	if(!data_setting)
	{
		if(B3==RESET)
		{
			HAL_Delay(5);
			if(B3==RESET)
			{
				which_position_change=which_position_change+1;
				if(which_position_change==7)
					which_position_change=1;
				while(!B3);
			}
		}
		else if(B4==RESET)
		{
			HAL_Delay(5);
			if(B4==RESET)
			{
				change_send_time(which_position_change);
				while(!B4);
			}
		}		
	}
	if(B1==RESET)
	{
		HAL_Delay(5);
		if(B1==RESET)
		{
			led_enble=!led_enble;
			while(!B1);
		}
	}
	else if(B2==RESET)
	{
		HAL_Delay(5);
		if(B2==RESET)
		{
			if(!data_setting)
			{
				uint8_t i;
				for(i=0;i<=2;i++)
				{
					send_volt_time[i]=temp_volt_time[i];
				}
		  }
			data_setting=!data_setting;
			while(!B2);
		}		
	}
}

/***********部分函数初始化*********/
void task_init(void)
{
	close_leds();
	
	//k=x24c02_read(0x01)*0.1;
	x24c02_write(0x01,1);
}

/***********任务函数*********/
void project7(void)
{
	key_scan();
	control_leds();
	display();
	time_to_send_volt();
}
