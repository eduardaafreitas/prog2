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

  char *token;

  token = strtok(categorias, ",");

  for(int i = 0; token != NULL ; i++ ){
    elemento->categorias = realloc(elemento->categorias, (i+1) * sizeof(char*));

    if(!elemento->categorias){
    perror("Erro ao alocar memoria realloc");
    exit(1);
  }

    elemento->categorias[i] = (char*)malloc(strlen(token) * sizeof(char));
    
    if(!elemento->categorias[i]){
      perror("Erro ao alocar memoria malloc");
      exit(1);
    }

    strncpy(elemento->categorias[i], token, strlen(token));
    
    token = strtok(NULL, ",");
  }

}

/*static int conta_categorias(FILE *arff, int quantidade){

  char linha[1025];
  char *tok;
  char *token;
  int qtd_categoria = 0;
  int maior_categoria = 0;
  char *tipos;

  for(int i = 0; i < quantidade; i++){
    fgets(linha, sizeof(linha), arff);
    tok = strtok(linha, " "); //ignoramos a string "@attribute"
    tok = strtok(NULL, " "); //pegamos o rotulo
    //infos[i].rotulo = strdup(tok);
    tok = strtok(NULL, " "); //pegamos o tipo
    tipos = strdup(tok);
    if (strstr(tipos, "{") != NULL){
      strcpy(linha, tipos); // Copia a string de categorias para uma auxiliar
      token = strtok(linha, ",");
      while( token != NULL ) { // Conta quantas categorias existem para delimitar o tamanhho do malloc do vetor de strings
        qtd_categoria++;
        if (qtd_categoria > maior_categoria){
          maior_categoria = qtd_categoria;
          token = strtok(NULL, ",");
        }
      }
    }
  }
  return maior_categoria;
}*/

atributo* processa_atributos(FILE *arff, int quantidade){
  //Fun��o do A1 (com modifica��es para o atributo de categorias)
  printf("quantidade: %d\n", quantidade);
 
  if(!arff){
    perror ("Erro ao abrir arquivo");
    exit (1) ;
  }

  fseek(arff, 0, SEEK_SET); //eh preciso resetar o ponteiro do arquivo para o inicio do arquivo

  char linha[1025];
  char *tok;

  atributo *infos = (atributo*)malloc(quantidade * sizeof(atributo));
  fseek(arff, 0, SEEK_SET); //eh preciso resetar o ponteiro do arquivo para o inicio do arquivo

  for(int i = 0; i < quantidade; i++){
    fgets(linha, sizeof(linha), arff);
    tok = strtok(linha, " "); //ignoramos a string "@attribute"

    tok = strtok(NULL, " "); //pegamos o rotulo
    infos[i].rotulo = (char *)realloc(infos[i].rotulo, strlen(tok) * sizeof(char));
    strcpy(infos[i].rotulo, tok);

    tok = strtok(NULL, " "); //pegamos o tipo
    infos[i].tipo  = (char *)realloc(infos[i].tipo, strlen(tok) * sizeof(char));
    strcpy(infos[i].tipo, tok);

    if (strstr(infos[i].tipo, "{") != NULL){
      processa_categorias(&infos[i], tok);
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