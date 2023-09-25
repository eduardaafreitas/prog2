#ifndef __ARFF__
#define __ARFF__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  char *rotulo;
  char *tipo;
  char **categorias;
} atributo;

char *nome_arq;

void exibe_atributos(atributo *infos, int quantidade);
int conta_atributos(FILE *arff);
void processa_categorias(atributo *elemento, char *categorias);
atributo* processa_atributos(FILE *arff, int quantidade);
void valida_arff(FILE *arff, atributo *atributos, int quantidade);

#endif