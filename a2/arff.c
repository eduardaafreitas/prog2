#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "arff.h"

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
    perror("O arquivo ARFF fornecido eh invalido por nao possuir");
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
  int i = 0;

  token = strtok(categorias, ",");

  for(; token != NULL ; i++){
    elemento->categorias = realloc(elemento->categorias, (i+1) * sizeof(char*));

    if(!elemento->categorias){
    perror("Erro ao alocar memoria - realloc processa_categorias");
    exit(1);
  }

    elemento->categorias[i] = (char*)malloc(strlen(token) * sizeof(char));
    
    if(!elemento->categorias[i]){
      perror("Erro ao alocar memoria - malloc processa_categorias");
      exit(1);
    }

    strncpy(elemento->categorias[i], token, strlen(token));
    
    token = strtok(NULL, ",");
  }
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
  //Recebe um arquivo arff com ponteiro de leitura antes do "@data"; 
  //passa por todas as linhas de dados e valida cada elementos de cada coluna em
  //rela��o ao vetor de atributos tamb�m fornecido como argumento.

  if(!arff){ //verifica se o arquivo foi aberto
    perror ("Erro ao abrir arquivo - valida_arff");
    exit (1) ;
  }
  char linha[2049];
  //flags:
  int numerico = 0;         
  int string = 0;
  int categorico = 0;

  //variaveis auxiliares:
  char *tok;
  char *aux; 
  char *endptr;

  int contador = 0;
  //caso o arquivo nao esteja com o ptr de leitura no @data esse while fara isso
  while (fgets(linha, sizeof(linha), arff) != NULL) {
    if (strstr(linha, "@data") != NULL) {
      break; // Encontramos a linha "@data", vamos parar de ler
    }
  }
  fseek(arff, ftell(arff), SEEK_SET);
  printf("linha: %s\n", linha);


  printf("entrou no valida\n");
  
  while(!feof(arff)){
    printf("entrou no while valida\n");
    fgets(linha, sizeof(linha), arff);
    if(strcmp(linha, "\n") == 0){
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
    aux = tok;

    for (int i = 0; i < quantidade; i++){
      if( strcmp(atributos[i].tipo, "numeric") != NULL){ 
        double num = strtod(aux, &endptr);
        if (*endptr == '\0' && num >= INT_MIN && num <= INT_MAX) {
          numerico = 0;
        } else {
          numerico = 1;
        }

        if (endptr == aux){ //verifica se a conversao deu certo
          //printf("Erro\n");
          exit(1);
        }
      } else if (strstr(atributos->tipo[i], "{") != NULL){
        for(int j = 0; atributos[i].categorias[j] != NULL; j++){
          if (strcmp(aux, atributos[i].categorias[j]) == 0){
            categorico = 0;
            break;
          } else if (atributos[i].categorias[j+1] == NULL){
            categorico = 0;
            exit(1);
          } else if (strcmp(aux, atributos[i].categorias[j]) != 0){
            categorico = 1;
            continue;
          } 
        }

      } else if (strcmp(atributos[i].tipo, "string") != NULL){
        string = 1;
      }

    }
    contador++;
    

  }
  

}

