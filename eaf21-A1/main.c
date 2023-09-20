#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  char *rotulo;
  char *tipo;
  char *categorias;
} atributo;

void exibe_atributos(atributo *infos, int tamanho){
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

int conta_atributos(FILE* arff, char* nome_arq){
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


atributo* processa_atributos(FILE *arff, char* nome_arq){
  int tamanho = conta_atributos(arff, nome_arq);
  atributo *infos = malloc(tamanho * sizeof(atributo));
  char linha[1025];
  char *token;
  char *aux;

  arff = fopen(nome_arq, "r");
  if(!arff){
    perror ("Erro ao abrir arquivo");
    exit (1) ;
  }

  for(int i = 0; i < tamanho; i++){
    fgets(linha, sizeof(linha), arff);
    token = strtok(linha, " ");
    infos[i].rotulo = strdup(token);
    token = strtok(NULL, " ");
    infos[i].tipo = strdup(token);
    if (strstr(infos[i].tipo, "{") != NULL){
      aux = strstr(infos[i].tipo, "{");
      infos[i].categorias = strdup(aux);
    }
    else {
      infos[i].categorias = NULL;
    }
  }

  fclose(arff);
  return infos;
   
}



int main(int argc, char **argv){
  int opt;
  char exibicao = 0;
  char *entrada = 0;
  FILE *arq;

  while ((opt = getopt(argc, argv, "pi:")) != -1) {
    switch (opt) {
    case 'i':
      entrada = strdup(optarg);
      break;
    case 'p':
      exibicao = 1;
      break;
    default:
      fprintf(stderr, "Forma de uso: ./arff -i <arq_in> [-p]\n");
      exit(1);
    }
  }

  if ( ! entrada ) {
    fprintf(stderr, "Forma de uso: ./arff -i <arq_in> [-p]\n");
    exit(2);
  }

  if(entrada){
    conta_atributos(arq, entrada);
  }

  if (exibicao){
    //to do
  }

  return 0 ;
}