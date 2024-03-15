#include "stm32f10x.h" // Device header
#include "KEY.h"
#include "TIM.h"
#include "USART.h"
#include <stdio.h>
#include <stdlib.h>
#include "ADC.h"
#include "Delay.h"
#include "SPI.h"
#include "lcd.h"
#include "lcd_init.h"
#include "ui_measure.h"
#include "menu.h"
#include "list.h"
int main(void)
{
	// led_init();
	key_init();
	usart1_init(9600);
	Delay_us(100);
	TIM6_Init(10);
	adc1_init();
	LCD_Init();

	LCD_Clear();
	// 回调函数,显示,交互
	tMenuCfg.pfnCurrentShowFun = MenuList_Show;
	tMenuCfg.pfnCurrentHmiFun = MenuList_Hmi;

	tMenuCfg.pfnSupMenuFun = &tMenuList[0];
	tMenuCfg.pfnCurrenMenuFun = &tMenuList[0]; // 赋值给当前菜单
	while (1)
	{
		// tMenuCfg.pfnCurrentShowFun();
		// tMenuCfg.pfnCurrentHmiFun(tMenuCfg.Key);
 
		MenuList_Show(tMenuCfg.pfnCurrenMenuFun);
		MenuList_Hmi(tMenuCfg.Key);
		Delay_ms(500);
	}
}
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line11) || EXTI_GetITStatus(EXTI_Line12) || EXTI_GetITStatus(EXTI_Line13) == SET)
	{
		printf("\r\n1");
		EXTI_ClearITPendingBit(EXTI_Line11);
		EXTI_ClearITPendingBit(EXTI_Line12);
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}
// 定时器6中断服务程序
void TIM6_IRQHandler(void)							   // TIM6中断
{													   // 清除TIMx更新中断标志
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) // 检查TIM6更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		static uint16_t sum = 0;
		int8_t temp = key_scan();
		if (temp >= 0) // 有按键按下时返回,无按键按下不返回
		{
			tMenuCfg.Key = temp;
		}
		sum++;
		// if (tMenuCfg.Key >= 0)
		// {
		// 	printf("KEY=%d\r\n", tMenuCfg.Key);
		// 	printf("ID=%d\r\n", tMenuCfg.CurrentID);
		// }
		if (sum >= 100)
		{
			sum = 0;
			tPower_Value.Wh += tPower_Value.Power * 0.0002777f;
			tPower_Value.Time += 1;
			// printf(" ADC:%.2fV \r\n", tADC_Value.current);
		}
	}
}
