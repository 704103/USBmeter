
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "ui_measure.h"
#include "lcd.h"
#include "list.h"

tMenuCfg_Struct tMenuCfg = {0, -1};
tCurrent_Struct Current;

tMenuList_Struct tMenuList[] = {
    {"MENU   ", UI_Measure_Load, UI_Measure_Exit, UI_Measure_Task, NULL, &tMenuList[1], 2},
    {"measure", UI_Measure_Load, UI_Measure_Exit, UI_Measure_Task, NULL, NULL, 0},
    {"adj    ", UI_Measure_Load, UI_Measure_Exit, UI_Measure_Task, NULL, NULL, 0},
};
void MenuList_Show(tMenuList_Struct *tMenu)
{
    if (tMenu->subMenu != NULL)
    {
        // 如果当前菜单项有子菜单，则显示子菜单项
        for (int i = 0; i < tMenu->subMenuCount; ++i)
        {
            // printf("%s\r\n", tMenu->subMenu[i].describe); 
            LCD_ShowString(52, 111+i*52, (u8 *)tMenu->subMenu[i].describe, CYAN, BLACK, 48, 0); //
        }
    }
    else
    {
        // 如果没有子菜单，则显示当前菜单项的功能
        printf("执行功能 %s\r\n", tMenu->describe);
        if (tMenu->pfnMenuShow)
        {
            tMenu->pfnMenuShow(); // 执行功能
        }
    }
}
void MenuList_Minus(void)
{
    if (tMenuCfg.CurrentID > 0)
    {
        tMenuCfg.CurrentID--;
    }
    else
    {
        tMenuCfg.CurrentID = MENU_LIST_MAX - 1;
    }
}
void MenuList_Add(void)
{
    if (tMenuCfg.CurrentID < MENU_LIST_MAX - 1)
    {
        tMenuCfg.CurrentID++;
    }
    else
    {
        tMenuCfg.CurrentID = 0;
    }
}
void MenuList_Enter(tMenuList_Struct *tMenu)
{
    if (tMenu->subMenu != NULL)
    {
        tMenuCfg.pfnSupMenuFun = tMenu;                                  // 当前菜单赋值保存
        tMenuCfg.pfnCurrenMenuFun = &tMenu->subMenu[tMenuCfg.CurrentID]; // 下级菜单赋值给函数指针
        tMenu->pfnMenuLoad();
    }
    else
    {
        return; // 退出
    }  
}
void MenuList_Exti(void)
{
    if (tMenuCfg.pfnSupMenuFun != NULL)
    {
        tMenuCfg.pfnCurrenMenuFun->pfnMenuLoad();
        tMenuCfg.pfnCurrenMenuFun = tMenuCfg.pfnSupMenuFun; // 赋值给当前菜单函数指针
    }
    else
    {
        return; // 退出
    }
}
void MenuList_Hmi(int8_t key)
{
    switch (key)
    {
    case KEY0_CLICK:
        MenuList_Minus();
        break;
    case KEY1_CLICK:
        MenuList_Add();
        break;
    case KEY2_CLICK:
        MenuList_Enter(tMenuList);
        break;
    case KEY2_LONG:
        MenuList_Exti();
        break;

    default:
        break;
    }

    tMenuCfg.Key = -1; // 执行完,按键归位,不占用中断资源
}
