//desenvolvido por Eduarda de Aguiar Freitas, GRR 20211799
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "io.h"
#include "lib.h"

//nesta biblioteca estao presentes as funcoes usadas para manipulacao direta dos dados, funcoes que sao chamadas dentro das funcoes da biblioteca io.c

void options(){

    printf("1) Sumario do arquivo\n");
    printf("2) Mostrar\n");
    printf("3) Filtrar\n");
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

void database_filtered(csv *keeper, base *database, size_t *index, char *file_name){ //grava um arquivo com os dados filtrados

    FILE *archive = fopen(file_name, "w");
    if(!archive){
        fprintf(stderr, "Erro ao abrir arquivo. err: database_filtered\n");
        exit(9);
    }

    for(size_t i = 0; i < keeper->row; i++){
        if(index[i] == i){                                  //se o index da linha for igual ao numero da linha, entao a linha é gravada
            for(size_t j = 0; j < keeper->column; j++){
                fprintf(archive, "%s", database->data[i][j]);
                if (j < (keeper->column - 1)) {
                    fprintf(archive, ",");
                }
            }
        }
        if (i < (keeper->row - 1) && (index[i] == i)){
            fprintf(archive, "\n");
        }
            
    }
}

void show_filter(csv *keeper, base *database_copy, size_t *index){  //mostra os dados filtrados
    
    fill_string(keeper, database_copy);

    int counter = 0;
    for(size_t i = 0; i < keeper->row-1; i++){
        if(i < keeper->row && index[i] == i && counter < 5){
            // Acesso seguro a index[i]
            for(size_t j = 0; j < keeper->column; j++){
                printf("%10s ", database_copy->data[i][j]);
            }
            counter++;
        }
        if ((i < (keeper->row - 1)) && (index[i] == i)){
            printf("\n");
        }
        if (counter == 5){
            break;
        }
    }

    printf("---------\n");

    size_t counter_aux = 0; 
    size_t aux = 0;
    for (size_t k = keeper->row - 2; k > 0; k--){   //percorre o index de tras para frente para saber o 5 ultimo index que foi marcado
        
        if (index[k] == k){
            counter_aux++;
            aux = k;

        }
        if (counter_aux == 5){
            break;
        }
    }

    counter = 0;
    for(size_t i = aux; i < keeper->row-1; i++){        //mostra os 5 ultimos index marcados
        if((index[i] == i) && (counter < 5)){
            for(size_t j = 0; j < keeper->column; j++){
                printf("%10s ", database_copy->data[i][j]);
            }
            counter++;
        }
        if (i < (keeper->row - 1) && (index[i] == i)){
            printf("\n");
        }
            
    }

    printf(" [%lu row x %lu column] \n\n", keeper->row, keeper->column);
}
//----------------------------------------------------------------------------------------------------------------------
//abaixo estao as funcoes usadas no filtro!
//cada filtro ( == > >= < <= != ) possui suas correspondentes funcoes:

//todas as funcoes *something*_number convertem o valor numerico do filtro para float e comparam com os valores da coluna
//caso atenda as condições, marcam o index da linha
void bigger_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    float value_convert, database_value;
    value_convert = atof(value);
    for(size_t i = 1; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            database_value = atof(database_copy->data[i][column]);
            if(database_value > value_convert){
                index[i] = i;
            } else {
                index[i] = 0;
            }
        }
    }
}

//todas as funcoes filter_*something* recebem o database original, o csv, uma copia do database original, o filtro e o valor do filtro
//e chama a correspondente funcao *something*_number para comparar os valores da coluna com o filtro
//caso atenda as condições, chama a funcao show_filter para mostrar os dados filtrados
//e pergunta se deseja gravar um arquivo com os dados filtrados
void filter_bigger(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index){

    char save[1];
    char file_name[1025];
    size_t column = find_column(keeper, database_copy, filter);
    if(keeper->type[column] == 'N'){
        bigger_number(keeper, database_copy, column, value, index);    
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if((strcmp(save, "S") == 0) || (strcmp(save, "s") == 0)){
            printf("Entre com o nome do arquivo: \n");
            scanf("%s", file_name);
            database_filtered(keeper, database, index, file_name);

        }
        else if(strcmp(save, "N") == 0){
            exit(9);
        }
    } 
    else{
        printf("Erro: Coluna nao numerica. Escolha outro filtro.\n");
    }

}
//----------------------------------------------------------------------------------------------------------------------
void smaller_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    float value_convert, database_value;
    value_convert = atof(value);
    for(size_t i = 1; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            database_value = atof(database_copy->data[i][column]);
            if(database_value < value_convert){
                index[i] = i;
            } else {
                index[i] = 0;
            }
        }
    }
}
void filter_smaller(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index){

    char save[1];
    char file_name[1025];
    size_t column = find_column(keeper, database_copy, filter);
    if(keeper->type[column] == 'N'){
        smaller_number(keeper, database_copy, column, value, index);      
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if((strcmp(save, "S") == 0) || (strcmp(save, "s") == 0)){
            printf("Entre com o nome do arquivo: \n");
            scanf("%s", file_name);
            database_filtered(keeper, database, index, file_name);

        }
        else if(strcmp(save, "N") == 0){
            exit(9);
        }
    } 
    else{
        printf("Erro: Coluna nao numerica. Escolha outro filtro.\n");
    }

}
//----------------------------------------------------------------------------------------------------------------------
void lessorequal_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    float value_convert, database_value;
    value_convert = atof(value);
    for(size_t i = 0; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            database_value = atof(database_copy->data[i][column]);
            if((database_value < value_convert) || (database_value == value_convert)){
                index[i] = i;
            } else {
                index[i] = 0;
            }
        }
    }
}
void filter_lessorequal(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index){

    char save[1];
    char file_name[1025];
    size_t column = find_column(keeper, database_copy, filter);
    if(keeper->type[column] == 'N'){
        lessorequal_number(keeper, database_copy, column, value, index); 
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if((strcmp(save, "S") == 0) || (strcmp(save, "s") == 0)){
            printf("Entre com o nome do arquivo: \n");
            scanf("%s", file_name);
            database_filtered(keeper, database, index, file_name);

        }
        else if(strcmp(save, "N") == 0){
            exit(9);
        }
    } 
    else{
        printf("Erro: Coluna nao numerica. Escolha outro filtro.\n");
    }

}
//----------------------------------------------------------------------------------------------------------------------
void biggerorequal_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    float value_convert, database_value;
    value_convert = atof(value);
    for(size_t i = 0; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            database_value = atof(database_copy->data[i][column]);
            if(database_value >= value_convert){
                index[i] = i;
            } else {
                index[i] = 0;
            }
        }
    }
}
void filter_biggerorequal(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index){

    char save[1];
    char file_name[1025];
    size_t column = find_column(keeper, database_copy, filter);
    if(keeper->type[column] == 'N'){
        biggerorequal_number(keeper, database_copy, column, value, index);      
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if((strcmp(save, "S") == 0) || (strcmp(save, "s") == 0)){
            printf("Entre com o nome do arquivo: \n");
            scanf("%s", file_name);
            database_filtered(keeper, database, index, file_name);

        }
        else if(strcmp(save, "N") == 0){
            exit(9);
        }
    } 
    else{
        printf("Erro: Coluna nao numerica. Escolha outro filtro.\n");
    }

}
//----------------------------------------------------------------------------------------------------------------------
void equal_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    float value_convert, database_value;
    value_convert = atof(value);
    for(size_t i = 0; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            database_value = atof(database_copy->data[i][column]);
            if(database_value == value_convert){
                index[i] = i;
            } else {
                index[i] = 0;
            }
        }
    }
}
void equal_string(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    for(size_t i = 0; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            if(strcmp(database_copy->data[i][column], value) == 0){
                index[i] = i;
            } else {
                index[i] = 0;
            }
        }
    }
}
void filter_equal(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index){

    char save[1];
    char file_name[1025];
    size_t column = find_column(keeper, database_copy, filter);
    if(keeper->type[column] == 'N'){
        equal_number(keeper, database_copy, column, value, index);      
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if((strcmp(save, "S") == 0) || (strcmp(save, "s") == 0)){
            printf("Entre com o nome do arquivo: \n");
            scanf("%s", file_name);
            database_filtered(keeper, database, index, file_name);

        }
        else if((strcmp(save, "N") == 0) || (strcmp(save, "n") ) == 0){
            exit(9);
        }
    } 
    else{
        equal_string(keeper, database_copy, column, value, index);
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if((strcmp(save, "S") == 0) || (strcmp(save, "s") == 0)){
            printf("Entre com o nome do arquivo: \n");
            scanf("%s", file_name);
            database_filtered(keeper, database, index, file_name);

        }
        else if(strcmp(save, "N") == 0){
            exit(9);
        }
    }

}
//----------------------------------------------------------------------------------------------------------------------
void different_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    float value_convert, database_value;
    value_convert = atof(value);
    for(size_t i = 0; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            database_value = atof(database_copy->data[i][column]);
            if(database_value != value_convert){
                index[i] = i;
            } else {
                index[i] = 0;
            }
        }
    }
}
void different_string(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    for(size_t i = 0; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            if(strcmp(database_copy->data[i][column], value) != 0){
                index[i] = i;
            } else {
                index[i] = 0;
            }
        }
    }
}
void filter_different(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index){

    char save[1];
    char file_name[1025];
    size_t column = find_column(keeper, database_copy, filter);
    if(keeper->type[column] == 'N'){
        different_number(keeper, database_copy, column, value, index);      
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if((strcmp(save, "S") == 0) || (strcmp(save, "s") == 0)){
            printf("Entre com o nome do arquivo: \n");
            scanf("%s", file_name);
            database_filtered(keeper, database, index, file_name);

        }
        else if(strcmp(save, "N") == 0){
            exit(9);
        }
    } 
    else{
        different_string(keeper, database_copy, column, value, index);
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if((strcmp(save, "S") == 0) || (strcmp(save, "s") == 0)){
            printf("Entre com o nome do arquivo: \n");
            scanf("%s", file_name);
            database_filtered(keeper, database, index, file_name);

        }
        else if(strcmp(save, "N") == 0){
            exit(9);
        }
    }

}
