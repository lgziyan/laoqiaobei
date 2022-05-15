#include "task.h"
#include "gpio.h"
#include "i2c_hal.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>
#include "tim.h"
#include "usart.h"


#define B1 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)
#define B2 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)
#define B3 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)
#define B4 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)


uint8_t led1=0;
uint8_t led2=0;
uint8_t strs_show[20];
uint8_t xy_shop[2]={0,0};
uint8_t  xy_price[2]={10,10};
uint8_t xy_rep[2]={10,10};
uint8_t which_diplay=1;
uint8_t ok_task=0;
uint8_t RX[10];
uint8_t send_str[20];
uint8_t  led2_revese=0;

uint32_t  freq=0;
uint8_t  duty=0;
uint8_t strs_test[20];
uint8_t test_i2c=0;

/***********led相关函数***********/
void close_leds(void)
{
	GPIOC->ODR=0x0000ff00;
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}
void control_leds(void)
{
	if((xy_rep[0]==0)&&(xy_rep[1]==0))
	{
		led2=1;
	}
	else
		led2=0;
	if(led1)
	{
		if(led2_revese)
		{
		GPIOC->ODR=0x0000fc00;
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);			
		}
		else
		{
		GPIOC->ODR=0x0000fe00;
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);			
		}
	}
	else if(led2&&!led1)
	{
		if(led2_revese)
		{
		GPIOC->ODR=0x0000fd00;
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);			
		}
		else
		{
		GPIOC->ODR=0x0000ff00;
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);			
		}		
	}
	else
		 close_leds();
}
		


/***********EEPROM***********/
void x24c02_write(uint8_t address,uint8_t inf)
{
	I2CStart();
	I2CSendByte(0xa0);
	I2CWaitAck();
	I2CSendByte(address);
	I2CWaitAck();	
	I2CSendByte(inf);
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
	val=I2CReceiveByte();
	I2CStop();
	return val;
}

/***********界面显示函数***********/
void display_shop(void)
{
	LCD_DisplayStringLine(Line1,(uint8_t*)"        SHOP        ");
	sprintf((char*)strs_show,"     X:%d          ",xy_shop[0]);
	LCD_DisplayStringLine(Line3,strs_show);
	sprintf((char*)strs_show,"     Y:%d          ",xy_shop[1]);
	LCD_DisplayStringLine(Line4,strs_show);	
	
}
void display_price(void)
{
	LCD_DisplayStringLine(Line1,(uint8_t*)"        PRICE       ");
	sprintf((char*)strs_show,"     X:%.1f          ",xy_price[0]*1.0/10);
	LCD_DisplayStringLine(Line3,strs_show);
	sprintf((char*)strs_show,"     Y:%.1f          ",xy_price[1]*1.0/10);
	LCD_DisplayStringLine(Line4,strs_show);	
}
void display_rep(void)
{
	LCD_DisplayStringLine(Line1,(uint8_t*)"        REP         ");
	sprintf((char*)strs_show,"     X:%d          ",xy_rep[0]);
	LCD_DisplayStringLine(Line3,strs_show);
	sprintf((char*)strs_show,"     Y:%d          ",xy_rep[1]);
	LCD_DisplayStringLine(Line4,strs_show);	
	
}
void display(void)
{
	if(which_diplay==1)
	{
		display_shop();
	}
	else if(which_diplay==2)
	{
		display_price();
	}
	else if(which_diplay==3)
	{
		display_rep();
	}
}

/***********按键扫描函数***********/

void add_price(uint8_t num)
{
		xy_price[num]=xy_price[num]+1;
		if(xy_price[num]==21)
			xy_price[num]=10;
}
void key_scan(void)
{
	if(B1==RESET)
	{
		HAL_Delay(5);
		if(B1==RESET)
		{
			which_diplay=which_diplay+1;
			if(which_diplay>=4)
				which_diplay=1;
			while(!B1);
		}
	}
	if(which_diplay==1)
	{
		if(B2==RESET)
		{
			HAL_Delay(5);
			if(B2==RESET)
			{
				if(++xy_shop[0]>xy_rep[0])
					xy_shop[0]=0;
				while(!B2);
			}			
		}
		else if(B3==RESET)
		{
			HAL_Delay(5);
			if(B3==RESET)
			{
				if(++xy_shop[1]>xy_rep[1])
					xy_shop[1]=0;
				while(!B3);
			}			
		}
		else if(B4==RESET)
		{
			HAL_Delay(5);
			if(B4==RESET)
			{
				ok_task=1;
				led1=1;
				while(!B4);
		  }
	  }
	}
	else if(which_diplay==2)
	{
		if(B2==RESET)
		{
			HAL_Delay(5);
			if(B2==RESET)
			{
				add_price(0);
				x24c02_write(0x02,xy_price[0]);
				while(!B2);
			}			
		}
		else if(B3==RESET)
		{
			HAL_Delay(5);
			if(B3==RESET)
			{
				add_price(1);
				x24c02_write(0x03,xy_price[1]);
				while(!B3);
			}			
		}		
	}
	else if(which_diplay==3)
	{
		if(B2==RESET)
		{
			HAL_Delay(5);
			if(B2==RESET)
			{
				xy_rep[0]++;
				x24c02_write(0x00,xy_rep[0]);
				while(!B2);
			}			
		}
		else if(B3==RESET)
		{
			HAL_Delay(5);
			if(B3==RESET)
			{
				xy_rep[1]++;
				x24c02_write(0x01,xy_rep[1]);
				while(!B3);
			}			
		}		
	}
}

/***********改变PWM频率和占空比的函数***********/
void change_TIM2_freq(uint32_t freq,uint8_t duty)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 79;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000000/freq-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = duty*1000000.0/freq/100;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
}

/***********串口接收回调函数***********/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(RX[0]=='?')
	{
		sprintf((char*)send_str,"X:%.1f,Y:%.1f\r\n",xy_price[0]*1.0/10,xy_price[1]*1.0/10);
	  HAL_UART_Transmit_IT(&huart1,send_str, strlen((char*)send_str));
		
	}
	HAL_UART_Receive_IT(&huart1, RX, 1);
}

/***********B4确定任务函数***********/
void ok_B4task(void)
{	
	if(ok_task)
	{
	sprintf((char*)send_str,"X:%d,Y:%d,Z:%.1f\r\n",xy_shop[0],xy_shop[1],xy_shop[0]*xy_price[0]*1.0/10+xy_shop[1]*xy_price[1]*1.0/10);
	HAL_UART_Transmit_IT(&huart1,send_str, strlen((char*)send_str));
	xy_rep[0]=xy_rep[0]-xy_shop[0];
	xy_rep[1]=xy_rep[1]-xy_shop[1];
	x24c02_write(0x00,xy_rep[0]);
		HAL_Delay(10);
		x24c02_write(0x01,xy_rep[1]);
	xy_shop[0]=0;
	xy_shop[1]=0;
	change_TIM2_freq(2000,30);
	ok_task=0;
	}
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	static uint32_t fre_val=0,duty_val=0;
	if(htim->Channel ==HAL_TIM_ACTIVE_CHANNEL_1)
	{
		fre_val=HAL_TIM_ReadCapturedValue(&htim15,TIM_CHANNEL_1);
		freq=1000000/fre_val;
	}
	else if(htim->Channel ==HAL_TIM_ACTIVE_CHANNEL_2)
	{
		duty_val=HAL_TIM_ReadCapturedValue(&htim15,TIM_CHANNEL_2);
		duty=100*duty_val/fre_val;
	}
}
void read_freq_duty(void)
{
	sprintf((char*)strs_test,"f:%d,d=%d    ",freq,duty);
	LCD_DisplayStringLine(Line8,strs_test);
}


/***********任务初始化函数***********/
void task_init(void)
{
	if(x24c02_read(0x10)!=10)
	{
		HAL_Delay(10);
		x24c02_write(0x10,10);
		HAL_Delay(10);
		x24c02_write(0x00,10);
	}
	else
		xy_rep[0]=x24c02_read(0x00);
	HAL_Delay(10);
	
	if(x24c02_read(0x11)!=10)
	{
		HAL_Delay(10);
		x24c02_write(0x11,10);
		HAL_Delay(10);
		x24c02_write(0x01,10);
	}
	else
		xy_rep[1]=x24c02_read(0x01);
	HAL_Delay(10);	
	
	if(x24c02_read(0x12)!=10)
	{
		x24c02_write(0x12,10);
		HAL_Delay(10);
		x24c02_write(0x02,10);
		HAL_Delay(10);
	}
	else
		xy_price[0]=x24c02_read(0x02);
	HAL_Delay(10);
	
	if(x24c02_read(0x13)!=10)
	{
		x24c02_write(0x13,10);
		HAL_Delay(10);
		x24c02_write(0x03,10);
		HAL_Delay(10);
	}
	else
		xy_price[1]=x24c02_read(0x03);
	HAL_Delay(10);


	
//	if(x24c02_read(0x10)!=10)
//	{
//		HAL_Delay(10);
//		x24c02_write(0x10,10);
//		x24c02_write(0x00,10);
//	}
//	else
//				xy_price[1]=x24c02_read(0x03);
//	HAL_Delay(10);
		
//	HAL_Delay(10);	

//    test_i2c=x24c02_read(0x01);
//	  x24c02_write(0x01,1);
//		HAL_Delay(10);	
//		test_i2c=x24c02_read(0x03);
//		
//		test_i2c=test_i2c+1;
//		HAL_Delay(100);
//		x24c02_write(0x03,test_i2c);
			
		//test_i2c=x24c02_read(0x00);	
	
	LCD_Init();
	LCD_Clear(Black);
  LCD_SetTextColor(White);
  LCD_SetBackColor(Black);
	close_leds();
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_UART_Receive_IT(&huart1, RX, 1);
	//HAL_TIM_IC_Start_IT(&htim15,TIM_CHANNEL_1);
	//HAL_TIM_IC_Start_IT(&htim15,TIM_CHANNEL_2);
//		sprintf((char*)strs_test,"0x00:%d           ",test_i2c);
//	LCD_DisplayStringLine(Line9,strs_test);
	
//
}

/***********任务函数***********/
void project(void)
{
	control_leds();
	display();
	key_scan();
	ok_B4task();
	//read_freq_duty();
}
