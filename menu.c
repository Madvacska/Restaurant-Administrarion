#include "menu.h"

Menu * processMenu(char * base_str, Menu * final_menu_list)
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
        Menu * newElement=(Menu*)malloc(sizeof(Menu));
        if(newElement==NULL)
            return NULL;

        sscanf(splitted,"%d:%[^:]:%[^:]:%d",&newElement->code,newElement->type,newElement->name, &newElement->prize);
        newElement->next=NULL;
        if(final_menu_list==NULL)
            final_menu_list=newElement;
        else
        {
            Menu * finder = final_menu_list;
            while(finder->next!=NULL)
                finder=finder->next;
            finder->next=newElement;
        }
        splitted = strtok(NULL, ";");
    }
    return final_menu_list;
}
char * makeStrFromMenu(Menu* first_elemnt)
{
    Menu * finder = first_elemnt;
    int size=0;
    char temp [50]="";
    char * finalStr= (char*)malloc(sizeof(char));
    if(finalStr==NULL)
    {
        printf("Nem sikerult memoriat foglalni");
        return NULL;
    }
    finalStr[0]='\0';
    while(finder!=NULL)
    {
        sprintf(temp,"%d:%s:%s:%d;",finder->code,finder->type,finder->name,finder->prize);
        size +=strlen(temp);
        finalStr=realloc(finalStr,((size+1)*sizeof(char)));
        strcat(finalStr,temp);
        finder=finder->next;
    }
    return finalStr;
}

Menu* removeMenuElement(Menu* first, int const code)
{

    Menu *inchworm = NULL;
    Menu * finder = first;
    while (finder != NULL && finder->code != code)
    {
        inchworm = finder;
        finder = finder->next;
    }
    if (inchworm == NULL)
    {
        Menu *newFirst = finder->next;
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
Menu* isExistMenu(Menu * first_element, int const menu_number)
{
    Menu * finder = first_element;
    bool yes_or_no = false;
    while(finder!=NULL && !yes_or_no)
    {
        if(finder->code==menu_number)
            yes_or_no=true;
        else
            finder=finder->next;
    }
    return finder;
}
Menu* manageMenuMP(Menu * first_element)
{
    int mp;
    do
    {
        printf("Kerem valasszon a kovetkezo menupontok kozul!\n");
        printf("1=Menu listazasa\n2=Menu hozzaadasa\n3=Menu torlese\n0=Kilepes\n");
        scanf("%d",&mp);
        getchar();
        system("cls");
        switch(mp)
        {
        case 1:
            printf("Menu listazasa");
            Menu * finder = first_element;
            while(finder!=NULL)
            {
                printf("\nA menuelem szama: %d\nTipusa: %s\nNeve: %s\nAra: %d\n",finder->code,finder->type,finder->name,finder->prize);
                finder=finder->next;
            }
            break;
        case 2:
            printf("Menu hozzaadasa-egyszerre tobb\n");
            printf("Kerem adja meg a menu adatait a kovetkezo formatumban: kod:tipus:nev:ar;\nVagy modositsa/hozza letre a files/menu.txt fajlt és inditsa ujra a programot!\n");
            int size=50;
            char * newMenuStr= readConsole(&size);
            first_element=processMenu(newMenuStr,first_element);
            free(newMenuStr);
            newMenuStr=makeStrFromMenu(first_element);
            makeFile(newMenuStr,"files/menu.txt");
            free(newMenuStr);
            break;
        case 3:
            printf("Menu torlese-egyesevel\n");
            printf("Kerem adja meg annak a menunek a kodjat, amit torolni szeretne!\n");
            int menuCode;
            scanf("%d",&menuCode);
            getchar();
            if(isExistMenu(first_element,menuCode))
            {
                first_element=removeMenuElement(first_element,menuCode);
                newMenuStr=makeStrFromMenu(first_element);
                makeFile(newMenuStr,"files/menu.txt");
                free(newMenuStr);
            }
            else
                printf("Nincs ilyen menu elem");

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
void freeUpMenu(Menu* first)
{
    Menu* finder = first;
    while (finder != NULL)
    {
        Menu *nxt = finder->next;
        free(finder);
        finder = nxt;
    }
}
