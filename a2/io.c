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
    printf("count_columns\n");
    char buffer_temp[1025];
    char *linha;
    unsigned long i = 0;
    char *tok;

    linha = fgets(buffer_temp, 1025, archive);
    
    if (linha != buffer_temp){
        fprintf(stderr, "arquivo com erro. err: countcolumn\n");
        exit(3);
    }
    
    tok = strtok(buffer_temp, ",");

    while (tok != NULL){
        tok = strtok(NULL, ",");
        i++;
    }
    
    return i;
}

unsigned long count_rows(FILE *archive){
    printf("count_rows\n");
    char buffer_temp[1025];
    char *linha;
    unsigned long contador = 0;

    while(1){
        
        linha = fgets(buffer_temp, 1024, archive);

        if (linha == NULL){

            break;
        }

        if (linha != buffer_temp){
            fprintf(stderr, "arquivo com erro. err: countline \n");
            exit(4);
        }

        contador++;
    }
    return contador;
}

csv *alloc_csv(){
    printf("alloc_csv\n");
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
    printf("alloc_database\n");
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

void count_stringsize(FILE *archive,csv *keeper, unsigned long row, unsigned long column){

    unsigned long i, j;
    char buffer_temp[1025];

    keeper->sizes = (size_t*) malloc(column * sizeof(size_t));

    if (!keeper->sizes) {
        fprintf(stderr, "Erro ao alocar memoria. err: alloc_sizes\n");
        exit(7);
    }
    for (i = 0; i < column; i++){
        keeper->sizes[i] = 0;
    }

    for (i = 0; i < keeper->row; i++){

        char* linha = fgets(buffer_temp, 1025, archive);

        if (linha == NULL){
            printf("Fim do arquivo. \n");
            break;
        }     

        char *line_ptr = buffer_temp;

        for (j = 0; j < keeper->column; j++) {
            char *comma_ptr = strchr(line_ptr, ',');
            if (comma_ptr != NULL) {
            
                size_t token_length = comma_ptr - line_ptr;
                if (token_length == 0) {
                    keeper->sizes[j] = keeper->sizes[j];
                } else {
                    if (token_length > keeper->sizes[j]) {
                        keeper->sizes[j] = token_length;
                    }
                }
                line_ptr = comma_ptr + 1;
            } else {
                if (*line_ptr == '\n' || *line_ptr == '\0') {
                    size_t token_length = strlen(line_ptr);
                    if (token_length > keeper->sizes[j]) {
                        keeper->sizes[j] = token_length;
                    }
                } else {
                    size_t token_length = strlen(line_ptr);
                    if (token_length > keeper->sizes[j]) {
                        keeper->sizes[j] = token_length;
                    }
                }
                break;
            }
        }
    }
}

void layin_csv(FILE *archive, csv *keeper, base *database, unsigned long row, unsigned long column){
    printf("layin_csv\n");
    char buffer_temp[1025];

    //atribui numero de linhas e colunas às structs
    keeper->row = row;
    keeper->column = column;

    database->row = row;
    database->column = column;

    count_stringsize(archive, keeper, row, column);
    fseek(archive, 0, SEEK_SET);

    unsigned long i, j;

    database->data = (char***) malloc(row * sizeof(char**)); //linha
    if (!database->data){
        fprintf(stderr, "Erro ao alocar memoria. err: alloc_data\n");
        exit(8);
    }

    for (i = 0; i < row; i++){
        database->data[i] = (char**) malloc(column * sizeof(char*)); //coluna
        if (!database->data[i]){
            fprintf(stderr, "Erro ao alocar memoria. err: alloc_data[i]\n");
            exit(8);
        }
    }


    for(j = 0; j < column; j++){
        printf("keeper->sizes[%lu]: %lu\n", j, keeper->sizes[j]);
    }

    for (i = 0; i < keeper->row; i++){
        char* linha = fgets(buffer_temp, 1025, archive);

        if (linha == NULL){
            printf("Fim do arquivo. \n");
            break;
        }     

        char *line_ptr = buffer_temp;

        for (j = 0; j < keeper->column; j++) {
            // Encontra a próxima vírgula ou o final da linha
            char *comma_ptr = strchr(line_ptr, ',');
            if (comma_ptr != NULL) {
                // Se encontrou uma vírgula, copia o token até a vírgula
                size_t token_length = comma_ptr - line_ptr;
                if (token_length == 0) {
                    // Se o token for vazio, atribui NULL
                    // database->data[i][j] = malloc((keeper->sizes[j]+1) * sizeof(char));
                     database->data[i][j] = NULL;
                    // printf("encontrou vazio\n");
                } else {
                    // Caso contrário, aloca memória e copia o token
                    database->data[i][j] = malloc((keeper->sizes[j]+1) * sizeof(char));
                    strncpy(database->data[i][j], line_ptr, token_length);
                    database->data[i][j][token_length] = '\0'; // Adiciona o caractere nulo
                }
                // Atualiza o ponteiro para apontar para o próximo caractere após a vírgula
                line_ptr = comma_ptr + 1;
            } else {
                // Se não encontrou uma vírgula, estamos no final da linha
                // Verifica se há um token restante
                if (*line_ptr == '\n' || *line_ptr == '\0') {
                    // Se for uma quebra de linha ou o final da linha, atribui NULL
                    database->data[i][j] = NULL;
                } else {
                    // Se não for, copia o token até o final da linha
                    //size_t token_length = strlen(line_ptr);
                    database->data[i][j] = malloc((keeper->sizes[j]+1) * sizeof(char));
                    strcpy(database->data[i][j], line_ptr);
                }
                // Não há mais tokens na linha, saia do loop
                break;
            }
        }
    }
}


int type_verify(char * token){
    printf("type_verify\n");
    if ((token[0] >= '0' && token[0] <= '9') || (token[0] == '-') || (token[0] == '+')){
        return 1;
    }
    else{
        return 2;
    }
}

void sumario(csv *keeper, base *database, unsigned long column){
    printf("sumario\n");
    keeper->type = (char*) malloc(column * sizeof(char));

    if(!keeper->type){
        fprintf(stderr, "Erro ao alocar memoria. err: alloc_type\n");
        exit(9);
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


char* put_spaces(size_t size, size_t diff, char *origin_string){
    //printf("put_spaces\n");
    char *spaces = (char *)malloc((size + 1) * sizeof(char));
    memset(spaces, ' ', diff);
    strcat(spaces, origin_string);
    spaces[size] = '\0';
    //printf("spaces: %s\n", spaces);

    return spaces;

}

void fill_string(csv *keeper, base *database, unsigned long row, unsigned long column){
    printf("fill_string\n");
    for (int i = 0; i < row; i++){ //percorre linhas
    //printf("for linha\n");
        for (int j = 0; j < column; j++){ //percorre colunas  
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

void mostrar(csv *keeper, base *database, unsigned long row, unsigned long column){
    printf("mostrar\n");
    fill_string(keeper, database, row, column);

    // for(int i = 0; i < column; i++){
    //     printf("coluna %d: %d\n", i, keeper->sizes[i]);
    // }
    for (int i = 0; i < 5; i++){
        if (i != 0)
            printf("%10d", i-1);
        else
            printf("%10s", " ");
        for (int j = 0; j < column; j++){
            printf("%10s ", database->data[i][j]);  
        }
        printf("\n");
    }

    // for (int i = 0; i < (column+1); i++){
    //     printf("%s ", "...");
    // }

    printf("\n\n");

    for (int i = (row - 5); i < row; i++){
        printf("%10d", i);
        for (int j = 0; j < column; j++){
            if(database->data[i][j] != NULL)
                printf("%10s ", database->data[i][j]);
        }
        printf("\n");
    }
    printf(" [%lu row x %lu column] \n\n", row, column);
}

void free_csv(csv *keeper){

    free(keeper->archive);
    free(keeper->type);
    free(keeper->sizes);

    free(keeper);
}

void free_database(base *database){

    for(int i = 0; i < database->row; i++){
        for(int j = 0; j < database->column; j++){
            free(database->data[i][j]);
        }
        free(database->data[i]);
    }
    free(database);
}
