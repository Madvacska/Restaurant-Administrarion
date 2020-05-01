#ifndef ORDERS_H
#define ORDERS_H
#include "tables.h"
#include "menu.h"
#include "debugmalloc.h"
#include "debugmalloc-impl.h"
typedef struct Orders
{
    int * meals;
    int n;
    int table_number;
    struct Orders * next;
} Orders;
Orders * findOrderByTable(Orders* ordr_list, int const table_number);
Orders * processOrder(char * base_str, Orders * order_list, int const table_number);
Orders * addOrder(char * base_str, Orders *current_element);
Orders* removeOrdersElement(Orders* first, int const code);
void printOrderByName(Orders* current_element, Menu* menu_list);
Orders * manageOrdersMP(Orders * o_first,TableList* t_first, Menu* m_first);
void freeUpOrders(Orders* first);
#endif // ORDERS_H
