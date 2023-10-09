#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "arff.h"

int numerico;         
int string;
int categorico;

void exibe_atributos(atributo *infos, int tamanho){  //modificar
  //Fun��o do A1 (com modifica��es para o atributo de categorias) 
  if (infos == 0){
    printf("O arquivo ARFF fornecido eh invalido print\n");
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

void libera_atributos(atributo *infos, int tamanho)
{
  for (int i = 0; i < tamanho; i++)
  {
    free(infos[i].rotulo);
    free(infos[i].tipo);
    free(infos[i].categorias);
  }
  free(infos);
}

int conta_atributos(FILE *arff){ //ok
  //Fun��o do A1
  int contador = 0;
  int flag = 0;
  int flag_data = 0;

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
    if (strstr(linha, "@data") != NULL) {
      flag_data = 1;
    }
    strncpy(linha_aux, linha, sizeof(linha_aux));
  }

  if (flag_data == 0) {
    perror("O arquivo ARFF fornecido eh invalido por nao fornecer a linha @data");
    exit (1);
  } else if (contador == 0) {
    perror("O arquivo ARFF fornecido eh invalido por nao possuir atributos");
    exit (1);
  }

  if (flag == 0) {
    perror("O arquivo ARFF fornecido eh invalido");
    exit (1);
  }

  printf("O arquivo ARFF fornecido possui %d atributos\n", contador); //tirar depois
  //fclose (arff);
  return contador;
}

void processa_categorias(atributo *elemento, char *categorias){
  //Recbe uma string com as categorias e atualiza o elemento com um vetor de strings (modificar a struct)
  char *token;
  int i = 0;

  // ignora {
  categorias++;
  token = strtok(categorias, ",");
  elemento->categorias = NULL;
  for(; token != NULL ; i++){
    elemento->categorias = realloc(elemento->categorias, (i+1) * sizeof(char*));

    if(!elemento->categorias){
    perror("Erro ao alocar memoria - realloc processa_categorias");
    exit(1);
  }

    elemento->categorias[i] = (char*)malloc((strlen(token)+1) * sizeof(char));
    
    if(!elemento->categorias[i]){
      perror("Erro ao alocar memoria - malloc processa_categorias");
      exit(1);
    }

    strncpy(elemento->categorias[i], token, strlen(token)+1);
    
    token = strtok(NULL, ",");
  }

  // ignora }
  elemento->categorias[i-1][strlen(elemento->categorias[i-1])-1] = '\0';
  
  elemento->categorias = realloc(elemento->categorias, (i+1) * sizeof(char*));
  elemento->categorias[i] = NULL; // garantir que o final é nulo.
}



atributo* processa_atributos(FILE *arff, int quantidade){
  //Fun��o do A1 (com modifica��es para o atributo de categorias)
  printf("quantidade: %d\n", quantidade);
 
  if(!arff){
    perror ("Erro ao abrir arquivo - processa_atributos");
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
    infos[i].rotulo = NULL;
    infos[i].rotulo = (char *)realloc(infos[i].rotulo, (strlen(tok)+1) * sizeof(char));
    strcpy(infos[i].rotulo, tok);

    tok = strtok(NULL, " "); //pegamos o tipo
    tok[strlen(tok)-1] = '\0';
    infos[i].tipo = NULL;
    infos[i].tipo  = (char *)realloc(infos[i].tipo, (strlen(tok)+1) * sizeof(char));
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
  //Recebe um arquivo arff com ponteiro de leitura antes do "@data"; 
  //passa por todas as linhas de dados e valida cada elementos de cada coluna em
  //rela��o ao vetor de atributos tamb�m fornecido como argumento.

  if(!arff){ //verifica se o arquivo foi aberto
    perror ("Erro ao abrir arquivo - valida_arff");
    exit (1) ;
  }
  char linha[2049];
  //flags:

  //variaveis auxiliares:
  char *tok;
  char *endptr;

  //para localizar as linhas(caso necessario):
  int contador = 0;

  //caso o arquivo nao esteja com o ptr de leitura no @data esse while fara isso
  while (fgets(linha, sizeof(linha), arff) != NULL) {
    if (strstr(linha, "@data") != NULL) {
      break; // Encontramos a linha "@data", vamos parar de ler
    }
  }
  fseek(arff, ftell(arff), SEEK_SET);
 
  int valido = 1;
  while(!feof(arff)){
    if (!fgets(linha, sizeof(linha), arff)) {
      break;
    }
    if(strcmp(linha, "\n") == 0){ //verificar se a linha esta vazia
      continue;
    }
    if(strcmp(linha, "@data") == 0){
      continue;
    }
    tok = strtok(linha, ",");
    if (tok == NULL){
      printf("Erro na linha (tok eh NULL - valida arff) %d\n", contador);
      exit(1);
    }
    for (int i = 0; i < quantidade; i++){
      if (i == quantidade-1) {
        // remove \n
        tok[strlen(tok)-1] = '\0';
      }
      if( strcmp(atributos[i].tipo, "numeric") == 0){
        strtod(tok, &endptr);
        if (*endptr == '\0') {
          numerico = 1;
        } else {
          valido = 0;
        }
      } else if (strstr(atributos[i].tipo, "{") != NULL){
        categorico = 0;
        for(int j = 0; atributos[i].categorias[j] != NULL; j++){
          if (strcmp(tok, atributos[i].categorias[j]) == 0){
            categorico = 1;
            break;
          }
        }
        if (!categorico) { valido = 0; }
      } else if (strcmp(atributos[i].tipo, "string") == 0){
        string = 1;
      }
      tok = strtok(NULL, ",");
    }
    contador++;
  }
  fprintf(stderr, "valido: %d\n", valido);
}