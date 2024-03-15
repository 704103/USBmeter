#include "stm32f10x.h"
#include "adc.h"
#include "Delay.h"
#include "USART.h"
#include <stdio.h>
uint16_t AD_Value[2] = {0};
tADC_Original tADC_OriginalValue = {5};
struct tADC_Value_Struct tADC_Value = {0};

// PA2 ---ADC1_通道2  PC4---ADC1_通道14
//  初始化ADC
void adc1_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	GPIO_InitTypeDef GPIO_InitStrtu;
	GPIO_InitStrtu.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStrtu.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStrtu.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStrtu);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_55Cycles5); // 将ADC通道2配置为常规转换序列3的第三个通道，采样时间为55.5个周期

	ADC_InitTypeDef ADC_Initstructure;
	ADC_Initstructure.ADC_ContinuousConvMode = ENABLE;
	ADC_Initstructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Initstructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Initstructure.ADC_Mode = ADC_Mode_Independent;
	ADC_Initstructure.ADC_NbrOfChannel = 1;
	ADC_Initstructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1, &ADC_Initstructure);

	// ADC_ResetCalibration(ADC1);
	// while (ADC_GetResetCalibrationStatus(ADC1)) ;
	// ADC_StartCalibration(ADC1);
	// while (ADC_GetCalibrationStatus(ADC1)) ;

	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;				  // 外设地址为ADC1数据寄存器地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	  // 外设数据长度为半字
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			  // 外设地址不变
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&tADC_OriginalValue.current; // 存储器地址为AD_Value数组地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			  // 存储器数据长度为半字
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						  // 存储器地址自增
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							  // 数据传输方向为外设到存储器
	DMA_InitStructure.DMA_BufferSize = 1;										  // 数据缓存区大小为4
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								  // 循环模式
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								  // 禁止存储器到存储器的传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						  // DMA优先级为中等
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);								  // 配置DMA通道1

	DMA_Cmd(DMA1_Channel1, ENABLE); // 打开DMA1通道1
	ADC_DMACmd(ADC1, ENABLE);		// 使能ADC1的DMA模式
	ADC_Cmd(ADC1, ENABLE);			// 启用ADC1
	ADC_ResetCalibration(ADC1);		// 复位ADC1校准寄存器
	while (ADC_GetResetCalibrationStatus(ADC1) == SET)
		;						// 等待校准寄存器复位完成
	ADC_StartCalibration(ADC1); // 开始执行ADC校准
	while (ADC_GetCalibrationStatus(ADC1) == SET)
		;									// 等待校准完成
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 启动软件转换
}

// 将ADC数值uint16_t转换为模拟量float
void ADC_GetValue(void)
{
	// 去噪,注意uint16_t最大65536范围
	uint8_t count = 10;
	tADC_Value.current = 0; //
	// tADC_Value.voltage = 0;// 未定义电压采样ADC
	for (uint8_t i = 0; i < count; i++)
	{
		tADC_Value.current += tADC_OriginalValue.current;
		// tADC_Value.voltage += tADC_OriginalValue.voltage;	 //未定义电压采样ADC
		Delay_us(10);
	}
	tADC_Value.current = tADC_Value.current / (float)count * 3.3f / 4095.0f;
	tADC_Value.voltage = tADC_OriginalValue.voltage * 1.0f;
	// tADC_Value.voltage = tADC_Value.voltage / (float)count * 3.3f / 4095.0f;  //未定义电压采样ADC
}
