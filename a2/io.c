#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "io.h"


void options(){

    printf("1) Sumario do arquivo\n");
    printf("2) Mostrar\n");
    printf("3) Fim\n\n");

    printf("Digite a opcao desejada: ");
}

unsigned long count_columns(FILE *archive){
    char buffer_temp[1025];
    char *linha;
    unsigned long i = 0;
    char *tok;

    linha = fgets(buffer_temp, 1024, archive);
    
    if (linha != buffer_temp){
        fprintf(stderr, "archive com erro. err: countcolumn\n");
        exit(3);
    }
    
    tok = strtok(buffer_temp, ",");

    while (tok != NULL){
        // printf("%s \n", tok);
        tok = strtok(NULL, ",");
        i++;
    }
    
    return i;
}

unsigned long count_rows(FILE *archive){
    char buffer_temp[1025];
    char *linha;
    unsigned long contador = 0;

    while(1){
        
        linha = fgets(buffer_temp, 1024, archive);

        if (linha == NULL){
            break;
        }

        if (linha != buffer_temp){
            fprintf(stderr, "archive com erro. err: countline \n");
            exit(4);
        }

        contador++;
    }
    return contador;
}

csv *alloc_csv(){

    csv *keeper = (csv*) malloc(sizeof(csv));

    if (!keeper){
        fprintf(stderr, "Erro ao alocar memoria. err: alloc_csv\n");
        exit(5);
    }

    //inicializa campos da struct
    keeper->archive = NULL;
    keeper->row = 0;
    keeper->column = 0;
    keeper->type = NULL;
    keeper->pos = 0;
    keeper->sizes = NULL;

    return keeper;
}

base *alloc_database(){

    base *database = (base*) malloc(sizeof(base));

    if (!database){
        fprintf(stderr, "Erro ao alocar memoria. err: alloc_database\n");
        exit(6);
    }

    database->column = 0;
    database->row = 0;
    database->data = NULL;

    return database;

}

void layin_csv(FILE *archive, csv *keeper, base *database, unsigned long row, unsigned long column){

    char buffer_temp[1025];

    //atribui numero de linhas e colunas às structs
    keeper->row = row;
    keeper->column = column;

    database->row = row;
    database->column = column;

    unsigned long i, j;

    database->data = (char***) malloc(row * sizeof(char**)); //linha

    for (i = 0; i < row; i++){
        database->data[i] = (char**) malloc(column * sizeof(char*)); //coluna
    }

    for (i = 0; i < keeper->row; i++){
        char* linha = fgets(buffer_temp, 1025, archive);

        if (linha == NULL){
            printf("Fim do archive. \n");
            break;
        }

        char *tok = strtok(buffer_temp, ",\n");

        for (j = 0; j < keeper->column; j++) {
            database->data[i][j] = malloc(sizeof(char) * strlen(tok) + 1); //celula
            strcpy(database->data[i][j], tok);
            tok = strtok(NULL, ",\n");
        }
    }
    
}

void count_stringsize(FILE *archive, csv *keeper, base *database, unsigned long row, unsigned long column){

    unsigned long i, j;

    keeper->sizes = (short*) malloc(column * sizeof(short));
    if (!keeper->sizes) {
        fprintf(stderr, "Erro ao alocar memoria. err: alloc_sizes\n");
        exit(7);
    }
    for (i = 0; i < keeper->column; i++){
        keeper->sizes[i] = 0;
    }
    
    for (i = 1; i < keeper->row; i++){
        for (j = 0; j < keeper->column; j++){
            if (strlen(database->data[i][j]) > keeper->sizes[i]){
                keeper->sizes[i] = strlen(database->data[i][j]);
            }
        }
    }
}

int type_verify(char * tok){
    if ((tok[0] >= '0' && tok[0] <= '9') || (tok[0] == '-') || (tok[0] == '+')){
        return 1;
    }
    else{
        return 2;
    }
}

//a fazer: verificar ,, antes de interpretar dados
void sumario(csv *keeper, base *database, int column){

    keeper->type = (char*) malloc(column * sizeof(char));

    if(!keeper->type){
        fprintf(stderr, "Erro ao alocar memoria. err: alloc_type\n");
        exit(8);
    }
    for (int i = 0; i < column; i++){
        if (type_verify(database->data[1][i]) == 1){
            keeper->type[i] = 'N';
            printf("%s  [%c] \n", database->data[0][i], keeper->type[i]);
        }
        else{
            keeper->type[i] = 'S';
            printf("%s  [%c] \n", database->data[0][i], keeper->type[i]);
        }
    }
    printf("%d variaveis encontradas\n", column);
}

/*void mostrar(csv *keeper, int rows, int columns){
    for (int i = 0; i < 5; i++){
        if (i != 0)
            printf("%20d ", i-1);
        else
            printf("%20s ", "");
        for (int j = 0; j < columns; j++){
            //printf("%20s ", dados[i][j]);  
        }
        printf("\n");
    }

    for (int i = 0; i < (columns+1); i++){
        printf("%20s ", "...");
    }

    printf("\n");

    for (int i = (rows - 5); i < rows; i++){
        printf("%20d ", i-1);
        for (int j = 0; j < columns; j++){
            //printf("%20s ", dados[i][j]);
        }
        printf("\n");
    }
    printf(" %d rows x %d columns \n\n", rows, columns);
}*/




void free_csv(csv *keeper){
    free(keeper);
}

void free_database(base *database){
    free(database);
}
