#ifndef MENU_H
#define MENU_H
#include <stdint.h>
#include "stm32f10x.h" // Device header
#define MENU_LIST_MAX 2
#define KEY0_CLICK 0
#define KEY1_CLICK 1
#define KEY2_CLICK 2
#define KEY2_LONG 3
typedef void (*MenuFun_F)();
typedef struct tMenuList_Struct
{
    const u8 *describe; // 当前菜单描述
    // const char *engdescribe; // 当前菜单英语描述
    MenuFun_F pfnMenuLoad;
    MenuFun_F pfnMenuExit;
    MenuFun_F pfnMenuShow;
    MenuFun_F pfnMenuHmi;             // 交互
    struct tMenuList_Struct *subMenu; // 下一级菜单
    uint8_t subMenuCount;             // 下一级菜单总数
} tMenuList_Struct;
typedef struct
{
    uint8_t CurrentID; // 当前菜单
    int8_t Key;
    MenuFun_F pfnCurrentShowFun; // 当前显示,回调函数
    MenuFun_F pfnCurrentHmiFun;  // 当前交互
    tMenuList_Struct *pfnCurrenMenuFun;    // 当前菜单
    tMenuList_Struct *pfnSupMenuFun;    // 上一级菜单
} tMenuCfg_Struct;
typedef struct
{
    MenuFun_F pfnMenuShowFun;
    MenuFun_F pfnMenuHmiFun;
} tCurrent_Struct;
extern tMenuList_Struct tMenuList[];
extern tMenuCfg_Struct tMenuCfg;
extern tCurrent_Struct Current;
void MenuList_Show(tMenuList_Struct *tMenu);
void MenuList_Minus(void);
void MenuList_Add(void);
void MenuList_Enter(tMenuList_Struct *tMenu);
void MenuList_Exti(void);
void MenuList_Hmi(int8_t key);

#endif
