#ifndef __TASK_H
#define __TASK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
extern uint8_t count_flag;
extern uint8_t	PSD_STA;
extern uint8_t true_flase;
extern uint8_t led1;
extern uint8_t led2;
extern uint8_t input_password[3];
extern uint8_t saved_password[3];
void close_led(void);
void	display_PSD(void);
void	display_STA(void);

void Re_TIM2_Init(void);
void password_change(uint8_t num);
void change_to_sta(void);
void key_scan(void);
void control_led(void);
void change_display(void);
void project13(void);
#ifdef __cplusplus
}
#endif

#endif

