#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arff.h"

char *nome_arq = 0;

void exibe_atributos(atributo *infos, int quantidade){  //modificar
  //Fun��o do A1 (com modifica��es para o atributo de categorias) 
  if (infos == 0){
      printf("O arquivo ARFF fornecido eh invalido\n");
      exit(0);
  }

  printf("==== ATRIBUTOS DO ARQUIVO ====\n");
  for(int i = 0; i < tamanho; i++){
      printf("-> Atributo #%d\n", i+1);
      printf("Rotulo: %s\n", infos[i].rotulo);
      printf("Tipo: %s\n", infos[i].tipo);
      if (infos[i].categorias) printf("Categorias: %s\n", infos[i].categorias);
      if (i < tamanho-1) printf("------------------------------\n");
  }
  printf("===============================\n");
  
}

int conta_atributos(FILE *arff){ //ok
  //Fun��o do A1
    int contador = 0;
  int flag = 0;
  arff = fopen(nome_arq, "r");

  if(!arff){/home/eduarda/semestre-3/prog2/eaf21-A1
  	perror ("Erro ao abrir arquivo");
    exit (1) ;
  }

  char linha[1025];
  char linha_aux[1025];

  while (fgets(linha, sizeof(linha), arff) != NULL) {
    if (strstr(linha, "@attribute") != NULL) {
      contador++;
    }
    else {
      flag = 1;
    }
    strncpy(linha_aux, linha, sizeof(linha_aux));
  }

  if (strstr(linha_aux, "@data") == NULL) {
    perror("O arquivo ARFF fornecido eh invalido");
    exit (1);
  }

  if (flag == 0) {
    perror("O arquivo ARFF fornecido eh invalido por nao possuir");
    exit (1);
  }

  fclose (arff);
  exit (0) ;
}

void processa_categorias(atributo *elemento, char *categorias){
  //Recbe uma string com as categorias e atualiza o elemento com um vetor de strings (modificar a struct)
  //char [i][j]
  /*if (strstr(infos[i].tipo, "{") != NULL){
      aux = strstr(infos[i].tipo, "{");
      infos[i].categorias = strdup(aux);
    }
    else {
      infos[i].categorias = NULL;
    }*/
}

atributo* processa_atributos(FILE *arff, int quantidade){
  //Fun��o do A1 (com modifica��es para o atributo de categorias)
}

void valida_arff(FILE *arff, atributo *atributos, int quantidade){
  //Recebe um arquivo arff com ponteiro de leitura antes do "@data"; passa por todas as linhas de dados e valida cada elementos de cada coluna em
  //rela��o ao vetor de atributos tamb�m fornecido como argumento.
}