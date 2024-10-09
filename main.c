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
    char *file_out = NULL;

    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i':
                file_in = strdup(optarg);
                break;
            case 'o':
                file_out = strdup(optarg);
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

    image *new = alloc_image();
    new_img_init(img, new);

    lbp_generate(img, new);

    FILE *arq_out;
    if(!( arq_out = fopen(file_out, "wb"))){
      fprintf(stderr, "Nao foi possivel abrir o arquivo!\n");
      exit(1);
    }

    out_img_generate(new, arq_out);
    
    fclose(arq);
    free(file_in);

    return 0;
}