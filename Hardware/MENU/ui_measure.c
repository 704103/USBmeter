#include "ui_measure.h"
#include "adc.h"
#include "lcd.h"
#include "lcd_init.h"
#include "stdbool.h" 
#include <stdio.h> 

tPower_ValueStruct tPower_Value = {0};
bool ConnectFlag = false; // 连接标志
bool MeasureFlag = false; // 测量标志
void UI_Measure_Load(void)
{
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK); /*! 160*80 初始化 */
    MeasureFlag = true;
    ConnectFlag = false;
}

void UI_Measure_Exit(void)
{
    MeasureFlag = false;
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK); /*! 160*80 初始化 */
}

void UI_Measure_Task(void)
{
    if (1)
    {
        char strbuf[256];
        static float AH = 0;
        uint8_t location_x = 64 - 12, location_y = 0;
        uint8_t distance = 0, size = 48;
        ADC_GetValue();
        sprintf(strbuf, "v %.2f ", tADC_Value.voltage);
        LCD_ShowString(52, location_y + 4, (u8 *)strbuf, CYAN, BLACK, 48, 0); //

        if (tADC_Value.voltage >= 10) // 电流显示对齐电压显示
        {
            sprintf(strbuf, "a  %.2f ", tADC_Value.current);
            LCD_ShowString(52, 48, (u8 *)strbuf, LGRAYBLUE, BLACK, 48, 0); //
        }
        else
        {
            sprintf(strbuf, "a %.2f ", tADC_Value.current);
            LCD_ShowString(52, 48, (u8 *)strbuf, LGRAYBLUE, BLACK, 48, 0); //
        }

        tPower_Value.Power = tADC_Value.current * tADC_Value.voltage;
        sprintf(strbuf, "w %.2f ", tPower_Value.Power);
        LCD_ShowString(52, 96, (u8 *)strbuf, BRRED, BLACK, 48, 0); //

        // WH+=tPower_Value.Power*0.00002777f; 
        sprintf(strbuf, "%.2fWh", tPower_Value.Wh);
        LCD_ShowString(4, 154, (u8 *)strbuf, LBBLUE, BLACK, 32, 0); //

        AH = tPower_Value.Wh * 1000.0f /tADC_Value.voltage;
        sprintf(strbuf, "%.0fmA", AH);
        LCD_ShowString(4, 186, (u8 *)strbuf, LIGHTGREEN, BLACK, 32, 0); //

        sprintf(strbuf, "%.2f()", tADC_Value.temp);  //温度
        LCD_ShowString(144+16, 154, (u8 *)strbuf, RED, BLACK, 32, 0); //

        // 开机时间显示
        LCD_ShowString(144 + 16 * 2, location_y + size * 3 + distance * 4 + 32 + 10, ":", WHITE, BLACK, 32, 0); //
        LCD_ShowString(144 + 16 * 5, location_y + size * 3 + distance * 4 + 32 + 10, ":", WHITE, BLACK, 32, 0); //
        LCD_ShowIntNum(144, location_y + size * 3 + distance * 4 + 32 + 10, (uint16_t)(tPower_Value.Time / 3600), 2, WHITE, BLACK, 32);             //
        LCD_ShowIntNum(144 + 16 * 3, location_y + size * 3 + distance * 4 + 32 + 10, (uint16_t)(tPower_Value.Time / 60 % 60), 2, WHITE, BLACK, 32); //
        LCD_ShowIntNum(144 + 16 * 6, location_y + size * 3 + distance * 4 + 32 + 10, (uint16_t)(tPower_Value.Time % 60), 2, WHITE, BLACK, 32);

        LCD_Fill(28, 146, 256, 149, LGRAY);

        // sprintf(strbuf, "D+:%.2fV ", tADC_Value.SampleVoltageDP);
        // LCD_ShowString(92, 0+80, (u8 *)strbuf, GREEN, BLACK, 12, 0); //

        // sprintf(strbuf, "D-:%.2fV ", tADC_Value.SampleVoltageDN);
        // LCD_ShowString(92, 12+80, (u8 *)strbuf, GREEN, BLACK, 12, 0); //
    }
    else
    {
        LCD_ShowString(0, 16 + 80, "please connect the ", WHITE, BLACK, 16, 0);  //
        LCD_ShowString(0, 32 + 80, "device,then push the", WHITE, BLACK, 16, 0); //
        LCD_ShowString(0, 48 + 80, " OK key.", WHITE, BLACK, 16, 0);             //
    }
}
