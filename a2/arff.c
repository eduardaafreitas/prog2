#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arff.h"

char *nome_arq;

void exibe_atributos(atributo *infos, int tamanho){  //modificar
  //Fun��o do A1 (com modifica��es para o atributo de categorias) 
  if (infos == 0){
      printf("O arquivo ARFF fornecido eh invalido\n");
      exit(0);
  }

  printf("==== ATRIBUTOS DO ARQUIVO ====\n");
  for (int i = 0; i < tamanho; i++) {
    printf("-> Atributo #%d\n", i + 1);
    printf("Rotulo: %s\n", infos[i].rotulo);
    printf("Tipo: %s\n", infos[i].tipo);
    if (infos[i].categorias != NULL) {
      printf("Categorias:\n");
      for (int j = 0; infos[i].categorias[j] != NULL; j++) {
        printf("   %s\n", infos[i].categorias[j]);
      }
    }
    if (i < tamanho - 1)
      printf("------------------------------\n");
  }
  printf("===============================\n");
  
}

int conta_atributos(FILE *arff){ //ok
  //Fun��o do A1
  int contador = 0;
  int flag = 0;
  //arff = fopen(nome_arq, "r");

  if(!arff){
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

  printf("O arquivo ARFF fornecido possui %d atributos\n", contador); //tirar depois
  //fclose (arff);
  return contador;
}

void processa_categorias(atributo *elemento, char *categorias){
  //Recbe uma string com as categorias e atualiza o elemento com um vetor de strings (modificar a struct)
  //int tamanho = conta_atributos(arff);
  
  char linha[1025];
  char *tok;
  char *aux;
  char **matriz_auxiliar;

  /*arff = fopen(nome_arq, "r");
  if(!arff){
    perror ("Erro ao abrir arquivo");
    exit (1) ;
  }*/

  char *token;
  int qtd_categoria = 0;
  token = strtok(linha, " ");
  while( token != '}' ) { // Conta quantas categorias existem
    qtd_categoria++;
    token = strtok(NULL, " ");
  }
  matriz_auxiliar = (char**)malloc(qtd_categoria * sizeof(char*)); // Aloca a matriz de categorias
  if(!matriz_auxiliar){
    perror("Erro ao alocar memoria");
    exit(1);
  }
  for(int j = 0; j < qtd_categoria; j++){
    matriz_auxiliar[j] = (char*)malloc(1025 * sizeof(char)); // Aloca cada linha da matriz - "nome" da categoria
    if(!matriz_auxiliar[j]){
      perror("Erro ao alocar memoria");
      exit(1);
    }
    strcpy(matriz_auxiliar[j], token); // Copia o nome da categoria para a matriz
    token = strtok(NULL, " "); // Pega o proximo nome de categoria
  }
}


atributo* processa_atributos(FILE *arff, int quantidade){
  //Fun��o do A1 (com modifica��es para o atributo de categorias)
  //int tamanho = conta_atributos(arff, nome_arq);
  atributo *infos = malloc(quantidade * sizeof(atributo));
  char linha[1025];
  char *tok;
  char *aux;

  arff = fopen(arff, "r");
  if(!arff){
    perror ("Erro ao abrir arquivo");
    exit (1) ;
  }

  for(int i = 0; i < quantidade; i++){
    fgets(linha, sizeof(linha), arff);
    tok = strtok(linha, " "); //ignoramos a string "@attribute"
    tok = strtok(NULL, " "); //pegamos o rotulo
    infos[i].rotulo = strdup(tok);
    tok = strtok(NULL, " "); //pegamos o tipo
    infos[i].tipo = strdup(tok);
    if (strstr(infos[i].tipo, "{") != NULL){
      processa_categorias(&infos[i], infos[i].tipo);
    }
    else {
      infos[i].categorias = NULL;
    }
  }

  fclose(arff);
  return infos;

}

void valida_arff(FILE *arff, atributo *atributos, int quantidade){
  //Recebe um arquivo arff com ponteiro de leitura antes do "@data"; passa por todas as linhas de dados e valida cada elementos de cada coluna em
  //rela��o ao vetor de atributos tamb�m fornecido como argumento.
}