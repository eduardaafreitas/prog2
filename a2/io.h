#ifndef __IO__
#define __IO__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct arq_csv arq_csv;

struct arq_csv {
    FILE *arquivo;
    unsigned long row;
    unsigned long column;
    char *type;
    long pos;
    short *sizes;
};

void options();

int conta_colunas(FILE *arquivo);

int conta_linhas(FILE *arquivo);

arq_csv* processa_arquivo(FILE *arquivo, int qtd_linhas, int qtd_colunas);

int interpreta_dados(char * tok);

void sumario(arq_csv *keeper_csv, int qtd_linhas, int qtd_colunas);

void mostrar(arq_csv *keeper_csv, int qtd_linhas, int qtd_colunas);

void libera(arq_csv *keeper_csv, int qtd_linhas, int qtd_colunas);


#endif