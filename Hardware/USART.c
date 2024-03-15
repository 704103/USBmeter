#include "stm32f10x.h"
#include "USART.h"
#include "stdio.h"
//////////////////////////////////////////////////////////////////
// 加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
// 标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
// 重定义fputc函数
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0)
        ; // 循环发送,直到发送完毕
    USART1->DR = (u8)ch;
    return ch;
}
#endif

uint16_t res=0;
void usart1_init(uint16_t Baud)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStrut;
    USART_InitTypeDef USART_InitStrut;
    NVIC_InitTypeDef NVIC_InitStrut;

    GPIO_InitStrut.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStrut.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStrut.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStrut);

    GPIO_InitStrut.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStrut.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStrut.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStrut);

    USART_InitStrut.USART_BaudRate = Baud;
    USART_InitStrut.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStrut.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStrut.USART_Parity = USART_Parity_No;
    USART_InitStrut.USART_StopBits = USART_StopBits_1;
    USART_InitStrut.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStrut);

    NVIC_InitStrut.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStrut.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStrut.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStrut.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStrut);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        res = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
// void usart1_seeddata(char bit)
// {

// }
