//Feito por Eduarda de Aguiar Freitas, GRR 20211799.

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
    char *file_out = NULL;
    char *directory = NULL;
    int flag_out = 0;

    while ((opt = getopt(argc, argv, "d:i:o:")) != -1) {
        switch (opt) {
            case 'd':
                directory = strdup(optarg);
                break;
            case 'i':
                file_in = strdup(optarg);
                break;
            case 'o':
                flag_out = 1;
                file_out = strdup(optarg);
                break;
            default:
                fprintf(stderr, "Forma de uso: ./LBP -d ./database -i img_in.pgm -o img_out.pgm\n");
                exit(1);
        }
    }   

    if(!directory){
        fprintf(stderr, "Forma de uso: ./LBP -d ./database -i img_in.pgm -o img_out.pgm\n");
        exit(1);
    }
    if(!file_in){
        fprintf(stderr, "Forma de uso: ./LBP -d ./database -i img_in.pgm -o img_out.pgm\n");
        exit(1);
    }

    //Abre arquivo da "imagem original:"
    arq = fopen(file_in, "r");
    if(arq == NULL){
        fprintf(stderr, "Erro ao abrir arquivo\n");
        free(file_in);
        exit(1);
    }
    
    //aloca estrutura da "imagem original", gera a nova imagem convertida:
    image *img_in = alloc_image();
    img_in = read_image(arq, img_in, file_in);
    
    image *new_img_in = alloc_image();
    new_img_init(img_in, new_img_in);
    lbp_generate(img_in, new_img_in);

    //gera imagem lbp de saida somente se o user solicitar
    if(flag_out){
        FILE *arq_out;
        if(!( arq_out = fopen(file_out, "wb"))){
            fprintf(stderr, "Nao foi possivel abrir o arquivo!\n");
            exit(1);
        }
        out_img_generate(new_img_in, arq_out);
    }
    
    //gera histograma da "imagem original" e fecha o arquivo de entrada da "imagem original":
    LBP *lbp_origin = alloc_lbp();
    define_histogram(file_in, new_img_in, lbp_origin);
    fclose(arq);    

    //percorre diretorio do banco de dados e retorna o nome do arquivo mais parecido e a distancia euclidiana:
    double distance = 1e12;
    char shorter_distance[256];
    directory_read(directory);
    euclidian_distance(directory, lbp_origin, distance, shorter_distance);

    free(file_in);
    free(file_out);
    free(directory);
    free(lbp_origin);
    free_memory(img_in);
    free_memory(new_img_in);

    return 0;
}