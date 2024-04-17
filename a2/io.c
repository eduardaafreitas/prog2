#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "io.h"
#include "lib.h"


unsigned long count_columns(FILE *archive){

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
    
    return i+1;
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
            fprintf(stderr, "arquivo com erro. err: countline \n");
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

        for (j = 1; j < keeper->column; j++) {
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

    char buffer_temp[1025];

    //atribui numero de linhas e colunas às structs
    keeper->row = row;
    keeper->column = column;

    database->row = row;
    database->column = column;

    count_stringsize(archive, keeper, row, column);
    fseek(archive, 0, SEEK_SET);

    unsigned long i, j;

    //converte o numero do indice para string
    char str[row];
    snprintf(str, row, "%lu", row);
    keeper->sizes[0] = strlen(str);

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


    // for(j = 0; j < column; j++){
    //     printf("keeper->sizes[%lu]: %lu\n", j, keeper->sizes[j]);
    // }

    for (i = 0; i < keeper->row; i++){
        char* linha = fgets(buffer_temp, 1025, archive);

        //substitui o \n por espaço
        char* aux = strchr(buffer_temp, '\n');
        *aux = ' ';

        if (linha == NULL){
            printf("Fim do arquivo. \n");
            break;
        }

        char *line_ptr = buffer_temp;

        for (j = 0; j < keeper->column; j++) {
            if (j == 0) {
                // Preenche a posição database[i][0] com o valor de j
                database->data[i][0] = malloc((keeper->sizes[0] + 1) * sizeof(char));
                snprintf(database->data[i][0], keeper->sizes[j] + 1, "%lu", i);
            } else {
                database->data[i][j] = malloc((keeper->sizes[j]+1) * sizeof(char));
                // Encontra a próxima vírgula ou o final da linha
                char *comma_ptr = strchr(line_ptr, ',');
                if (comma_ptr != NULL) {
                    // Se encontrou uma vírgula, copia o token até a vírgula
                    size_t token_length = comma_ptr - line_ptr;
                    if (token_length == 0) {
                        // Se o token for vazio, atribui NULL
                        database->data[i][j] = NULL;
                    } else {
                        // Caso contrário copia o token
                        strncpy(database->data[i][j], line_ptr, token_length);
                        database->data[i][j][token_length] = '\0'; // Adiciona o caractere nulo
                    }
                    // Atualiza o ponteiro para apontar para o próximo caractere após a vírgula
                    line_ptr = comma_ptr + 1;
                } else {
                    // Se não encontrou uma vírgula, estamos no final da linha
                    // Verifica se há um token restante
                    if (*line_ptr == '\n') {
                        // Se for uma quebra de linha ou o final da linha, gere um token vazio
                        getc(archive);
                    } else {
                        // Se não for, copia o token até o final da linha
                        //size_t token_length = strlen(line_ptr);
                        strcpy(database->data[i][j], line_ptr);
                    }
                    // Não tem mais tokens na linha, sai do loop
                    break;
                }
            }
        }
    }

    for (int i = 0; i < keeper->row; i++){
        for (int j = 0; j < keeper->column; j++){
            if (database->data[i][j] == NULL){
                database->data[i][j] = malloc((keeper->sizes[j]+1) * sizeof(char));
                database->data[i][j][0] = '0';
            }
                
        }
    }

}

void sumario(csv *keeper, base *database){

    type(keeper, database);
    for (int j = 1;j < keeper->column; j++){
        printf("%s  [%c] \n", database->data[0][j], keeper->type[j]);
    }

    printf("%lu variaveis encontradas\n", keeper->column-1);
}

void mostrar(csv *keeper, base *database){
    fill_string(keeper, database);

    for (int i = 0; i < 5; i++){
        for (int j = 0; j < keeper->column; j++){
            if (database->data[i][j] != NULL)
                printf("%10s ", database->data[i][j]);  
        }
        printf("\n");
    }

    printf("\n\n");

    for (int i = (keeper->row - 6); i < keeper->row-1; i++){
        for (int j = 0; j < keeper->column; j++){
            if (database->data[i][j] != NULL)
                printf("%10s ", database->data[i][j]);  
            
        }
        printf("\n");
    }
    printf(" [%lu row x %lu column] \n\n", keeper->row, keeper->column);
}

//PASSAR DATABASE_COPY NOS PARAMETROS
void filtrar(base *database, csv *keeper, base *database_copy){
    type(keeper, database_copy);
    char filter[1025];
    printf("entre com a variavel: ");
    scanf("%s", filter);

    size_t *index = (size_t*) malloc(keeper->row * sizeof(size_t));

    if (!index) {
        printf("Erro\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < keeper->row; i++) {
        index[i] = 0; // Preenche o vetor com 0 para inicializar
    }

    char filter_opt[3];
    bool correto = false;

    while (!correto) {
        printf("Escolha um filtro ( == > >= < <= != ):  ");
        scanf("%2s", filter_opt); // Limitando a entrada a 2 caracteres

        char value[1025];
        printf("Digite um valor: ");
        scanf("%s", value);


        if (strcmp(filter_opt, "==") == 0) {
            filter_equal(database, keeper, database_copy, filter, value, index);
            correto = true;
        } else if (strcmp(filter_opt, ">") == 0) {
            filter_bigger(database, keeper, database_copy, filter, value, index);

            correto = true;
        } else if (strcmp(filter_opt, ">=") == 0) {
            filter_biggerorequal(database, keeper, database_copy, filter, value, index);
            correto = true;
        } else if (strcmp(filter_opt, "<") == 0) {
            filter_smaller(database, keeper, database_copy, filter, value, index);
            correto = true;
        } else if (strcmp(filter_opt, "<=") == 0) {
            filter_lessorequal(database, keeper, database_copy, filter, value, index);
            correto = true;
        } else if (strcmp(filter_opt, "!=") == 0) {
            filter_different(database, keeper, database_copy, filter, value, index);
            correto = true;
        } else {
            fprintf(stderr, "Digite uma opcao valida!\n");
            // Limpar o buffer de entrada antes de tentar novamente
            while (getchar() != '\n');
        }
    }
    
    // for (size_t i = 0; i < keeper->row; i++) {
    //     free(index[i]);
    // }
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
