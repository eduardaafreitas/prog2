#ifndef __IO__
#define __IO__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void options();

int conta_colunas(FILE *arquivo);

int conta_linhas(FILE *arquivo);

char*** processa_arquivo(FILE *arquivo, int qtd_linhas, int qtd_colunas);

int interpreta_dados(char * tok);

void sumario(char ***dados, int qtd_linhas, int qtd_colunas);

void mostrar(char ***dados, int qtd_linhas, int qtd_colunas);

void libera(char ***dados, int qtd_linhas, int qtd_colunas);


#endif