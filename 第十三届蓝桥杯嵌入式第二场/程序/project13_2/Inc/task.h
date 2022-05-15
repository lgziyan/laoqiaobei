#ifndef __TASK_H
#define __TASK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
	
extern uint8_t led1;
extern uint8_t led2;	
extern uint8_t  led2_revese;
	
	
void close_leds(void);
void x24c02_write(uint8_t address,uint8_t inf);
uint8_t x24c02_read(uint8_t address);
void task_init(void);	
void display_shop(void);
void display_price(void);
void display_rep(void);	
void display(void);	
void add_price(uint8_t num);
void key_scan(void);	
void ok_B4task(void);
void change_TIM2_freq(uint32_t freq,uint8_t duty);
void control_leds(void);
void project(void);
void read_freq_duty(void);
#ifdef __cplusplus
}
#endif

#endif
