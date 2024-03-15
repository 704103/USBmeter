#include "menu.h"
#include "ui_measure.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Delay.h" 
MenuItem *tCurrent_Menu = NULL; // 初始化为NULL，表示没有当前菜单
MenuItem *supItems = NULL;      // 上级
// 假设这些函数已经定义好了，用来执行相应的功能
void functionA1() { printf("执行功能A1\n"); }
void functionA2() { printf("执行功能A2\n"); }
void functionB1() { printf("执行功能B1\n"); }
void functionB2() { printf("执行功能B2\n"); }

// 初始化菜单项
MenuItem menuItems[] = {
    {"Q", NULL, &menuItems[1], 2, NULL},
    {"A", NULL, &menuItems[3], 2, NULL},
    {"B", NULL, &menuItems[5], 2, NULL},
    {"1", NULL, NULL, 0, functionA1},
    {"2", NULL, NULL, 0, functionA2},
    {"C", NULL, NULL, 0, functionB1},
    {"D", NULL, NULL, 0, functionB2},
    // ... 可以继续添加更多菜单项
};

/*
// 为A和B分配子菜单项,占用很多RAM,改用初始化赋值
void initSubMenus(void)
{
    menuItems[0].subItems = (MenuItem *)malloc(2 * sizeof(MenuItem));
    menuItems[0].subItems[0] = menuItems[2];
    menuItems[0].subItems[1] = menuItems[3];
    menuItems[0].subItemCount = 2;

    menuItems[1].subItems = (MenuItem *)malloc(2 * sizeof(MenuItem));
    menuItems[1].subItems[0] = menuItems[4];
    menuItems[1].subItems[1] = menuItems[5];
    menuItems[1].subItemCount = 2;
}
*/
void showMenu(MenuItem *item)
{
    if (item->subItems != NULL)
    {
        // 如果当前菜单项有子菜单，则显示子菜单项
        for (int i = 0; i < item->subItemCount; ++i)
        {
            printf("%s\r\n", item->subItems[i].name);
        }
    }
    else
    {
        // 如果没有子菜单，则显示当前菜单项的功能
        printf("执行功能 %s\r\n", item->name);
        if (item->function)
        {
            item->function(); // 执行功能
        }
    }
}
void MenuEnter(MenuItem *item)
{
    if (item->subItems != NULL)
    {
        supItems = item;                                     // 上级菜单赋值保存
        tCurrent_Menu = &item->subItems[tMenuCfg.CurrentID]; // 赋值给当前菜单函数指针
    }
    else
    {
        return; // 退出
    }
}
void MenuExit(MenuItem *item)
{
    if (supItems != NULL)
    {
        tCurrent_Menu = supItems; // 赋值给当前菜单函数指针
    }
    else
    {
        return; // 退出
    }
}

void MenuKey_List(int8_t key)
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
        MenuEnter(tCurrent_Menu);
        break;

    case KEY2_LONG:
        printf("\r\nLONG");
        MenuExit(tCurrent_Menu);
        break;

    default:
        break;
    }

    tMenuCfg.Key = -1; // 执行完,按键归位,不占用中断资源
}

void displayMenu(MenuItem *item)
{
    printf("\n--- 菜单 ---\n");
    if (item->subItems)
    {
        // 如果当前菜单项有子菜单，则显示子菜单项
        for (int i = 0; i < item->subItemCount; ++i)
        {
            printf("%s\n", item->subItems[i].name);
        }
    }
    else
    {
        // 如果没有子菜单，则显示当前菜单项的功能
        printf("执行功能 %s\n", item->name);
        if (item->function)
        {
            item->function(); // 执行功能
        }
        return; // 退出递归
    }

    char choice[20];
    printf("请输入选择（或输入'back'返回上一级菜单）: ");
    scanf("%19s", choice); // 限制输入长度，防止溢出

    if (strcmp(choice, "back") == 0)
    {
        // 用户选择返回上一级菜单
        return;
    }

    // 查找用户选择的子菜单项
    for (int i = 0; i < item->subItemCount; ++i)
    {
        if (strcmp(item->subItems[i].name, choice) == 0)
        {
            // 递归显示选择的子菜单项
            displayMenu(&item->subItems[i]);
            break; // 找到后不再继续查找
        }
    }
}
