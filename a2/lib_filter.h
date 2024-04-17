//desenvolvido por Eduarda de Aguiar Freitas, GRR 20211799
#ifndef __LIB_FILTER__
#define __LIB_FILTER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "io.h"
#include "lib.h"

void database_filtered(csv *keeper, base *database, size_t *index, char *file_name);
void show_filter(csv *keeper, base *database_copy, size_t *index);

void bigger_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index);
void filter_bigger(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index);

void smaller_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index);
void filter_smaller(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index);

void lessorequal_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index);
void filter_lessorequal(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index);

void biggerorequal_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index);
void filter_biggerorequal(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index);

void equal_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index);
void equal_string(csv *keeper, base *database_copy, size_t column, char *value, size_t *index);
void filter_equal(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index);

void different_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index);
void different_string(csv *keeper, base *database_copy, size_t column, char *value, size_t *index);
void filter_different(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index);

#endif