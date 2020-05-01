#include "orders.h"
//a rendelest nem mentjuk fileba- nem modosithato csak torolheto es hozzadhato
Orders * findOrderByTable(Orders* ordr_list, int const table_number)
{
    Orders * finder = ordr_list;
    bool ready = false;
    while(finder!=NULL && !ready)
    {
        if(finder->table_number==table_number)
            ready=true;
        else
            finder=finder->next;
    }
    return finder;
}
Orders * addOrder(char * base_str, Orders *current_element)
{
    int n =0;
    for(int i=0; base_str[i]!='\0'; ++i)
        if(base_str[i] == ';')
            n++;
    if(n!=0)
    {
        char * splitted = strtok(base_str,";");
        current_element->meals = realloc(current_element->meals,(n+current_element->n)*sizeof(int));
        n+=current_element->n;
        for(int i =current_element->n; i<n; ++i)
        {
            int temp;
            sscanf(splitted,"%d",&temp);
            current_element->meals[i]=temp;
            splitted = strtok(NULL, ";");
        }
        current_element->n =n;

    }

    return current_element;
}
Orders * processOrder(char * base_str, Orders * order_list, int const table_number)
{
    Orders * newElement=(Orders *)malloc(sizeof(Orders));
    if(newElement==NULL)
        return NULL;
    newElement->table_number=table_number;
    newElement->next=NULL;
    int n =0;
    for(int i=0; base_str[i]!='\0'; ++i)
        if(base_str[i] == ';')
            n++;
    if(n==0)
        return NULL;
    char * splitted = strtok(base_str,";");
    int * mealsData= malloc(n*sizeof(int));
    for(int i =0; i<n; ++i)
    {
        int temp;
        sscanf(splitted,"%d",&temp);
        mealsData[i]=temp;
        splitted = strtok(NULL, ";");
    }
    newElement->meals=mealsData;
    newElement->n=n;
    if(order_list==NULL)
        order_list=newElement;
    else
    {
        Orders * finder = order_list;
        while(finder->next!=NULL)
            finder=finder->next;
        finder->next=newElement;
    }
    return order_list;
}
void printOrderByName(Orders* current_element, Menu* menu_list)
{
    for(int i =0; i< current_element->n; ++i)
    {
        printf("\nMenu elem kodja: %d\n",current_element->meals[i]);
        Menu* wichElement = isExistMenu(menu_list,current_element->meals[i]);
        if(wichElement==NULL)
            printf("Nincs ilyen elem\n");
        else
            printf("Tipusa: %s\nNeve: %s\nAra: %d\n",wichElement->type,wichElement->name,wichElement->prize);
    }

}
Orders* removeOrdersElement(Orders* first, int const code)
{

    Orders *inchworm = NULL;
    Orders * finder = first;
    while (finder != NULL && finder->table_number != code)
    {
        inchworm = finder;
        finder = finder->next;
    }
    if (inchworm == NULL)
    {
        free(finder->meals);
        Orders *newFirst = finder->next;
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

Orders * manageOrdersMP(Orders * o_first,TableList* t_first, Menu* m_first)
{
    printf("\nKerem adja meg, hogy melyik asztal rendelesevel dolgozna!\n");
    int wichTable=0;
    scanf("%d", &wichTable);
    getchar();
    if(!isExist(t_first,wichTable))
    {
        printf("Nincs ilyen asztal!\n");
    }
    else
    {
        if(findTableByCode(t_first,wichTable)->is_free==false)
        {
            int mp;
            do
            {
                Orders * currentElement= findOrderByTable(o_first,wichTable);
                printf("\nKerem valasszon a kovetkezo menupontok kozul!\n");
                printf("1=Rendeles kiiratasa\n2=Rendeles felvetele\n3=Rendeles torlese\n0=Kilepes\n");
                scanf("%d",&mp);
                getchar();
                system("cls");
                switch(mp)
                {
                case 1:
                    printf("Rendeles a(z) %d szamu asztalhoz \n",wichTable);
                    if(currentElement==NULL)
                        printf("\nNincs felvett rendeles ehhez az asztalhoz!\n");
                    else
                        printOrderByName(currentElement,m_first);
                    break;
                case 2:
                    printf("Rendeles felvetele-egyszerre egy asztalhoz\n");
                    printf("\nKerem adja meg az etelek kodjat, minden kod utan tegyen ;-t!\n");
                    int length=1;
                    char * ordersStr=readConsole(&length);
                    if(currentElement==NULL)
                        o_first=processOrder(ordersStr,o_first,wichTable);
                    else
                        currentElement=addOrder(ordersStr,currentElement);
                    printf("Sikeres uj rendeles!");
                    free(ordersStr);
                    break;
                case 3:
                    printf("Rendeles torlese-egyszerre egy asztaltol\n");
                    o_first=removeOrdersElement(o_first,wichTable);
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
        }
        else
            printf("\nItt nem ul senki");
    }


    return o_first;
}
void freeUpOrders(Orders* first)
{
    Orders* finder = first;
    while (finder != NULL)
    {
        if(finder->meals!=NULL)
            free(finder->meals);
        Orders *nxt = finder->next;
        free(finder);
        finder = nxt;
    }
}
