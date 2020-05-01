#include "reserv.h"
Reservation * processReservation(char * base_str, Reservation * final_reserv_list,int const table_number, TableList * table_list)
{
    int n =0;
    for(int i=0; base_str[i]!='\0'; ++i)
        if(base_str[i] == ';')
            n++;
    if(n==0)
        return NULL;

    char * splitted = strtok(base_str,";");
    int cnt=0;
    for(int i=0; splitted[i]!='\0'; ++i)
        if(splitted[i] == ':')
            cnt++;

    for(int i =0; i<n; ++i)
    {
        Reservation * newElement=(Reservation *)malloc(sizeof(Reservation));
        if(newElement==NULL)
            return NULL;
        newElement->next=NULL;
        if(cnt==5)
        {
            newElement->table_number=table_number;
            newElement->from_to.is_approp=false;
            sscanf(splitted,"%[^:]:%d:%d:%d:%d:%d",newElement->name,&newElement->from_to.day,&newElement->from_to.hour_from,&newElement->from_to.min_from,&newElement->from_to.hour_to,&newElement->from_to.min_to);
        }
        else
        {
            int temp;
            sscanf(splitted,"%d:%[^:]:%d:%d:%d:%d:%d:%d",&newElement->table_number,newElement->name,&newElement->from_to.day,&temp,&newElement->from_to.hour_from,&newElement->from_to.min_from,&newElement->from_to.hour_to,&newElement->from_to.min_to);
            newElement->from_to.is_approp=temp;
        }

        TableList * currentTable= findTableByCode(table_list,newElement->table_number);
        if(currentTable!=NULL && currentTable->is_free)
        {
            currentTable=setIsFree(table_list,newElement->table_number,0);
            if(final_reserv_list==NULL)
                final_reserv_list=newElement;
            else
            {
                Reservation * finder = final_reserv_list;
                while(finder->next!=NULL)
                    finder=finder->next;
                finder->next=newElement;
            }
        }
        else
            free(newElement);

        splitted = strtok(NULL, ";");
    }
    char * newTableStr=makeStrFromTables(table_list);
    makeFile(newTableStr,"files/tables.txt");
    free(newTableStr);
    return final_reserv_list;
}


Reservation* removeResElement(Reservation* first, int const code)
{
    Reservation *inchworm = NULL;
    Reservation * finder = first;
    while (finder != NULL && finder->table_number != code)
    {
        inchworm = finder;
        finder = finder->next;
    }
    if (inchworm == NULL)
    {
        Reservation *newFirst = finder->next;
        free(finder);
        first = newFirst;
    }
    else if(inchworm!=NULL)
    {
        inchworm->next = finder->next;
        free(finder);
    }

    return first;
}
Reservation* setIsAppropriate(Reservation * res_list,int const code,bool const value)
{
    Reservation * finder = res_list;
    bool ready =false;
    while(!ready&&finder!=NULL)
    {
        if(finder->table_number==code&&finder->from_to.is_approp!=value)
        {
            finder->from_to.is_approp=value;
            ready=true;
        }
        finder=finder->next;
    }
    return res_list;
}
char* makeStrFromReservation(Reservation* first_element)
{
    Reservation * finder = first_element;
    int size=0;
    char temp [30]="";
    char * finalStr= (char*)malloc(sizeof(char));
    if(finalStr==NULL)
    {
        printf("Nem sikerult memoriat foglalni");
        return NULL;
    }
    finalStr[0]='\0';
    while(finder!=NULL)
    {
        sprintf(temp,"%d:%s:%d:%d:%d:%d:%d:%d;",finder->table_number,finder->name,finder->from_to.day,finder->from_to.is_approp,finder->from_to.hour_from,finder->from_to.min_from,finder->from_to.hour_to,finder->from_to.min_to);
        size +=strlen(temp);
        finalStr=realloc(finalStr,(size+1*sizeof(char)));
        strcat(finalStr,temp);
        finder=finder->next;
    }
    return finalStr;
}
Reservation* manageReservationMP(Reservation * first_element,TableList * first_table)
{

    int mp;
    do
    {
        printf("\nKerem valasszon a kovetkezo menupontok kozul!\n");
        printf("1=Uj elorefoglalas rogzitese\n2=Letezo foglalas bevaltasa\n3=Letezo foglalas torlese\n4=Foglalasok listazasa\n0=Kilepes\n");
        scanf("%d",&mp);
        getchar();
        system("cls");
        switch(mp)
        {
        case 1:
            printf("Uj elorefoglalas rogzitese\n");
            printf("\nKerem adja meg, hogy melyik asztalt szeretne lefoglalni\n");
            int wichTable=0;
            scanf("%d", &wichTable);
            getchar();
            if(!isExist(first_table,wichTable)||!(findTableByCode(first_table,wichTable)->is_free))
            {
                printf("Az asztal mar foglalt vagy nem letezik!\n");
            }
            else
            {
                printf("Kerem adja meg a foglalas adatait a kovetkezo formatumban: felhasznalonev(spacek nelkuli):nap(0-6 azaz V-Szo):kezdoora:kezdoperc:vegora:vegperc;\nVagy modositsa/hozza letre a files/reservation.txt fajlt es inditsa ujra a programot!\n");
                int size=50;
                char * newReservStr= readConsole(&size);
                first_element=processReservation(newReservStr,first_element,wichTable,first_table);
                free(newReservStr);
                newReservStr=makeStrFromReservation(first_element);
                makeFile(newReservStr,"files/reservation.txt");
                free(newReservStr);
            }
            break;
        case 2:
            printf("Letezo foglalas bevaltasa\n");
            printf("\nKerem adja meg, hogy melyik asztal foglalasat szeretne bevaltani\nHa nem letezo foglalast ad meg az adatok nem valtoznak\n");
            wichTable=0;
            scanf("%d", &wichTable);
            getchar();
            if(isExist(first_table,wichTable)&&first_element!=NULL)
            {
                setIsAppropriate(first_element,wichTable,1);
                char * newReservStr=makeStrFromReservation(first_element);
                makeFile(newReservStr,"files/reservation.txt");
                free(newReservStr);
            }
            break;
        case 3:
            printf("Letezo foglalas torlese\n");
            printf("\nKerem adja meg, hogy melyik asztal foglalasat szeretne torolni\nHa nem letezo foglalast ad meg az adatok nem valtoznak\n");
            wichTable=0;
            scanf("%d", &wichTable);
            getchar();
            if(isExist(first_table,wichTable)&&first_element!=NULL)
            {
                first_table=setIsFree(first_table,wichTable,1);
                first_element=removeResElement(first_element,wichTable);
                char * newTableStr=makeStrFromTables(first_table);
                makeFile(newTableStr,"files/tables.txt");
                char * newReservStr=makeStrFromReservation(first_element);
                makeFile(newReservStr,"files/reservation.txt");
                free(newReservStr);
                free(newTableStr);
            }
            break;
        case 4:
            printf("Foglalasok listazasa-jol meadott asztalok eseten\n");
            Reservation * finder = first_element;
            while(finder!=NULL)
            {
                if(isExist(first_table,finder->table_number))
                    printf("\nAsztal: %d\nNev: %s\nNap: %d\nItt van?: %d\nTol: %d:%d\nIg: %d:%d\n",finder->table_number,finder->name,finder->from_to.day,finder->from_to.is_approp,finder->from_to.hour_from,finder->from_to.min_from,finder->from_to.hour_to,finder->from_to.min_to);
                finder=finder->next;
            }
            break;
        case 0:
            break;
        default:
            printf("Nincs ilyen menüpont!");
            break;
        }
        printf("\n");
    }

    while(mp!=0);
    return first_element;
}
void freeUpReserv(Reservation* first)
{
    Reservation* finder = first;
    while (finder != NULL)
    {
        Reservation *nxt = finder->next;
        free(finder);
        finder = nxt;
    }
}
