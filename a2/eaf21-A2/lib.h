//desenvolvido por Eduarda de Aguiar Freitas, GRR 20211799
#ifndef __LIB__
#define __LIB__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "io.h"
#include "lib_filter.h"

void options();

int type_verify(char * tok);

void type(csv *keeper, base *database);

char* put_spaces(size_t size, size_t diff, char *origin_string);

void fill_string(csv *keeper, base *database);

size_t find_column(csv *keeper, base *database_copy, char *filter);

#endif