#ifndef TIMEMANAGE_H
#define TIMEMANAGE_H

#include <stdbool.h>
#include <time.h>
#include "iofunctions.h"
#include "debugmalloc.h"
#include "debugmalloc-impl.h"

typedef struct Time
{
    int day; //0-6 = V-SZo
    bool is_approp; //nyitvatartásnál hogy nyitva van-e, foglalásnál hogy itt van-e már
    int hour_from;
    int min_from;
    int hour_to;
    int min_to;
} Time;
void processOH (char * base_str, Time *final_OH, int const n);
bool isOpenNow();
#endif // TIMEMANAGE_H
