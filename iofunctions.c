#include "iofunctions.h"

char *readFile(int * startsize, char const * file_name)
{
    FILE *fp;
    fp = fopen(file_name,"rt");
    if(fp==NULL)
    {
        return NULL;
    }

    char *strNew=(char*)malloc(((*startsize)+1)*sizeof(char));
    if(strNew==NULL)
    {
        printf("Nem sikerult memoriat foglalni");
        return NULL;
    }
    strNew[0]='\0';

    char temp [50];
    int count=0;
    while ((fscanf(fp, "%s\n", temp)) != EOF)
    {
        if(count += strlen(temp) > *startsize)
        {
            strNew=realloc(strNew,((count+1)*sizeof(char)));
            *startsize=count;
        }
        strcat(strNew,temp);
    }
    fclose(fp);
    return strNew;
}
char *readConsole(int * startsize)
{
    char *inpStr=(char*)malloc((1)*sizeof(char));
    if(inpStr==NULL)
    {
        printf("Nem sikerult memoriat foglalni");
        return NULL;
    }
    inpStr[0]='\0';
    char c;
    int count=0;
    while(c=getchar())
    {
        if(c=='\n')
            break;

        inpStr[count++]=c;
        inpStr=(char*)realloc(inpStr,(count+1)*sizeof(char));
    }
    inpStr[count]='\0';
    *startsize=count;
    return inpStr;
}
void makeFile(char * str, char const * file_name)
{
    FILE *fp;
    fp = fopen(file_name,"wt");
    if(fp==NULL)
    {
        perror("Nem sikerult megnyitni a fajlt, nem lett kimentve a nyitvatartas!");
        return NULL;
    }
    char cpyOfStr[strlen(str+1)];
    cpyOfStr[0]='\0';
    strcat(cpyOfStr,str);

    char *splitted = strtok(cpyOfStr, ";");
    while( splitted != NULL )
    {
        fprintf( fp,"%s;\n", splitted );
        splitted = strtok(NULL, ";");
    }
    fclose(fp);
}

