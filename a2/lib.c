//desenvolvido por Eduarda de Aguiar Freitas, GRR 20211799
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include "io.h"
#include "lib.h"
#include "lib_filter.h"
#include "lib_description.h"

//nesta biblioteca estao presentes as funcoes usadas para manipulacao direta dos dados, funcoes que sao chamadas dentro das funcoes da biblioteca io.c

void options(){

    printf("1) Sumario do arquivo\n");
    printf("2) Mostrar\n");
    printf("3) Filtrar\n");
    printf("4) Descricao dos dados\n");
    printf("5) Ordenar\n");
    printf("6) Selecionar\n");
    printf("7) Dados Faltantes\n");
    printf("8) Salvar\n");
    printf("9) Fim\n\n");

    printf("Digite a opcao desejada: ");
}

int type_verify(char * token){  //verifica se o token é numerico ou string
    if ((token[0] >= '0' && token[0] <= '9') || (token[0] == '-') || (token[0] == '+')){
        return 1;
    }
    else{
        return 2;
    }
}

void type(csv *keeper, base *database){ //verifica o tipo de dado de cada coluna
    
    keeper->type = (char*) malloc(keeper->column * sizeof(char));
    if(!keeper->type){
        fprintf(stderr, "Erro ao alocar memoria. err: alloc_type\n");
        exit(9);
    }

    for (int j = 1;j < keeper->column; j++){
        if (type_verify(database->data[1][j]) == 1){
            keeper->type[j] = 'N';
        }
        else{
            keeper->type[j] = 'S';
        }
    }
}

char* put_spaces(size_t size, size_t diff, char *origin_string){    //função que coloca espaços em branco para preencher a string
    char *spaces = (char *)malloc((size + 1) * sizeof(char));
    memset(spaces, ' ', diff);
    strcat(spaces, origin_string);
    spaces[size] = '\0';
    return spaces;

}

void fill_string(csv *keeper, base *database){  //preenche as strings com espaços em branco para que todas tenham o mesmo tamanho

    for (int i = 0; i < keeper->row; i++){ //percorre linhas
        for (int j = 0; j < keeper->column; j++){ //percorre colunas  
            if(((database->data[i][j] != NULL)) && (strlen (database->data[i][j]) < keeper->sizes[j])){ //verifica se a string é menor que o tamanho da coluna
                size_t diff = keeper->sizes[j] - strlen(database->data[i][j]);                          //calcula a diferença entre o tamanho da string e o tamanho da coluna
                char *spaces = put_spaces(keeper->sizes[j], diff, database->data[i][j]);                //preenche a string com espaços em branco
                strcpy(database->data[i][j], spaces);                                                   //copia a string preenchida para o database original
            }
        }
    }
}

size_t find_column(csv *keeper, base *database_copy, char *filter){ //encontra a coluna que contem o filtro
    size_t j, number_column;
    int cmp;

    for(j = 1; j < keeper->column; j++){
        cmp = strcasecmp(database_copy->data[0][j], filter);    //compara a string da coluna com o filtro
        if (cmp == 0){
            number_column = j;
        }
    }
    return number_column;   //retorna o numero da coluna em que o filtro desejado se encontra
}
