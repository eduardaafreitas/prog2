#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "lbp.h"
#include <stdint.h>

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

image *fill_pixels_p2(FILE *arq, image *img){
    int temp_aux;
    for(int i = 0; i < img->height; i++){
        for(int j = 0; j < img->width; j++){
            fscanf(arq, "%d", &temp_aux);
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

    if(img->type[1] == '5'){
        img = fill_pixels_p5(arq, img);
    } else if(img->type[1] == '2'){
        img = fill_pixels_p2(arq, img);
    } else {
        printf("Imagem sem tipo definido\n");
        exit(1);
    }
    printf("img: \n");
    print_matriz(img);

    return img;
}

void new_img_init(image *img, image *new){

    strcpy((new->type), img->type);
    new->width = img->width;
    new->height = img->height;
    new->max_value = img->max_value;

    alloc_pixels(new);

    if(!new){
        fprintf(stderr, "Erro ao alocar nova struct image\n");
        exit(1);
    }

}

void print_matriz(image *img){
    for(int i = 0;i < 3;i++){
        for(int j = 0;j < 3;j++){
            printf("%d ", img->pixel[i][j]);
        }
        printf("\n");
    }
}

void math(image *img, image *new, int i, int j){

    int aux[3][3];


    for(int lin = 0;lin < 3;lin++){
        for(int col = 0;col < 3;col++){
            if(img->pixel[(lin+i)-1][(col+j)-1] >= img->pixel[i][j])
                aux[lin][col] = 1;
            else
                aux[lin][col] = 0;
            //printf("%d ", aux[lin][col]);
        }
        //printf("\n");
    }

    //printf("\n\n");
    int mult = 1;
    for(int lin = 0;lin < 3;lin++){
        for(int col = 0;col < 3;col++){
            if ( lin == 1 && col == 1) {
                aux[lin][col] = 0;
            } else {
                aux[lin][col] *= mult;
                //printf("%d ", aux[lin][col]);
                mult *= 2;
            }
        }
        //printf("\n");
    }
    //printf("\n\n");


    int soma = 0;
    for(int lin = 0;lin < 3;lin++){
        for(int col = 0;col < 3;col++){
            soma += aux[lin][col];
        }
        //printf("\n");
    }

    //printf("%d\n", soma);      
    new->pixel[i][j] = soma;
    
}

void lbp_generate(image *img, image *new){

    for(int i = 1;i < ((img->height)-1);i++){
        for(int j = 1;j < ((img->width)-1);j++){
            math(img, new, i, j);
            //exit(0);
        }
    }

}

void out_img_generate(image *new, FILE *arq_out){

    fprintf(arq_out, "P%c\n", new->type[1]);
    fprintf(arq_out, "%d %d\n", new->width, new->height);
    fprintf(arq_out,"%d", new->max_value);

    fwrite("\n", sizeof(char), 1, arq_out);
    for(int i = 0; i < new->height; i++){
        for(int j = 0; j < new->width; j++){
            fprintf(arq_out, "%d ", new->pixel[i][j]);
        }
    }

    fclose(arq_out);

}



void free_memory(image *img){
    
    for(int i = 0; i < img->height; i++){
        free(img->pixel[i]);
    }
    free(img->pixel);

    free(img);
}