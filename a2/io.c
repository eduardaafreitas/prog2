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

void handle_input(int argc, char *argv[]){
    if (argc < 1){
        fprintf(stderr, "Forma de uso: ./csvreader <arq_in> \n");
        exit(3);
    }
}

void open_check(FILE *archive){
    if (!archive){
        fprintf(stderr, "Erro ao abrir o archive!\n");
        exit(4);
    }    
}

unsigned long count_columns(FILE *archive){
    char buffer_temp[1025];
    char *linha;
    unsigned long i = 0;
    char *tok;

    linha = fgets(buffer_temp, 1024, archive);
    
    if (linha != buffer_temp){
        fprintf(stderr, "archive com erro. err: countcolumn\n");
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
        exit(5);
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

    for (i = 1; i < keeper->row; i++){
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

/*int interpreta_dados(char * tok){
    if ((tok[0] >= '0' && tok[0] <= '9') || (tok[0] == '-') || (tok[0] == '+')){
        return 1;
    }
    else{
        return 2;
    }
}*/


void sumario(csv *keeper, int rows, int columns){

    //printf("%d variaveis encontradas\n", columns);
    for (int i = 0; i < columns; i++){
        /*if (interpreta_dados(keeper.type) == 1){
            printf("%s  [N] \n", dados[0][i]);
        }
        else{
            printf("%s  [S] \n", dados[0][i]);
        }*/
        //printf("sumario!!!!\n");
    }
    printf("sumario!!!!\n");

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
