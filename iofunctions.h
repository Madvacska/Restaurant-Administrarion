#ifndef IOFUNCTIONS_H
#define IOFUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debugmalloc.h"
#include "debugmalloc-impl.h"

char *readFile(int * startsize, char const * file_name);
char *readConsole(int * startsize);
void makeFile(char * str, char const * file_name);

#endif // IOFUNCTIONS_H


