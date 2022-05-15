/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t AUTO_MANU=1;
uint8_t Data_Para=1;
uint8_t Duty_A6=10;
uint8_t Duty_A7=10;
uint8_t Flag_count1=0;
uint8_t Flag_count2=0;
uint8_t str_it[20];
#define B1 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)
#define B2 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)
#define B3 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)
#define B4 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void keyscan(void)
{
	if(Data_Para)
	{
			if(B1==RESET)
			{
				HAL_Delay(10);
				if(B1==RESET)
				{
					Data_Para=!Data_Para;
					LCD_Clear(Black);
//					LCD_SetBackColor(Black);
//					LCD_SetTextColor(White);
					mode_change();
					Control_led12();
				}
				while(!B1);	
				
			}
			else if(B4==RESET)
			{
				HAL_Delay(10);
				if(B4==RESET)
				{
					AUTO_MANU=!AUTO_MANU;
					LCD_Clear(Black);
//					LCD_SetBackColor(Black);
//					LCD_SetTextColor(White);
					mode_change();
					Control_led12();
				}
				while(!B4);
				
			}			
	}
	else
	{
		if(B1==RESET)
		{
			HAL_Delay(10);
			if(B1==RESET)
			{
				Data_Para=!Data_Para;
				LCD_Clear(Black);
//				LCD_SetBackColor(Black);
//				LCD_SetTextColor(White);
				mode_change();
				Control_led12();
			}
			while(!B1);	
			
		}
		if(!AUTO_MANU)
		{
		 if(B2==RESET)
		 {
			HAL_Delay(10);
			if(B2==RESET)
			{
				Duty_A6=Duty_A6+10;
				if(Duty_A6>90)
				Duty_A6=10;
			}
			while(!B2);	
		 }
		 else if(B3==RESET)
		 {
			HAL_Delay(10);
			if(B3==RESET)
			{
				Duty_A7=Duty_A7+10;
				if(Duty_A7>90)
				Duty_A7=10;
			}
			while(!B3);		
		 }
        }
	}
	
}

void mode_change(void)
{
	if(Data_Para&&AUTO_MANU)
	{
		Display_Data_Auto();
		
		
	}
	else if(Data_Para&&(!AUTO_MANU))
	{
		Display_Data_MANU();
	}
	else if(!Data_Para)
	{
		Display_Para();
	}
}

void Real_Time_Show(void)
{
	if(Flag_count1==1)
	{
		Flag_count1=0;
	if(Data_Para)
	{
	sprintf((char*)str_it,"   V:%.2fV          ",Get_R37value()*3.3/4096);
	LCD_DisplayStringLine(Line2,str_it);
	}
	}
	else if(Flag_count2==1)
	{
		Flag_count2=0;
		if(AUTO_MANU&&(!Data_Para))
		{
			sprintf((char*)str_it,"  PA6:%2d%%         ",Get_R37value()*100/4096);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,Get_R37value()*10000/4096);
			LCD_DisplayStringLine(Line2,str_it);
			sprintf((char*)str_it,"  PA7:%2d%%         ",Get_R37value()*100/4096);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,Get_R37value()*5000/4096);//time3计数值为10000，time17计数值为5000
			LCD_DisplayStringLine(Line4,str_it);
		}
		else if((!AUTO_MANU)&&(!Data_Para))
		{
			sprintf((char*)str_it,"  PA6:%2d%%         ",Duty_A6);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,100*Duty_A6);
			LCD_DisplayStringLine(Line2,str_it);
			sprintf((char*)str_it,"  PA7:%2d%%         ",Duty_A7);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,50*Duty_A7);
			LCD_DisplayStringLine(Line4,str_it);
		}
		else if(AUTO_MANU&&Data_Para)
		{
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,Get_R37value()*10000/4096);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,Get_R37value()*5000/4096);
			
		}
		else if((!AUTO_MANU)&&Data_Para)
		{
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,100*Duty_A6);
			__HAL_TIM_SET_COMPARE(&htim17,TIM_CHANNEL_1,50*Duty_A7);
			
		}
	}	
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC2_Init();
  MX_TIM3_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
	LCD_Init();
	Close_led();
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	Display_Data_Auto();
	Control_led12();
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  
	  keyscan();
	  Real_Time_Show();
	  //mode_change();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks 
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
