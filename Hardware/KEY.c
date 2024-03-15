#include "stm32f10x.h"
#include "KEY.h"
tKeyStatus_Struct tKeyStatus;
/*
按键扫描程序
按下立马返回单击
长按键需要在按下按键时累计时长判断是否达到长按时间,在不够时长时还需要松开后再返回单击 
*/

#define KEY_COUNT 3			 // 按键总数
#define SHORT_KEY_DELAY 2	 // 短按延时
#define LONG_KEY_DELAY 100	 // 长按延时
#define DOUBLE_KEY_DELAY 30	 // 双击两次触发之间的最大延时
#define CNTINUS_KEY_DELAY 20 // 连续触发延时
/**
 * @brief 扫描按键长按
 *
 * 检测指定按键（GPIOE的Pin_11）是否长按，并返回相应结果。
 *
 * @return 如果检测到长按，返回5；否则返回-1。
 */
int8_t key_scan_long(void)
{
	uint8_t key_input = 1;			// 初始化按键输入状态为未按下
	int8_t res = -1;				// 初始化返回结果为-1
	static uint16_t key_status = 0; // 静态变量，用于记录按键按下的持续时间
	static uint8_t key_flag = 1;	// 静态变量，用于标记按键状态（是否已被识别）

	// 读取按键输入状态
	key_input = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11);

	if (key_input == 0) // 如果按键被按下
	{
		key_status++;								  // 增加按键按下的持续时间
		if (key_status >= LONG_KEY_DELAY && key_flag) // 如果按键按下的时间超过长按阈值且按键状态未被标记
		{
			key_flag = 0; // 标记按键状态为已识别
			res = 3;	  // 设置返回结果为长按识别码
		}
	}
	else // 如果按键未被按下
	{
		if (key_status >= SHORT_KEY_DELAY && key_flag) // 如果之前按键被按下并且达到消除抖动的阈值
		{
			key_flag = 0; // 标记按键状态为已识别
			res = 2;	  // 设置返回结果为单击识别码
		}
		key_flag = 1;	// 重置按键状态标记
		key_status = 0; // 重置按键持续时间
	}

	return res; // 返回识别结果
}

/**
 * @brief 扫描按键
 *
 * 扫描多个按键（GPIOE的Pin_13和Pin_12），并返回按下按键的索引。
 *
 * @return 如果检测到按键单击，返回按键索引（0或1）；否则返回-1或长按识别码。
 */
int8_t key_scan(void)
{
	uint8_t key_input[KEY_COUNT] = {1};			 // 初始化按键输入数组，所有按键默认未按下
	int8_t res = key_scan_long();				 // 调用长按扫描函数，处理可能的长按事件
	static uint16_t key_status[KEY_COUNT] = {0}; // 静态数组，用于记录每个按键按下的持续时间
	static uint8_t key_flag[KEY_COUNT] = {1};	 // 静态数组，用于标记每个按键状态（是否已被识别）

	// 读取每个按键的输入状态
	key_input[0] = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13);
	key_input[1] = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12);

	for (uint8_t i = 0; i < KEY_COUNT - 1; i++) // 遍历每个按键
	{
		if (key_input[i] == 0) // 如果按键被按下
		{
			key_status[i]++;					   // 增加按键按下的持续时间
			if (key_status[i] >= 2 && key_flag[i]) // 如果按键按下的时间超过消除抖动的阈值且按键状态未被标记
			{
				key_flag[i] = 0; // 标记按键状态为已识别
				res = i;		 // 设置返回结果为当前按键的索引
			}
		}
		else // 如果按键未被按下
		{
			key_flag[i] = 1;   // 重置按键状态标记
			key_status[i] = 0; // 重置按键持续时间
		}
	}

	return res; // 返回识别结果
}

void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStrct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStrct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStrct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStrct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStrct);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	// GPIO_SetBits(GPIOB,GPIO_Pin_5);
}
void key2_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitTypeDef GPIO_InitStrct;
	GPIO_InitStrct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStrct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStrct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStrct);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);

	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line13;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_Init(&NVIC_InitStruct);
}
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	// GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource11);
	// GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource12);
	// GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource12);

	// EXTI_InitTypeDef EXTI_InitStruct;
	// EXTI_InitStruct.EXTI_Line = EXTI_Line11 | EXTI_Line12 | EXTI_Line13;
	// EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	// EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	// EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	// EXTI_Init(&EXTI_InitStruct);

	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// NVIC_InitTypeDef NVIC_InitStruct;
	// NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	// NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	// NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	// NVIC_Init(&NVIC_InitStruct);
}
