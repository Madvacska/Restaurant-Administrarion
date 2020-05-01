#ifndef RESERV_H
#define RESERV_H
#include "timemanage.h"
#include "tables.h"
#include "debugmalloc.h"
#include "debugmalloc-impl.h"

typedef struct Reservation
{
    int table_number;
    char name[51];
    Time from_to;
    struct Reservation* next;
} Reservation;
Reservation * processReservation(char * base_str, Reservation * final_reserv_list,int const table_number, TableList * table_list);
Reservation* manageReservationMP(Reservation * first_element,TableList * first_table);
Reservation* removeResElement(Reservation* first, int const code);
Reservation* setIsAppropriate(Reservation * res_list,int const code,bool const value);
void freeUpReserv(Reservation* first);
#endif // RESERV_H
