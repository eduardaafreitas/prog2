#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <dirent.h>
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

LBP *alloc_lbp(){
    LBP *lbp = (LBP *)malloc(sizeof(LBP));

    if(!lbp){
        fprintf(stderr, "Erro ao alocar LBP\n");
        exit(1);
    }

    for (int h = 0; h < 256; h++){
        lbp->histogram[h] = 0;
    }

    lbp->size = 0;
    
    return lbp;
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
    //le o cabecalho ignorando os comentarios:
    char comment[128];
    do {
        fgets(comment, 128, arq);
    } while( comment[0] == '#' );  
    sscanf(comment, "%s", img->type);

    do {
        fgets(comment, 128, arq);
    } while( comment[0] == '#' );
    sscanf(comment, "%d %d", &img->width, &img->height);

    do {
        fgets(comment, 128, arq);
    } while( comment[0] == '#' );
    sscanf(comment, "%d", &img->max_value);
    //----------------------------
    alloc_pixels(img);
    getc(arq); 

    //trata o tipo da imagem:
    if(img->type[1] == '5'){
        img = fill_pixels_p5(arq, img);
    } else if(img->type[1] == '2'){
        img = fill_pixels_p2(arq, img);
    } else {
        printf("Imagem sem tipo definido\n");
        exit(1);
    }

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


    int sum = 0;
    for(int lin = 0;lin < 3;lin++){
        for(int col = 0;col < 3;col++){
            sum += aux[lin][col];
        }
        //printf("\n");
    }

    //printf("%d\n", sum);      
    new->pixel[i][j] = sum;
    
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

void define_histogram(char *file_in, image *new, LBP *lbp){

    for (int i = 0; i < new->height; i++){
        for (int j = 0; j < new->width; j++){
            lbp->histogram[new->pixel[i][j]]++;
        }
    }

    char name[128];
    strcpy(name, file_in);
    strcat(name, ".lbp");

    FILE *histogram;

    histogram = fopen(name, "w");
    if(histogram == NULL){
        fprintf(stderr, "Erro ao abrir arquivo file_in\n");
        exit(1);
    }

    for (int h = 0; h < 256; h++){
        fprintf(histogram, "%d ", lbp->histogram[h]);
    }
    
    fclose(histogram);
}

void euclidian_distance(LBP *aux, LBP *lbp_origin, LBP *lbp_compare){

    float sum, square, distance;

    for(int h = 0; h < 256; h++){
        aux->histogram[h] = lbp_origin->histogram[h] - lbp_compare->histogram[h];
        square = (aux->histogram[h]) * 2;
        sum = square + sum;
    }

    distance = sqrt(sum);
    aux->size = distance;

}

void directory_read(char *directory_name){
    DIR *database;
    struct dirent *dir;
    char dir_path[256];
    char compare_dir[256];

    char *compare_pointer;

    strcpy(dir_path, directory_name);

    database = opendir(directory_name);

    if(!database){
        perror("Não foi possível abrir o diretorio.\n");
  	    exit(1);
    }
    
    dir = readdir(database);
    if(!dir){
        perror("Não foi possível acessar o diretorio.\n");
  	    exit(1);
    }

    while(dir){
        strcat(compare_dir, dir->d_name);
        compare_pointer = strstr(dir->d_name, compare_dir);
        if((dir->d_name[0] == '.') || compare_pointer ){
            continue;
        }
        printf("%s\n", dir->d_name);
        strcat(dir_path, dir->d_name);
        lbp_convert(dir_path);
        
        memset(dir_path, 0, strlen(dir_path));
        strcpy(dir_path, directory_name);
        dir = readdir(database);

    }
    closedir(database);
}

void lbp_convert(char file_in[256]){
    FILE *arq = NULL;
    arq = fopen(file_in, "r");
    if(arq == NULL){
        fprintf(stderr, "Erro ao abrir arquivo\n");
        exit(1);
    }
    
    //aloca estrutura:
    image *img_in = alloc_image();
    img_in = read_image(arq, img_in, file_in);

    image *new_img = alloc_image();
    new_img_init(img_in, new_img);
    lbp_generate(img_in, new_img);

    LBP *lbp = alloc_lbp();
    define_histogram(file_in, new_img, lbp);

    free_memory(img_in);
    free_memory(new_img);
    fclose(arq);


}

void lbp_convert_origin(char *file_in){
    printf("converteu\n");
    FILE *arq = NULL;
    arq = fopen(file_in, "r");
    if(arq == NULL){
        fprintf(stderr, "Erro ao abrir arquivo\n");
        free(file_in);
        exit(1);
    }
    
    //aloca estrutura:
    image *img_in = alloc_image();
    img_in = read_image(arq, img_in, file_in);

    image *new_img = alloc_image();
    new_img_init(img_in, new_img);
    lbp_generate(img_in, new_img);

    LBP *lbp = alloc_lbp();
    define_histogram(file_in, new_img, lbp);

    fclose(arq);

}



