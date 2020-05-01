#include "iofunctions.h"
#include "timemanage.h"
#include "debugmalloc.h"
#include "debugmalloc-impl.h"
#include "tables.h"
#include "menu.h"
#include "orders.h"
#include "reserv.h"
void printCheck(Orders* orders_list, int table_number, Menu* menu_list)
{
    Orders* orderToPay= findOrderByTable(orders_list, table_number);
    if(orderToPay!=NULL)
    {
        printOrderByName(orderToPay,menu_list);
        int sum=0;
        for(int i =0; i< orderToPay->n; ++i)
        {
            Menu* wichElement = isExistMenu(menu_list,orderToPay->meals[i]);
            if(wichElement!=NULL)
                sum+=wichElement->prize;
        }
        printf("\nOsszesen: %d\n",sum);
        removeOrdersElement(orders_list,table_number);
    }
    else
        printf("\nA megadott asztalhoz nem tartozik rendeles\n");
}
int main(void)
{
    bool ion= isOpenNow();
    if(ion==true)
    {
        int size =200;
        char * strMenu = readFile(&size,"files/menu.txt");
        char * strTables = readFile(&size,"files/tables.txt");
        char * strReservation = readFile(&size,"files/reservation.txt");

        TableList * tFirst=NULL;
        Menu * mFirst=NULL;
        Orders * oFirst=NULL;
        Reservation * rFirst=NULL;
        if(strMenu==NULL)
            printf("Nincs megadva helyes menu. A program menu nelkul lehetseges, hogy nem mukodik helyesen.\nKerem vigyen fel menut a megfelelo (2.) funkcio kivalasztasaval!\n");
        else
            mFirst=processMenu(strMenu,mFirst);
        if(strTables==NULL)
            printf("Nincsenek megadva helyes asztalok. A program enelkul lehetseges, hogy nem mukodik helyesen.\nKerem vigyen fel asztalokat a megfelelo (1.) funkcio kivalasztasaval!\n");
        else
            tFirst= processTables(strTables,tFirst);
        if(strReservation!=NULL)
            rFirst=processReservation(strReservation,rFirst,0,tFirst);
        int mPnt;
        system("cls");
        do
        {
            printf("\n1=Asztalok kezelese\n2=Menu/menuelem kezelese\n3=Uj asztal nyitasa\n4=Rendeles felvetele\n5=Szamlazas\n6=Uj elorefoglalas rogzitese\n0=Kilepes\n");
            scanf("%d",&mPnt);
            getchar();
            system("cls");
            switch(mPnt)
            {
            case 1:
                printf("Asztalok kezelese\n");
                tFirst=manageTablesMP(tFirst);
                break;
            case 2:
                printf("Menu/menuelem kezelese\n");
                mFirst=manageMenuMP(mFirst);
                break;
            case 3:
                printf("Uj asztal nyitasa\n");
                tFirst = setReservation(tFirst);
                break;
            case 4:
                printf("Rendeles felvetele\n");
                oFirst=manageOrdersMP(oFirst,tFirst,mFirst);
                break;
            case 5:
                printf("Szamlazas\nAdja meg melyik asztal szeretne fizetni:\n");
                int wichTable;
                scanf("%d",&wichTable);
                if(!isExist(tFirst,wichTable))
                    printf("Nincs ilyen asztal\n");
                else
                {
                    printCheck(oFirst,wichTable,mFirst);
                    setIsFree(tFirst,wichTable,1);
                }
                break;
            case 6:
                printf("Uj elorefoglalas rogzitese\n");
                rFirst= manageReservationMP(rFirst,tFirst);
                break;
            case 0:
                break;
            default:
                printf("Nincs ilyen menupont!");
                break;
            }

        }
        while(mPnt!=0);

        freeUpReserv(rFirst);
        free(strMenu);
        free(strReservation);
        free(strTables);
        freeUpTables(tFirst);
        freeUpMenu(mFirst);
        freeUpOrders(oFirst);

    }
    else
        printf("Az etterem program csak nyitvatartasi idoben hasznalhato!");
    printf("\nViszont latasra!");
    getchar(); //kulsõ futtatás esetén szükséges, hogy ne záródjon be az ablak
    return 0;
}
