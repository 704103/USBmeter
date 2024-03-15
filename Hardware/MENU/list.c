#include "menu.h"
#include "ui_measure.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Delay.h" 
MenuItem *tCurrent_Menu = NULL; // ��ʼ��ΪNULL����ʾû�е�ǰ�˵�
MenuItem *supItems = NULL;      // �ϼ�
// ������Щ�����Ѿ�������ˣ�����ִ����Ӧ�Ĺ���
void functionA1() { printf("ִ�й���A1\n"); }
void functionA2() { printf("ִ�й���A2\n"); }
void functionB1() { printf("ִ�й���B1\n"); }
void functionB2() { printf("ִ�й���B2\n"); }

// ��ʼ���˵���
MenuItem menuItems[] = {
    {"Q", NULL, &menuItems[1], 2, NULL},
    {"A", NULL, &menuItems[3], 2, NULL},
    {"B", NULL, &menuItems[5], 2, NULL},
    {"1", NULL, NULL, 0, functionA1},
    {"2", NULL, NULL, 0, functionA2},
    {"C", NULL, NULL, 0, functionB1},
    {"D", NULL, NULL, 0, functionB2},
    // ... ���Լ�����Ӹ���˵���
};

/*
// ΪA��B�����Ӳ˵���,ռ�úܶ�RAM,���ó�ʼ����ֵ
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
        // �����ǰ�˵������Ӳ˵�������ʾ�Ӳ˵���
        for (int i = 0; i < item->subItemCount; ++i)
        {
            printf("%s\r\n", item->subItems[i].name);
        }
    }
    else
    {
        // ���û���Ӳ˵�������ʾ��ǰ�˵���Ĺ���
        printf("ִ�й��� %s\r\n", item->name);
        if (item->function)
        {
            item->function(); // ִ�й���
        }
    }
}
void MenuEnter(MenuItem *item)
{
    if (item->subItems != NULL)
    {
        supItems = item;                                     // �ϼ��˵���ֵ����
        tCurrent_Menu = &item->subItems[tMenuCfg.CurrentID]; // ��ֵ����ǰ�˵�����ָ��
    }
    else
    {
        return; // �˳�
    }
}
void MenuExit(MenuItem *item)
{
    if (supItems != NULL)
    {
        tCurrent_Menu = supItems; // ��ֵ����ǰ�˵�����ָ��
    }
    else
    {
        return; // �˳�
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

    tMenuCfg.Key = -1; // ִ����,������λ,��ռ���ж���Դ
}

void displayMenu(MenuItem *item)
{
    printf("\n--- �˵� ---\n");
    if (item->subItems)
    {
        // �����ǰ�˵������Ӳ˵�������ʾ�Ӳ˵���
        for (int i = 0; i < item->subItemCount; ++i)
        {
            printf("%s\n", item->subItems[i].name);
        }
    }
    else
    {
        // ���û���Ӳ˵�������ʾ��ǰ�˵���Ĺ���
        printf("ִ�й��� %s\n", item->name);
        if (item->function)
        {
            item->function(); // ִ�й���
        }
        return; // �˳��ݹ�
    }

    char choice[20];
    printf("������ѡ�񣨻�����'back'������һ���˵���: ");
    scanf("%19s", choice); // �������볤�ȣ���ֹ���

    if (strcmp(choice, "back") == 0)
    {
        // �û�ѡ�񷵻���һ���˵�
        return;
    }

    // �����û�ѡ����Ӳ˵���
    for (int i = 0; i < item->subItemCount; ++i)
    {
        if (strcmp(item->subItems[i].name, choice) == 0)
        {
            // �ݹ���ʾѡ����Ӳ˵���
            displayMenu(&item->subItems[i]);
            break; // �ҵ����ټ�������
        }
    }
}
