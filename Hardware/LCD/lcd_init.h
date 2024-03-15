#ifndef LCD_INIT_H
#define LCD_INIT_H
#include "stm32f10x.h" // Device header
// #include "sys.h"

#define USE_HORIZONTAL 2   


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 280

#else
#define LCD_W 280
#define LCD_H 240
#endif




//-----------------LCD绔�鍙ｅ畾涔�---------------- 

// #define LCD_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)//SCL=SCLK
// #define LCD_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)

// #define LCD_MOSI_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SDA=MOSI
// #define LCD_MOSI_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define LCD_RES_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_6)//RES
#define LCD_RES_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define LCD_DC_Clr()   GPIO_ResetBits(GPIOA,GPIO_Pin_4)//DC
#define LCD_DC_Set()   GPIO_SetBits(GPIOA,GPIO_Pin_4)
 		     
#define LCD_CS_Clr()   GPIO_ResetBits(GPIOE,GPIO_Pin_5)//CS
#define LCD_CS_Set()   GPIO_SetBits(GPIOE,GPIO_Pin_5)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOE,GPIO_Pin_6)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOE,GPIO_Pin_6)



void LCD_GPIO_Init(void);// 初始化GPIO
void LCD_Writ_Bus(u8 dat);// 写入SPI数据
void LCD_WR_DATA8(u8 dat);// 写入一个字节数据
void LCD_WR_DATA(u16 dat);// 写入两个字节数据
void LCD_WR_REG(u8 dat);// 写入一个寄存器
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);// 设置坐标函数
void LCD_Init(void);//LCD初始化

#endif




