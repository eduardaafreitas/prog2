//desenvolvido por Eduarda de Aguiar Freitas, GRR 20211799
#ifndef __LIB_DESCRIPTION__
#define __LIB_DESCRIPTION__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "io.h"
#include "lib_filter.h"

float media_describe(base *database, csv *keeper, base *database_copy, size_t column);
float desvio_padrao(base *database, csv *keeper, base *database_copy, size_t column);
char moda(base *database, csv *keeper, base *database_copy, size_t column);

#endif