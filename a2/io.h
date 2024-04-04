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
    short *sizes;
};

typedef struct base base;

struct base {
    unsigned long row;
    unsigned long column;
    char ***data;
};

void options();

unsigned long count_columns(FILE *archive);

unsigned long count_rows(FILE *archive);

csv* alloc_csv();

base *alloc_database();

void layin_csv(FILE *archive, csv *keeper, base *database, unsigned long row, unsigned long column);

void count_stringsize(csv *keeper, base *database, unsigned long row, unsigned long column);

int type_verify(char * tok);

void sumario(csv *keeper, base *database, int column);

void mostrar(csv *keeper, base *database, int row, int column);

void free_csv(csv *keeper_csv);
void free_database(base *database);


#endif