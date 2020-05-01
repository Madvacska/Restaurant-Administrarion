#ifndef TABLES_H
#define TABLES_H
#include "iofunctions.h"
#include "debugmalloc.h"
#include "debugmalloc-impl.h"
#include <stdbool.h>
typedef struct TableList
{
    int number;
    int for_howmany;
    bool is_free;
    struct TableList* next;
} TableList;
TableList * processTables(char * base_str, TableList * final_table_list);
char * makeStrFromTables(TableList* first_elemnt);
TableList* removeTableElement(TableList* first, int const code);
TableList* manageTablesMP(TableList * first_element);
TableList * setIsFree(TableList* first,int const witch_table,bool const value);
TableList * setReservation (TableList * first);
bool isExist(TableList * first_element, int const table_number);
TableList * findTableByCode(TableList* table_list, int const code);
void freeUpTables(TableList* first);
#endif // TABLES_H
