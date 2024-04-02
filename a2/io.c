#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "io.h"


void options(){

    printf("1) Sumario do Arquivo\n");
    printf("2) Mostrar\n");
    printf("3) Fim\n\n");

    printf("Digite a opcao desejada: ");
}

int conta_colunas(FILE *arquivo){
    char buffer_temp[1025];
    char *linha;
    int i = 0;
    char *tok;

    linha = fgets(buffer_temp, 1024, arquivo);
    
    if (linha != buffer_temp){
        fprintf(stderr, "Arquivo com erro. (f contcolumn)\n");
        exit(4);
    }
    
    tok = strtok(buffer_temp, ",");

    while (tok != NULL){
        // printf("%s \n", tok);
        tok = strtok(NULL, ",");
        i++;
    }
    
    return i;
}

int conta_linhas(FILE *arquivo){
    char buffer_temp[1025];
    char *linha;
    int contador = 0;

    while(1){
        
        linha = fgets(buffer_temp, 1024, arquivo);

        if (linha == NULL){
            break;
        }

        if (linha != buffer_temp){
            fprintf(stderr, "Arquivo com erro. (f contline) \n");
            exit(4);
        }

        contador++;
    }
    return contador;
}

arq_csv *processa_arquivo(FILE *arquivo, int qtd_linhas, int qtd_colunas){
    //char buffer_temp[1025];

    arq_csv *keep_csv = (arq_csv*) malloc(sizeof(arq_csv));
    
    //int i = 0;

    //char ***dados = (char***) malloc(qtd_linhas * sizeof(char**)); //linha

    /*for (i = 0; i < qtd_linhas; i++){
        //dados[i] = (char**) malloc(qtd_colunas * sizeof(char*)); //coluna
    }*/
    printf("entrou\n");
    if (!keep_csv){
        fprintf(stderr, "Erro ao alocar memoria. \n");
        exit(5);
    }

    /*for (i = 0; i < qtd_linhas; i++){   
        char* linha = fgets(buffer_temp, 1024, arquivo);

        if (linha == NULL){
            printf("Fim do arquivo. \n");
            break;
        }
        char *tok = strtok(buffer_temp, ",\n");

        for (int j = 0; j < qtd_colunas; j++) {
            //dados[i][j] = malloc(sizeof(char) * strlen(tok) + 1); //celula
            //strcpy(dados[i][j], tok);
            //tok = strtok(NULL, ",\n");
        }
    }*/

    return keep_csv;
}

int interpreta_dados(char * tok){
    if ((tok[0] >= '0' && tok[0] <= '9') || (tok[0] == '-') || (tok[0] == '+')){
        return 1;
    }
    else{
        return 2;
    }
}


void sumario(arq_csv *keeper_csv, int qtd_linhas, int qtd_colunas){

    printf("%d variaveis encontradas\n", qtd_colunas);
    for (int i = 0; i < qtd_colunas; i++){
        /*if (interpreta_dados(keeper_csv.type) == 1){
            printf("%s  [N] \n", dados[0][i]);
        }
        else{
            printf("%s  [S] \n", dados[0][i]);
        }*/
        printf("sumariooooo\n");
    }

}

void mostrar(arq_csv *keeper_csv, int qtd_linhas, int qtd_colunas){
    for (int i = 0; i < 5; i++){
        if (i != 0)
            printf("%20d ", i-1);
        else
            printf("%20s ", "");
        for (int j = 0; j < qtd_colunas; j++){
            //printf("%20s ", dados[i][j]);  
        }
        printf("\n");
    }

    for (int i = 0; i < (qtd_colunas+1); i++){
        printf("%20s ", "...");
    }

    printf("\n");

    for (int i = (qtd_linhas - 5); i < qtd_linhas; i++){
        printf("%20d ", i-1);
        for (int j = 0; j < qtd_colunas; j++){
            //printf("%20s ", dados[i][j]);
        }
        printf("\n");
    }
    printf(" %d rows x %d columns \n\n", qtd_linhas, qtd_colunas);
}

/*void libera(arq_csv *keeper_csv, int qtd_linhas, int qtd_colunas){
    for (int i = 0; i < qtd_linhas; i++){
        for (int j = 0; j < qtd_colunas; j++){
            free(dados[i][j]);
        }
        free(dados[i]);
    }
    free(dados);
}*/
