#ifndef LIST_H
#define LIST_H 
typedef struct MenuItem
{
    const char *name;
    struct MenuItem *supItems; // 上级
    struct MenuItem *subItems; // 下级
    int subItemCount;
    void (*function)(); // 指向功能的函数指针，可以为NULL
} MenuItem;



extern MenuItem *tCurrent_Menu; // 初始化为NULL，表示没有当前菜单 
extern MenuItem *supItems; // 上级
void freeSubMenus(MenuItem *item);
// 在适当的时候调用这个函数来释放内存，比如在main循环结束前
void cleanup(void);
extern MenuItem menuItems[];
void initSubMenus(void);
void showMenu(MenuItem *item);
void MenuEnter(MenuItem *item);
void MenuExit(MenuItem *item);
void MenuKey_List(int8_t key);
#endif
