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
    FILE *arq = NULL;
    char *file_in = NULL;
    //char *file_out = NULL;
    char *directory = NULL;

    while ((opt = getopt(argc, argv, "d:i:")) != -1) {
        switch (opt) {
            case 'd':
                directory = strdup(optarg);
                break;
            case 'i':
                file_in = strdup(optarg);
                break;
            // case 'o':
            //     file_out = strdup(optarg);
            //     break;
            default:
                fprintf(stderr, "Forma de uso:\n");
                exit(1);
        }
    }   
    

    //lbp_convert_origin(file_in);
    
    
    arq = fopen(file_in, "r");
    if(arq == NULL){
        fprintf(stderr, "Erro ao abrir arquivo\n");
        free(file_in);
        exit(1);
    }
    
    //aloca estrutura:
    image *img_in = alloc_image();
    img_in = read_image(arq, img_in, file_in);
    
    image *new_img_in = alloc_image();
    new_img_init(img_in, new_img_in);

    lbp_generate(img_in, new_img_in);

    LBP *lbp_origin = alloc_lbp();
    double distance = 0.0;
    char shorter_distance[256];
    
    define_histogram(file_in, new_img_in, lbp_origin);

    fclose(arq);    

    directory_read(directory);

    euclidian_distance(directory, lbp_origin, distance, shorter_distance);

    printf("arquivo mais parecido: %s\n", shorter_distance);
    printf("distancia: %.6f\n",distance);
    

    free(file_in);
    free(directory);
    free(lbp_origin);
    free_memory(img_in);
    free_memory(new_img_in);

    return 0;
}