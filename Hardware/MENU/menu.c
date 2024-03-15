
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
        // �����ǰ�˵������Ӳ˵�������ʾ�Ӳ˵���
        for (int i = 0; i < tMenu->subMenuCount; ++i)
        {
            // printf("%s\r\n", tMenu->subMenu[i].describe); 
            LCD_ShowString(52, 111+i*52, (u8 *)tMenu->subMenu[i].describe, CYAN, BLACK, 48, 0); //
        }
    }
    else
    {
        // ���û���Ӳ˵�������ʾ��ǰ�˵���Ĺ���
        printf("ִ�й��� %s\r\n", tMenu->describe);
        if (tMenu->pfnMenuShow)
        {
            tMenu->pfnMenuShow(); // ִ�й���
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
        tMenuCfg.pfnSupMenuFun = tMenu;                                  // ��ǰ�˵���ֵ����
        tMenuCfg.pfnCurrenMenuFun = &tMenu->subMenu[tMenuCfg.CurrentID]; // �¼��˵���ֵ������ָ��
        tMenu->pfnMenuLoad();
    }
    else
    {
        return; // �˳�
    }  
}
void MenuList_Exti(void)
{
    if (tMenuCfg.pfnSupMenuFun != NULL)
    {
        tMenuCfg.pfnCurrenMenuFun->pfnMenuLoad();
        tMenuCfg.pfnCurrenMenuFun = tMenuCfg.pfnSupMenuFun; // ��ֵ����ǰ�˵�����ָ��
    }
    else
    {
        return; // �˳�
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

    tMenuCfg.Key = -1; // ִ����,������λ,��ռ���ж���Դ
}
