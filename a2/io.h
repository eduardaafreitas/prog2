#ifndef __IO__
#define __IO__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct csv csv;
struct csv {
    FILE *archive;
    unsigned long row;
    unsigned long column;
    char *type;
    long pos;
    size_t *sizes;
};

typedef struct base base;
struct base {
    unsigned long row;
    unsigned long column;
    char ***data;
};


unsigned long count_columns(FILE *archive);

unsigned long count_rows(FILE *archive);

csv* alloc_csv();

base *alloc_database();

void count_stringsize(FILE *archive, csv *keeper, unsigned long row, unsigned long column);

void layin_csv(FILE *archive, csv *keeper, base *database, unsigned long row, unsigned long column);

void sumario(csv *keeper, base *database, unsigned long column);

void mostrar(csv *keeper, base *database, unsigned long row, unsigned long column);

void filtrar(csv *keeper, base *database_copy, unsigned long row, unsigned long column);

void free_csv(csv *keeper_csv);
void free_database(base *database);


#endif