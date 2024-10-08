#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "lbp.h"


image *alloc_image(){ //aloca estrutura IMAGE
    image *img = (image *)malloc(sizeof(image));

    if (!img) {
        fprintf(stderr, "Erro ao alocar imagem\n");
        exit(1);
    }

    img->width = 0;
    img->height = 0;
    img->pixel = NULL;
    img->max_value = 0;
    img->type[0] = 'P';
    img->type[1] = '0';
    return img;
}

void alloc_pixels(image *img){ //aloca matriz de pixels
    img->pixel = (unsigned char **)malloc(img->height * sizeof(unsigned char *));
    if (!img->pixel) {
        fprintf(stderr, "Erro ao alocar pixel\n");
        exit(1);
    }

    for (int i = 0; i < img->height; i++) {
        img->pixel[i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
        if (!img->pixel[i]) {
            fprintf(stderr, "Erro ao alocar linha de pixels\n");
            exit(1);
        }
    }

}

image *fill_pixels_p5(FILE *arq, image *img){
    unsigned char temp_aux;
    for(int i = 0; i < img->height; i++){
        for(int j = 0; j < img->width; j++){
            fscanf(arq, "%c", &temp_aux);
            img->pixel[i][j] = temp_aux;
        }
    }
    return img;
}

image *read_image(FILE *arq, image *img, char *image_name){ 

    if (!arq) {
        fprintf(stderr, "Erro: arquivo de imagem não aberto\n");
        return NULL;
    }
    fscanf(arq, "%s", img->type);
    fscanf(arq, "%d %d", &img->width, &img->height);
    fscanf(arq, "%d", &img->max_value);

    alloc_pixels(img);
    getc(arq); 


    if ( ){
        printf("%d\n", );
        img = fill_pixels_p5(arq, img);
    }



    return img;
}



