#ifndef __LIB__
#define __LIB__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "io.h"

void options();


int type_verify(char * tok);
void type(csv *keeper, base *database, unsigned long column);
char* put_spaces(size_t size, size_t diff, char *origin_string);
void fill_string(csv *keeper, base *database, unsigned long row, unsigned long column);



#endif