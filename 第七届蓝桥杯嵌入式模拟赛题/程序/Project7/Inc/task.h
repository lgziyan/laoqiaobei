#ifndef __TASK_H
#define __TASK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

	
extern uint8_t led1_on;	
extern uint8_t star_counting;
extern uint8_t Recive_str[50];
extern float k;	

void OC_PD2(void);
void close_leds(void);
uint16_t getADCR37(void);
void Set_RTCtime(uint8_t hours,uint8_t minutes,uint8_t seconds);
void RTC_TimeShow(void);	
void display_data(void);
void display_setting(void);
void display(void);	
void control_leds(void);
void x24c02_write(uint8_t address,uint8_t infor);	
uint8_t x24c02_read(uint8_t address);
void task_init(void);
//void HAL_UART_IdleCpltCallback(UART_HandleTypeDef *huart);
void time_to_send_volt(void);
void change_send_time(uint8_t pos);
void key_scan(void);
void project7(void);
#ifdef __cplusplus
}
#endif

#endif
