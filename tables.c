#include "tables.h"
TableList * processTables(char * base_str, TableList * final_table_list)
{
    int n =0;
    for(int i=0; base_str[i]!='\0'; ++i)
        if(base_str[i] == ';')
            n++;
    if(n==0)
        return NULL;
    char * splitted = strtok(base_str,";");
    for(int i =0; i<n; ++i)
    {
        TableList * newElement=(TableList *)malloc(sizeof(TableList));
        if(newElement==NULL)
            return NULL;
        int temp;
        sscanf(splitted,"%d:%d:%d",&newElement->number,&newElement->for_howmany,&temp);
        newElement->is_free=temp;
        newElement->next=NULL;
        if(final_table_list==NULL)
            final_table_list=newElement;
        else
        {
            TableList * finder = final_table_list;
            while(finder->next!=NULL)
                finder=finder->next;
            finder->next=newElement;
        }
        splitted = strtok(NULL, ";");
    }
    return final_table_list;
}
char * makeStrFromTables(TableList* first_elemnt)
{
    TableList * finder = first_elemnt;
    int size=0;
    char temp [12]="";
    char * finalStr= (char*)malloc(sizeof(char));
    if(finalStr==NULL)
    {
        printf("Nem sikerult memoriat foglalni");
        return NULL;
    }
    finalStr[0]='\0';
    while(finder!=NULL)
    {
        sprintf(temp,"%d:%d:%d;",finder->number,finder->for_howmany,finder->is_free);
        size +=strlen(temp);
        finalStr=realloc(finalStr,((size+1)*sizeof(char)));
        strcat(finalStr,temp);
        finder=finder->next;
    }
    return finalStr;
}
TableList* removeTableElement(TableList* first, int const code)
{

    TableList *inchworm = NULL;
    TableList * finder = first;
    while (finder != NULL && finder->number != code)
    {
        inchworm = finder;
        finder = finder->next;
    }
    if (inchworm == NULL)
    {
        TableList *newFirst = finder->next;
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
bool isExist(TableList * first_element, int const table_number)
{
    TableList * finder = first_element;
    bool ready = false;
    while(finder!=NULL && !ready)
    {
        if(finder->number==table_number)
            ready=true;
        else
            finder=finder->next;
    }
    return (finder!=NULL);
}
TableList * findTableByCode(TableList* table_list, int const code)
{
    TableList* finder = table_list;
    bool ready=false;
    while(finder!=NULL && !ready)
    {
        if(finder->number==code)
            ready=true;
        else
            finder=finder->next;
    }
    return finder;
}
TableList* manageTablesMP(TableList * first_element)
{
    int mp;
    do
    {
        printf("Kerem valasszon a kovetkezo menupontok kozul!\n");
        printf("1=Asztalok listazasa\n2=Asztal hozzaadasa\n3=Asztal torlese\n0=Kilepes\n");
        scanf("%d",&mp);
        getchar();
        system("cls");
        switch(mp)
        {
        case 1:
            printf("Asztalok listazasa");
            TableList * finder = first_element;
            while(finder!=NULL)
            {
                printf("\nAsztal szama: %d\nHelyek szama: %d\nSzabad?: %d\n",finder->number,finder->for_howmany,finder->is_free);
                finder=finder->next;
            }
            break;
        case 2:
            printf("Asztal hozzaadasa-egyszerre tobb\n");
            printf("Kerem adja meg az asztal adatait a kovetkezo formatumban: sorszam:fo:foglat-e;\n(szabad=1, foglat=0)\nVagy modositsa/hozza letre a files/tables.txt fajlt es inditsa ujra a programot!\n");
            int size=50;
            char * newTableStr= readConsole(&size);
            first_element=processTables(newTableStr,first_element);
            free(newTableStr);
            newTableStr=makeStrFromTables(first_element);
            makeFile(newTableStr,"files/tables.txt");
            free(newTableStr);
            break;
        case 3:
            printf("Asztal torlese-egyesevel\n");
            printf("Kerem adja meg annak az asztalnak a szamat, amit torolni szeretne!\n");
            int tableCode;
            scanf("%d",&tableCode);
            getchar();
            if(isExist(first_element,tableCode))
            {
                first_element=removeTableElement(first_element,tableCode);
                newTableStr=makeStrFromTables(first_element);
                makeFile(newTableStr,"files/tables.txt");
                free(newTableStr);
            }
            else
                printf("Nincs ilyen asztal");
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
TableList * setIsFree(TableList* first,int const wich_table,bool const value)
{
    TableList * finder = first;
    bool ready =false;
    while(!ready&&finder!=NULL)
    {
        if(finder->number==wich_table&&finder->is_free!=value)
        {
            finder->is_free=value;
            ready=true;
        }
        finder=finder->next;
    }
    if(finder==NULL)
        printf("Sikertelen muvelet!\n");
    else
        printf("Sikeres muvelet\n");
    return first;
}
TableList * setReservation (TableList * first)
{
    printf("\nA jelenleg szabad asztalok a kovetkezok: ");
    printf("\nAsztal szama\tHelyek szama\tSzabad?\n");
    TableList * finder = first;
    while(finder!=NULL)
    {
        if(finder->is_free==true)
            printf("%d\t\t%d\t\t%d\n",finder->number,finder->for_howmany,finder->is_free);
        finder=finder->next;
    }
    int wichTable =0;
    printf("Melyik asztalt szeretne lefoglalni?\n");
    scanf("%d",&wichTable);
    getchar();
    first = setIsFree(first,wichTable,0);
    char * newTableStr=makeStrFromTables(first);
    makeFile(newTableStr,"files/tables.txt");
    free(newTableStr);
    return first;
}
void freeUpTables(TableList* first)
{
    TableList* finder = first;
    while (finder != NULL)
    {
        TableList *nxt = finder->next;
        free(finder);
        finder = nxt;
    }
}
