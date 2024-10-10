#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <dirent.h>
#include "lbp.h"

int main( int argc, char *argv[] ) {
    int opt;
    FILE *arq_in = NULL;
    char *file_in = NULL;
    char *file_out = NULL;
    char *directory = NULL;

    while ((opt = getopt(argc, argv, "d:i:o:")) != -1) {
        switch (opt) {
            case 'd':
                directory = strdup(optarg);
                break;
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
    

    arq_in = fopen(file_in, "r");
    if(arq_in == NULL){
        fprintf(stderr, "Erro ao abrir arquivo\n");
        free(file_in);
        exit(1);
    }
    
    //aloca estrutura:
    printf("alocando estrutura inicial da img principal\n");
    image *img_in = alloc_image();
    img_in = read_image(arq_in, img_in, file_in);
    image *new_img = alloc_image();
    new_img_init(img_in, new_img);
    lbp_generate(img_in, new_img);

    LBP *lbp_arq_in = alloc_lbp();
    define_histogram(file_in, new_img, lbp_arq_in);

    fclose(arq_in);
    printf("img alocada e definida\n");

    //lbp_convert_origin(file_in);
    
    
    directory_read(directory, lbp_arq_in);

    // FILE *arq_out;
    // if(!( arq_out = fopen(file_out, "wb"))){
    //   fprintf(stderr, "Nao foi possivel abrir o arquivo!\n");
    //   exit(1);
    // }

    // out_img_generate(new, arq_out);
    
    fclose(arq_in);
    free(file_in);

    return 0;
}