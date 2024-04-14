#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "io.h"
#include "lib.h"

void options(){

    printf("1) Sumario do arquivo\n");
    printf("2) Mostrar\n");
    printf("3) Filtrar\n");
    printf("9) Fim\n\n");

    printf("Digite a opcao desejada: ");
}

int type_verify(char * token){
    //printf("type_verify\n");
    if ((token[0] >= '0' && token[0] <= '9') || (token[0] == '-') || (token[0] == '+')){
        return 1;
    }
    else{
        return 2;
    }
}

void type(csv *keeper, base *database){
    //printf("type\n");
    
    keeper->type = (char*) malloc(keeper->column * sizeof(char));
    printf("tam type %ld\n", sizeof(keeper->type));

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


char* put_spaces(size_t size, size_t diff, char *origin_string){
    //printf("put_spaces\n");
    char *spaces = (char *)malloc((size + 1) * sizeof(char));
    memset(spaces, ' ', diff);
    strcat(spaces, origin_string);
    spaces[size] = '\0';
    //printf("spaces: %s\n", spaces);

    return spaces;

}

void fill_string(csv *keeper, base *database){
    printf("fill_string\n");
    for (int i = 0; i < keeper->row; i++){ //percorre linhas
    //printf("for linha\n");
        for (int j = 0; j < keeper->column; j++){ //percorre colunas  
        //printf("for coluna\n");
            // printf("[i][j]: %d %d\n", i, j);
            //printf("database->data[%d][%d]: %s\n", i, j, database->data[i][j]); 

            if((database->data[i][j]!=NULL) && (strlen (database->data[i][j]) < keeper->sizes[j])){
                size_t diff = keeper->sizes[j] - strlen(database->data[i][j]);
                char *spaces = put_spaces(keeper->sizes[j], diff, database->data[i][j]);
                //printf("spaces: %s\n", spaces);
                //printf("database->data[%d][%d]: %s\n", i, j, database->data[i][j]);
                strcpy(database->data[i][j], spaces);  
                
            }
        }
    }
}

size_t find_column(csv *keeper, base *database_copy, char *filter){
    size_t j, number_column;
    int cmp;

    for(j = 1; j < keeper->column; j++){
        cmp = strcasecmp(database_copy->data[0][j], filter);
        if (cmp == 0){
            number_column = j;
        }
    }
    return number_column;
}

