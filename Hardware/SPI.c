#include "stm32f10x.h"
#include "SPI.h"
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  // 使能SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能GPIOA

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  // 复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  // 初始化GPIOA
	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;		   // 只发送模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					   // 设置SPI工作模式：主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				   // 设置SPI数据大小：8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   // 串行同步时钟空闲时SCLK位高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;					   // 串行同步时钟空第二个时钟沿捕获
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						   // NSS信号由硬件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // 波特率预分频值：波特率预分频值为4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   // 数据传输高位先行
	SPI_InitStructure.SPI_CRCPolynomial = 7;						   // CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);								   // 初始化SPI
	SPI_Cmd(SPI1, ENABLE);											   // 使能SPI
}

u16 DMA1_MEM_LEN; // 保存DMA每次数据传送的长度
// DMA1的各通道配置
// 这里的传输形式是固定的,这点要根据不同的情况来修改
// 从存储器->外设模式/8位数据宽度/存储器增量模式
// DMA_CHx:DMA通道CHx
// cpar:外设地址
// cmar:存储器地址
// cndtr:数据传输量
void MYDMA_Config(DMA_Channel_TypeDef *DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // 使能DMA传输

	DMA_DeInit(DMA_CHx); // 将DMA的通道1寄存器重设为缺省值
	DMA1_MEM_LEN = cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;						// DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;							// DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						// 数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = cndtr;								// DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		// 外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					// 内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			// 数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							// 工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					// DMA通道 x拥有中优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							// DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);									// 根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
}
void MYDMA_Config1(DMA_Channel_TypeDef *DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // 使能DMA传输

	DMA_DeInit(DMA_CHx); // 将DMA的通道1寄存器重设为缺省值
	DMA1_MEM_LEN = cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;							// DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;								// DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;							// 数据传输方向，从内存读取发送到外设
	DMA_InitStructure.DMA_BufferSize = cndtr;									// DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			// 外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;					// 内存地址寄存器不变
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			// 数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								// 工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						// DMA通道 x拥有中优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								// DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);										// 根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
}

// 开启一次DMA传输
void MYDMA_Enable(DMA_Channel_TypeDef *DMA_CHx)
{
	DMA_Cmd(DMA_CHx, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel3, DMA1_MEM_LEN);
	DMA_Cmd(DMA_CHx, ENABLE);
}
