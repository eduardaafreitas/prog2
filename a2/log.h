#ifndef __LOG__
#define __LOG__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arff.h"

void cria_relatorio_ataque(char *nome_ataque, int numero_ocorrencias);
void filtro_ataque(FILE *arff, atributo *atributos, int quantidade);

#endif