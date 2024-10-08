#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "lbp.h"

int main( int argc, char *argv[] ) {
    int opt;
    FILE *arq = NULL;
    char *file_in = NULL;

    while ((opt = getopt(argc, argv, "i:")) != -1) {
        switch (opt) {
            case 'i':
                file_in = strdup(optarg);
                break;
            default:
                fprintf(stderr, "Forma de uso:\n");
                exit(1);
        }
    }   
  
    if (file_in == NULL){
        fprintf(stderr, "Erro ao abrir arquivo file_in\n");
        exit(1);
    }

    arq = fopen(file_in, "r");
    if(arq == NULL){
        fprintf(stderr, "Erro ao abrir arquivo\n");
        free(file_in);
        exit(1);
    }
    
    //aloca estrutura:
    image *img = alloc_image();

    img = read_image(arq, img, file_in);

    if (img == NULL) {
        fprintf(stderr, "Erro ao ler a imagem do arquivo %s\n", file_in);
        fclose(arq);
        free(file_in);
        exit(EXIT_FAILURE);
    }


    fclose(arq);
    free(file_in);

    return 0;
}