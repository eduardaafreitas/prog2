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

image *alloc_image(){ //aloca estrutura IMAGE
    image *img = (image *)malloc(sizeof(image));

    if (!img) {
        fprintf(stderr, "Erro ao alocar imagem.\n");
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
        fprintf(stderr, "Erro ao alocar LBP.\n");
        exit(2);
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
        fprintf(stderr, "Erro ao alocar pixel.\n");
        exit(3);
    }

    for (int i = 0; i < img->height; i++) {
        img->pixel[i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
        if (!img->pixel[i]) {
            fprintf(stderr, "Erro ao alocar linha de pixels.\n");
            exit(4);
        }
        for (int j = 0; j < img->width; j++) {
            img->pixel[i][j] = 0;  // Ou um valor padrão apropriado
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
        fprintf(stderr, "Erro: arquivo de imagem não aberto para leitura.\n");
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
        printf("Imagem sem tipo definido.\n");
        exit(5);
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
        fprintf(stderr, "Erro ao alocar nova struct image.\n");
        exit(6);
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
        }
    }

    int mult = 1;
    for(int lin = 0;lin < 3;lin++){
        for(int col = 0;col < 3;col++){
            if ( lin == 1 && col == 1) {
                aux[lin][col] = 0;
            } else {
                aux[lin][col] *= mult;
                mult *= 2;
            }
        }
    }


    int sum = 0;
    for(int lin = 0;lin < 3;lin++){
        for(int col = 0;col < 3;col++){
            sum += aux[lin][col];
        }
    }
    
    new->pixel[i][j] = sum;
    
}

void lbp_generate(image *img, image *new){

    for(int i = 1;i < ((img->height)-1);i++){
        for(int j = 1;j < ((img->width)-1);j++){
            math(img, new, i, j);
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

void define_histogram(char *file_in, image *new, LBP *lbp){

    for (int i = 0; i < 256; i++) {
        lbp->histogram[i] = 0;
    }

    for (int i = 0; i < new->height; i++){
        for (int j = 0; j < new->width; j++){
            lbp->histogram[new->pixel[i][j]]++;
        }
    }
    
    char name[128];
    for(int n = 0; n < 128; n++){
        name[n] = '0';
    }
    strcpy(name, file_in);
    strcat(name, ".lbp");

    FILE *histogram_file;

    histogram_file = fopen(name, "w");
    if(histogram_file == NULL){
        fprintf(stderr, "Erro ao abrir arquivo de histograma.\n(Function: define_histogram)\n");
        exit(7);
    }

    for (int h = 0; h < 256; h++){
        fprintf(histogram_file, "%d ", lbp->histogram[h]);
    }
    
    fclose(histogram_file);
}

void euclidian_math(LBP *aux, LBP *lbp_origin, LBP *lbp_compare){

    double sum = 0.0;
    double square = 0.0;

    for(int h = 0; h < 256; h++){
        aux->histogram[h] = pow(lbp_origin->histogram[h] - lbp_compare->histogram[h], 2);
        square = aux->histogram[h];
        sum = square + sum;
    }

    if (sum == 0) {
        aux->size = 0;
    } else {
        aux->size = sqrt(sum);
    }

}

LBP *read_lbp_struct(FILE *arquivo_lbp, LBP *aux){
    int temp_aux = 0;
    if(!arquivo_lbp){
        fprintf(stderr, "Erro: arquivo lbp não aberto.\n(Function: read_lbp_struct)\n");
        return NULL;
    }
    for(int h = 0; h < 256; h++){
        fscanf(arquivo_lbp, "%d",&temp_aux);
        aux->histogram[h] = temp_aux;
    }
    return aux;
}


void euclidian_distance(char *directory_name, LBP *lbp_origin, double distance, char shorter_distance[256]){

    DIR *database;
    FILE* arquivo;
    struct dirent *dir;
    char dir_path[256];

    char *compare_pointer;

    strcpy(dir_path, directory_name);
    database = opendir(directory_name);

    LBP *compare = alloc_lbp();
    LBP *aux = alloc_lbp();
    if(!database){
        perror("Não foi possível abrir o diretorio.\n(Function: euclidian_distance)\n");
  	    exit(8);
    }
    
    dir = readdir(database);
    if(!dir){
        perror("Não foi possível acessar o diretorio.\n(Function: euclidian_distance)\n");
  	    exit(9);
    }
    
    while(dir){
        compare_pointer = strstr(dir->d_name, ".lbp");
        if((dir->d_name[0] == '.') || (!compare_pointer) ){
            dir = readdir(database);
            continue;
        }
        strcat(dir_path, dir->d_name);

        arquivo = fopen(dir_path, "r");
        if(!arquivo){
            perror("Não foi possível abrir o arquivo do diretorio.\n(Function: euclidian_distance)\n");
            exit(10);
        }
        
        compare = read_lbp_struct(arquivo, compare);
        euclidian_math(aux, lbp_origin, compare);
        compare->size = aux->size;

        if(compare->size < distance){
            distance = compare->size; 
            strcpy(shorter_distance, dir->d_name);
        }
        

        memset(dir_path, 0, strlen(dir_path));
        strcpy(dir_path, directory_name);
        dir = readdir(database);
        fclose(arquivo);

    }
    char *ext = strstr(shorter_distance, ".lbp");
    if (ext != NULL) {
        *ext = '\0'; 
    }

    printf("Imagem mais similar: %s %.6f\n", shorter_distance, distance);


    closedir(database);
    free(aux);
    free(compare);

}

void directory_read(char *directory_name){
    DIR *database;
    struct dirent *dir;
    char dir_path[256];

    char *compare_pointer = NULL;

    strcpy(dir_path, directory_name);

    database = opendir(directory_name);

    if(!database){
        perror("Não foi possível abrir o diretorio.\n(Function: directoru_read)\n");
  	    exit(11);
    }
    
    dir = readdir(database);
    if(!dir){
        perror("Não foi possível acessar o diretorio.\n(Function: directoru_read)\n");
  	    exit(12);
    }

    while(dir){
        compare_pointer = strstr(dir->d_name, ".lbp");
        if((dir->d_name[0] == '.') || compare_pointer ){
            dir = readdir(database);
            continue;
        }
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
        fprintf(stderr, "Erro ao abrir arquivo.\n(Function: lbp_convert)\n");
        exit(13);
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
    free(lbp);
    fclose(arq);


}

void free_memory(image *img){
    
    for(int i = 0; i < img->height; i++){
        free(img->pixel[i]);
    }
    free(img->pixel);

    free(img);
}

