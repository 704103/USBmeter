#ifndef __LCD_H
#define __LCD_H
// #include "sys.h"
#include "stm32f10x.h"                                            // Device header
void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color); // 在指定区域内填充颜色
                                                                  // 清屏
void LCD_Clear(void);

void LCD_DrawPoint(u16 x, u16 y, u16 color); // 在指定位置绘制一个点

void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color); // 在两点之间绘制一条线

// Chinese(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode);

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);

void Draw_Circle(u16 x0, u16 y0, u8 r, u16 color);

void LCD_ShowChinese12x12(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode); // 显示12x12大小的中文

void LCD_ShowChinese16x16(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode); // 显示16x16大小的中文

void LCD_ShowChinese24x24(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode); // 显示24x24大小的中文

void LCD_ShowChinese32x32(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode); // 显示32x32大小的中文

void LCD_ShowChar(u16 x, u16 y, u8 num, u16 fc, u16 bc, u8 sizey, u8 mode); // 在指定位置显示一个字符

void LCD_ShowString(u16 x, u16 y, const u8 *p, u16 fc, u16 bc, u8 sizey, u8 mode); // 在指定位置显示一个字符串

u32 mypow(u8 m, u8 n); // 计算m的n次方

void LCD_ShowIntNum(u16 x, u16 y, u16 num, u8 len, u16 fc, u16 bc, u8 sizey); // 在指定位置显示一个整数

void LCD_ShowFloatNum1(u16 x, u16 y, float num, u8 len, u16 fc, u16 bc, u8 sizey); // 在指定位置显示一个浮点数（保留len位小数）

void LCD_ShowPicture(u16 x, u16 y, u16 length, u16 width, const u8 pic[]); // 在指定位置显示一张图片
// 将32位的RGB颜色值转换为16位的颜色值
#define GL_RGB_32_to_16(rgb) (((((unsigned int)(rgb)) & 0xFF) >> 3) | ((((unsigned int)(rgb)) & 0xFC00) >> 5) | ((((unsigned int)(rgb)) & 0xF80000) >> 8))

// 定义一些常见的颜色值
// 白色
#define WHITE 0xFFFF // 白色
// 黑色
#define BLACK 0x0000 // 黑色
// 蓝色
#define BLUE 0x001F // 蓝色
// 红色带蓝色
#define BRED 0XF81F // 红色带蓝色
// 绿色带红色
#define GRED 0XFFE0 // 绿色带红色
// 绿色带蓝色
#define GBLUE 0X07FF // 绿色带蓝色
// 红色
#define RED 0xF800 // 红色
// 洋红
#define MAGENTA 0xF81F // 洋红
// 绿色
#define GREEN 0x07E0 // 绿色
// 青色
#define CYAN 0x7FFF // 青色
// 黄色
#define YELLOW 0xFFE0 // 黄色
// 棕色
#define BROWN 0XBC40 // 棕色
// 深红带洋红
#define BRRED 0XFC07 // 深红带洋红
// 灰色
#define GRAY 0X8430 // 灰色
// 深蓝色
#define DARKBLUE 0X01CF // 深蓝色
// 浅蓝色
#define LIGHTBLUE 0X7D7C // 浅蓝色
// 灰蓝色
#define GRAYBLUE 0X5458 // 灰蓝色
// 浅绿色
#define LIGHTGREEN 0X841F // 浅绿色
// 亮灰色
#define LGRAY 0XC618 // 亮灰色
// 亮灰蓝色
#define LGRAYBLUE 0XA651 // 亮灰蓝色
// 浅蓝蓝色
#define LBBLUE 0X2B12 // 浅蓝蓝色

#endif
