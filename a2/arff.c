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

  char linha[1025];
  char **matriz_auxiliar = NULL;
  char *token;
  int qtd_categoria = 0;

  strcpy(linha, categorias); // Copia a string de categorias para uma auxiliar
  token = strtok(linha, ",");

  while( token != NULL ) { // Conta quantas categorias existem
    qtd_categoria++;
    token = strtok(NULL, ",");
  }

  printf("Quantidade de categorias: %d\n", qtd_categoria); //tirar depois

  matriz_auxiliar = (char**)malloc(qtd_categoria * sizeof(char*)); // Aloca a matriz de categorias
  if(!matriz_auxiliar){
    perror("Erro ao alocar memoria");
    exit(1);
  }
  printf("Alocou matriz\n");

  char *token_aux; 

  token_aux = strtok(categorias, ","); // Separa as categorias

  for(int j = 0; ((j < qtd_categoria) || (token_aux != NULL )); j++){
    matriz_auxiliar[j] = (char*)malloc(1025 * sizeof(char));
    if(!matriz_auxiliar[j]){
      perror("Erro ao alocar memoria");
      exit(1);
    }
    strcpy(matriz_auxiliar[j], token_aux);
    token_aux = strtok(NULL, ","); // Separa as categorias
    printf(" matriz aux %s \n", matriz_auxiliar[j]);
  }
  //return matriz_auxiliar;
}


atributo* processa_atributos(FILE *arff, int quantidade){
  //Fun��o do A1 (com modifica��es para o atributo de categorias)
  printf("entrou na processa antes malloc\n");
  printf("quantidade: %d\n", quantidade);

  atributo *infos = (struct atributo *)malloc(quantidade * sizeof(atributo));
  infos->categorias = NULL;
  infos->categorias = (char **)malloc(50*sizeof(char*));

  for(int i = 0; i < 50; i++){
    infos->categorias[i] = (char *)malloc(1025*sizeof(char));
  }


  char linha[1025];
  char *tok;

  if(!arff){
    perror ("Erro ao abrir arquivo");
    exit (1) ;
  }

  fseek(arff, 0, SEEK_SET); //eh preciso resetar o ponteiro do arquivo para o inicio do arquivo

  for(int i = 0; i < quantidade; i++){
    fgets(linha, sizeof(linha), arff);
    tok = strtok(linha, " "); //ignoramos a string "@attribute"
    tok = strtok(NULL, " "); //pegamos o rotulo
    infos[i].rotulo = strdup(tok);
    tok = strtok(NULL, " "); //pegamos o tipo
    infos[i].tipo = strdup(tok);
    if (strstr(infos[i].tipo, "{") != NULL){
      processa_categorias(infos[i].tipo, tok);
      printf("tok cat: %s \n", tok);
    }
    else {
      infos[i].categorias = NULL;
    }
  }

  return infos;

}

void valida_arff(FILE *arff, atributo *atributos, int quantidade){
  //Recebe um arquivo arff com ponteiro de leitura antes do "@data"; passa por todas as linhas de dados e valida cada elementos de cada coluna em
  //rela��o ao vetor de atributos tamb�m fornecido como argumento.
}