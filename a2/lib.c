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

    if ((token[0] >= '0' && token[0] <= '9') || (token[0] == '-') || (token[0] == '+')){
        return 1;
    }
    else{
        return 2;
    }
}

void type(csv *keeper, base *database){
    
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


char* put_spaces(size_t size, size_t diff, char *origin_string){
    char *spaces = (char *)malloc((size + 1) * sizeof(char));
    memset(spaces, ' ', diff);
    strcat(spaces, origin_string);
    spaces[size] = '\0';
    return spaces;

}

void fill_string(csv *keeper, base *database){

    for (int i = 0; i < keeper->row; i++){ //percorre linhas
        for (int j = 0; j < keeper->column; j++){ //percorre colunas  
            if(((database->data[i][j] != NULL)) && (strlen (database->data[i][j]) < keeper->sizes[j])){
                size_t diff = keeper->sizes[j] - strlen(database->data[i][j]);
                char *spaces = put_spaces(keeper->sizes[j], diff, database->data[i][j]);
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

size_t count_indexsize(csv *keeper, size_t *index){
    size_t count = 0;
    for (size_t i = 1; i < keeper->row; i++){
        if (index[i] == i){
            count++;
        }
    }
    return count;
}

void database_filtered(csv *keeper, base *database, size_t *index, char *file_name){

    FILE *archive = fopen(file_name, "w");
    if(!archive){
        fprintf(stderr, "Erro ao abrir arquivo. err: database_filtered\n");
        exit(9);
    }

    for(size_t i = 0; i < keeper->row; i++){
        if(index[i] == i){
            for(size_t j = 0; j < keeper->column; j++){
                fprintf(archive, "%s", database->data[i][j]);
                if (j < (keeper->column - 1)) {
                    fprintf(archive, ",");
                }
            }
        }
        if (i < (keeper->row - 1) && index[i] != -1){
            fprintf(archive, "\n");
        }
            
    }
}

void show_filter(csv *keeper, base *database_copy, size_t *index){
    
    fill_string(keeper, database_copy);

    for (int i = 0; i < 5; i++){
        if (index[i] != -1){
            for (int j = 0; j < keeper->column; j++){
                printf("%10s ", database_copy->data[i][j]);  
            }
            printf("\n");
        }
    }

    // for (int i = 0; i < (keeper->column+1); i++){
    //     printf("%s ", "...");
    // }

    printf("\n\n");

    for (int i = (keeper->row - 5); i < keeper->row; i++){
        for (int j = 0; j < keeper->column; j++){
            if(database_copy->data[i][j] != NULL)
                printf("%10s ", database_copy->data[i][j]);
        }
        printf("\n");
    }
    printf(" [%lu row x %lu column] \n\n", keeper->row, keeper->column);
}


//essa função converte o valor numerico do filtro para float e compara com os valores da coluna
//caso atenda as condições, marca o index da linha
void bigger_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    float value_convert, database_value;
    value_convert = atof(value);
    for(size_t i = 1; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            database_value = atof(database_copy->data[i][column]);
            //printf("database_value[%lu] %f\n", i, database_value);
            if(database_value > value_convert){
                index[i] = i;
            }
        }
    }
}
void filter_bigger(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index){

    char save[1];
    char file_name[1025];
    //size_t indexsize;
    size_t column = find_column(keeper, database_copy, filter);
    if(keeper->type[column] == 'N'){
        bigger_number(keeper, database_copy, column, value, index);
        //indexsize = count_indexsize(keeper, index);        
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if(strcmp(save, "S") == 0){
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

void smaller_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    float value_convert, database_value;
    value_convert = atof(value);
    for(size_t i = 1; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            database_value = atof(database_copy->data[i][column]);
            //printf("database_value[%lu] %f\n", i, database_value);
            if(database_value < value_convert){
                index[i] = i;
            }
        }
    }
}
void filter_smaller(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index){

    char save[1];
    char file_name[1025];
    //size_t indexsize;
    size_t column = find_column(keeper, database_copy, filter);
    if(keeper->type[column] == 'N'){
        smaller_number(keeper, database_copy, column, value, index);
        //indexsize = count_indexsize(keeper, index);        
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if(strcmp(save, "S") == 0){
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

void lessorequal_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    float value_convert, database_value;
    value_convert = atof(value);
    for(size_t i = 1; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            database_value = atof(database_copy->data[i][column]);
            //printf("database_value[%lu] %f\n", i, database_value);
            if(database_value <= value_convert){
                index[i] = i;
            }
        }
    }
}
void filter_lessorequal(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index){

    char save[1];
    char file_name[1025];
    //size_t indexsize;
    size_t column = find_column(keeper, database_copy, filter);
    if(keeper->type[column] == 'N'){
        lessorequal_number(keeper, database_copy, column, value, index);
        //indexsize = count_indexsize(keeper, index);        
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if(strcmp(save, "S") == 0){
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

void biggerorequal_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    float value_convert, database_value;
    value_convert = atof(value);
    for(size_t i = 1; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            database_value = atof(database_copy->data[i][column]);
            //printf("database_value[%lu] %f\n", i, database_value);
            if(database_value >= value_convert){
                index[i] = i;
            }
        }
    }
}
void filter_biggerorequal(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index){

    char save[1];
    char file_name[1025];
    //size_t indexsize;
    size_t column = find_column(keeper, database_copy, filter);
    if(keeper->type[column] == 'N'){
        biggerorequal_number(keeper, database_copy, column, value, index);
        //indexsize = count_indexsize(keeper, index);        
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if(strcmp(save, "S") == 0){
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

void equal_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    float value_convert, database_value;
    value_convert = atof(value);
    for(size_t i = 1; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            database_value = atof(database_copy->data[i][column]);
            //printf("database_value[%lu] %f\n", i, database_value);
            if(database_value == value_convert){
                index[i] = i;
            }
        }
    }
}
void filter_equal(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index){

    char save[1];
    char file_name[1025];
    //size_t indexsize;
    size_t column = find_column(keeper, database_copy, filter);
    if(keeper->type[column] == 'N'){
        equal_number(keeper, database_copy, column, value, index);
        //indexsize = count_indexsize(keeper, index);        
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if(strcmp(save, "S") == 0){
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

void different_number(csv *keeper, base *database_copy, size_t column, char *value, size_t *index){
    float value_convert, database_value;
    value_convert = atof(value);
    for(size_t i = 1; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL){    
            database_value = atof(database_copy->data[i][column]);
            //printf("database_value[%lu] %f\n", i, database_value);
            if(database_value != value_convert){
                index[i] = i;
            }
        }
    }
}
void filter_different(base *database, csv *keeper, base *database_copy, char *filter, char *value, size_t *index){

    char save[1];
    char file_name[1025];
    //size_t indexsize;
    size_t column = find_column(keeper, database_copy, filter);
    if(keeper->type[column] == 'N'){
        different_number(keeper, database_copy, column, value, index);
        //indexsize = count_indexsize(keeper, index);        
        show_filter(keeper, database_copy, index);
        printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: \n");
        scanf("%1s", save);
        if(strcmp(save, "S") == 0){
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
