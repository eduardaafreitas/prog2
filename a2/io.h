#ifndef __IO__
#define __IO__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct csv csv;

struct csv {
    FILE *arquivo;
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

unsigned long count_columns(FILE *arquivo);

unsigned long count_rows(FILE *arquivo);

csv* alloc_csv();

base *alloc_database();

void armazenar(FILE *arquivo, csv *keeper, base *database, unsigned long row, unsigned long column);

//int interpreta_dados(char * tok);

void sumario(csv *keeper_csv, int qtd_linhas, int qtd_colunas);

//void mostrar(csv *keeper_csv, int qtd_linhas, int qtd_colunas);

void free_csv(csv *keeper_csv);


#endif