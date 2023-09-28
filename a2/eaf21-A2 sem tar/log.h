#ifndef __LOG__
#define __LOG__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arff.h"

extern int ataque;
void reposiciona_ponteiro(FILE *arff, char *linha);
void cria_relatorio_ataque(char *nome_ataque, int numero_ocorrencias);
void filtro_ataque(FILE *arff, atributo *atributos, int quantidade);
void cria_relatorio_tamanho(char *nome_ataque, int tamanho);
void filtro_tamanho(FILE *arff, atributo *atributos, int quantidade);

#endif