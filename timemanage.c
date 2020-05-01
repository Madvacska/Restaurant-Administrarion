#include "timemanage.h"

void processOH (char * base_str, Time *final_OH, int const n)
{

    char * splitted = strtok(base_str,";");
    for(int i =0; i<n; ++i)
    {
        final_OH[i].day=i;
        final_OH[i].is_approp=true;
        if(strcmp(splitted,"ZARVA")==0)
            final_OH[i].is_approp=false;
        else
            sscanf(splitted,"%d:%d-%d:%d",&final_OH[i].hour_from,&final_OH[i].min_from,&final_OH[i].hour_to,&final_OH[i].min_to);
        splitted = strtok(NULL, ";");
    }
}
bool isOpenNow()
{
    int size=100;
    char *strOpnHrs=readFile(&size,"files/openinghours.txt");
    if(strOpnHrs==NULL)
    {
        printf("Nem sikerult megnyitni a fajlt! Lehetseges, hogy nincs megadva nyitvatartas!\nKerem adja meg, hogyan visz fel nyitvatartast: 0=file, 1=kezzel\n");
        int valasz =0;
        scanf("%d",&valasz);
        getchar();
        switch(valasz)
        {
        case 0:
            printf("Kerem hozza letre megfelelo formatumban a files/openinghours.txt-t\nMajd iron be barmely karaktert es usson entert!\n");
            char a;
            scanf("%c\n",&a);
            getchar();
            strOpnHrs=readFile(&size,"files/openinghours.txt");
            break;
        case 1:
            printf("Kerem adja meg a megfelelo adatokat! Figyeljen, hogy vasarnappal kezdjen!\npl1:10:30-17:00;\npl2:ZARVA;\n");
            strOpnHrs=readConsole(&size);
            makeFile(strOpnHrs,"files/openinghours.txt");
            break;
        default:
            printf("Nincs ilyen valasz!\nInditsa ujra a programot");
            break;
        }
    }
    int n=7;
    Time readyOH [n];
    processOH(strOpnHrs,readyOH, n);
    free(strOpnHrs);

    time_t rawtime = time(NULL);
    struct tm *now = localtime(&rawtime);

    for(int i=0; i<7; i++)
    {
        if(now->tm_wday==readyOH[i].day)
        {
            if(readyOH[i].is_approp == false)
                return false;
            if(readyOH[i].hour_from < now->tm_hour && readyOH[i].hour_to > now->tm_hour)
            {
                return true;
            }
            if(readyOH[i].hour_from == now->tm_hour)
            {
                if(readyOH[i].min_from <= now->tm_min)
                    return true;
            }
            if(readyOH[i].hour_to == now->tm_hour)
            {
                if(readyOH[i].min_to >= now->tm_min)
                    return true;
            }
            return false;
        }
    }
    return false;
}
