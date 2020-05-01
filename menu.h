#ifndef MENU_H
#define MENU_H
#include "iofunctions.h"
#include "debugmalloc.h"
#include "debugmalloc-impl.h"
#include <stdbool.h>
typedef struct Menu
{
    int code; //keresest segiti
    char type [51]; //max 50 karakter hosszu
    char name [51]; //max 50 karakter hosszu
    int prize;
    struct Menu* next;
} Menu;
Menu * processMenu(char * base_str, Menu * final_menu_list);
char * makeStrFromMenu(Menu* first_elemnt);
Menu* removeMenuElement(Menu* first, int const code);
Menu* isExistMenu(Menu * first_element, int const menu_number);
Menu* manageMenuMP(Menu * first_element);
void freeUpMenu(Menu* first);
#endif // MENU_H
