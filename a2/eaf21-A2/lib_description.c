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


float media_describe(base *database, csv *keeper, base *database_copy, size_t column){
    float database_description = 0;
    float counter = 0;
    float sum = 0;
    for(size_t i = 1; i < keeper->row; i++){
        if (database_copy->data[i][column] != NULL && database_copy->data[i][column][0] != '0'){    
            database_description = atof(database_copy->data[i][column]);
            sum += database_description;
            counter++;
            
        }
    }
    float media = sum / counter;

    return media;
}

float desvio_padrao(base *database, csv *keeper, base *database_copy, size_t column){
    float media = media_describe(database, keeper, database_copy, column);
    float counter = 0;
    float sum_square = 0;
    for(size_t i = 1; i < keeper->row; i++) {
        if (database_copy->data[i][column] != NULL && database_copy->data[i][column][0] != '0') {    
            float database_description = atof(database_copy->data[i][column]);
            float diff = database_description - media;
            sum_square += diff * diff;
            counter++;
        }
    }
    float variancia = sum_square / counter;
    float desvio = sqrt(variancia);
    return desvio;
}
