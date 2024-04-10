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

int type_verify(char * token){
    if ((token[0] >= '0' && token[0] <= '9') || (token[0] == '-') || (token[0] == '+')){
        return 1;
    }
    else{
        return 2;
    }
}

//a fazer: verificar ,, antes de interpretar dados
//(add row e fazer um for pra verificar a primeira linha que não possui ,,)
void sumario(csv *keeper, base *database, unsigned long column){

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
    printf("%lu variaveis encontradas\n", column);
}

void count_stringsize(csv *keeper, base *database, unsigned long row, unsigned long column){

    unsigned long i, j;

    keeper->sizes = (short*) malloc(column * sizeof(short));
    if (!keeper->sizes) {
        fprintf(stderr, "Erro ao alocar memoria. err: alloc_sizes\n");
        exit(7);
    }
    for (i = 0; i < keeper->column; i++){
        keeper->sizes[i] = 0;
    }
    
    for (i = 0; i < keeper->row; i++){
        for (j = 0; j < keeper->column; j++){
            if (strlen(database->data[i][j]) > keeper->sizes[j]){
                keeper->sizes[j] = strlen(database->data[i][j]);
            }
        }
    }
    // for (int k = 0; k < keeper->column; k++){
    //     printf("column %d", keeper->sizes[k]);
    // }
}

char* put_spaces(short size, size_t diff, char *origin_string){

    char *spaces = (char *)malloc((size + 1) * sizeof(char));
    memset(spaces, ' ', diff);
    strcat(spaces, origin_string);
    
    return spaces;

}


void fill_string(csv *keeper, base *database, unsigned long row, unsigned long column){

    count_stringsize(keeper, database, row, column);

    for (int i = 0; i < row; i++){ //percorre linhas
        for (int j = 0; j < column; j++){ //percorre colunas        
            if( strlen (database->data[i][j]) < keeper->sizes[j]){
                size_t diff = keeper->sizes[j] - strlen(database->data[i][j]);
                char *spaces = put_spaces(keeper->sizes[j], diff, database->data[i][j]);
            }
        }
        printf("\n");
    }
}

void mostrar(csv *keeper, base *database, unsigned long row, unsigned long column){
   
    fill_string(keeper, database, row, column);

    for(int i = 0; i < column; i++){
        printf("coluna %d: %d\n", i, keeper->sizes[i]);
    }
    for (int i = 0; i < 5; i++){
        if (i != 0)
            printf("%10d ", i-1);
        else
            printf("%s ", "");
        for (int j = 0; j < column; j++){
            printf("%10s ", database->data[i][j]);  
        }
        printf("\n");
    }

    for (int i = 0; i < (column+1); i++){
        printf("%s ", "...");
    }

    printf("\n");

    for (int i = (row - 5); i < row; i++){
        printf("%10d ", i-1);
        for (int j = 0; j < column; j++){
            printf("%10s ", database->data[i][j]);
        }
        printf("\n");
    }
    printf(" [%lu row x %lu column] \n\n", row, column);
}

void free_csv(csv *keeper){
    free(keeper);
}

void free_database(base *database){
    free(database);
}
