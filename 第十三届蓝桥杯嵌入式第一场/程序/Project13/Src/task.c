#include "main.h"
#include "task.h"
#include "lcd.h"
#include "tim.h"
#include <stdio.h>
#include <string.h>

#define B1 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)
#define B2 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)
#define B3 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)
#define B4 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

uint8_t PSD_STA=1;
uint8_t true_flase=0;
uint8_t count_flag=0;
uint8_t num_of_input=0;
uint8_t led1=0;
uint8_t led2=0;
uint8_t saved_password[]={'1','2','3'};
uint8_t input_password[]={'@','@','@'};


/**********关闭所有的led灯**********/
void close_led(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}


/**********显示界面函数**********/
void	display_PSD(void)
{
  unsigned char str[20];
	LCD_DisplayStringLine(Line1,(unsigned char *)"       PSD          ");
	sprintf((char*)str,"    B1:%c            ",input_password[0]);
	LCD_DisplayStringLine(Line3,str);	
	sprintf((char*)str,"    B2:%c            ",input_password[1]);
	LCD_DisplayStringLine(Line4,str);
	sprintf((char*)str,"    B3:%c            ",input_password[2]);
	LCD_DisplayStringLine(Line5,str);

}

void	display_STA(void)
{
	LCD_DisplayStringLine(Line1,(unsigned char *)"       STA          ");
	LCD_DisplayStringLine(Line3,(unsigned char *)"    F:2000Hz        ");	
	LCD_DisplayStringLine(Line4,(unsigned char *)"    D:10%           ");
  LCD_DisplayStringLine(Line5,(unsigned char *)"                    "); 	
}

/**********修改PA1的频率和占空比，对应于状态界面**********/
void Re_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 79;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 499;
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
  sConfigOC.Pulse = 50;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim2);

}

/**********按键扫描函数**********/
void password_change(uint8_t num)
{
	if(input_password[num]=='@')
	{
		input_password[num]='1';
	}
	else if(input_password[num]=='9')
	{
		input_password[num]='1';
	}
	else
	{
		input_password[num]=input_password[num]+1;
	}
}
void change_to_sta(void)
{
	if((saved_password[0]==input_password[0])&&(saved_password[1]==input_password[1])&&(saved_password[2]==input_password[2]))
	{
		PSD_STA=0;
		count_flag=1;
		num_of_input=0;
		true_flase=1;
		Re_TIM2_Init();
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
 __HAL_TIM_CLEAR_IT(&htim2,TIM_CHANNEL_2);
	}
	else
	{
		true_flase=0;
		PSD_STA=1;
		num_of_input++;
		input_password[0]='@';
	  input_password[1]='@';
	  input_password[2]='@';
		if(num_of_input>=3)
		{
			count_flag=1;
		}
	}
}

void key_scan(void)
{
	if(PSD_STA==1)
	{
		if(B1==RESET)
		{
			HAL_Delay(5);
			if(B1==RESET)
			{
				password_change(0);
				while(!B1);
			}
		}
		else if(B2==RESET)
		{
			HAL_Delay(5);
			if(B2==RESET)
			{
				password_change(1);
				while(!B2);
			}
		}
		else if(B3==RESET)
		{
			HAL_Delay(5);
			if(B3==RESET)
			{
				password_change(2);
				while(!B3);
			}
		}
		else if(B4==RESET)
		{
			HAL_Delay(5);
			if(B4==RESET)
			{
				
				change_to_sta();
				while(!B4);
			}
		}
	}
}

/**********控制led灯的函数**********/
void control_led(void)
{
	if(led1)
	{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
	}
	else if(led2)
	{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
	}
	else
	{
		close_led();
	}
}

/**********界面切换函数**********/
void change_display(void)
{
	if(PSD_STA)
	{
		display_PSD();
	}
	else
	{
		display_STA();
	}
}

/**********主任务函数**********/
void project13(void)
{
	change_display();
	control_led();
	key_scan();
}
