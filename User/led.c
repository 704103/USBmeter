#include "stm32f10x.h" 

//定时器1 每1秒绿灯闪烁
//中断 按下KEY1 红灯亮,松开灭
//按下KEY2 蓝灯开始呼吸,再按下关闭
//KEY1 PA0   KEY PC13     LED_G  PB0   LED_R PB5   TIM3_CH3    LED_B PB1 TIM3_CH4
//先初始化LED ,KEY,再定时器,PWM

int key1=0;
int key2=0;
int tim3=0;