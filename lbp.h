#ifndef __LBP__
#define __LBP__

struct LBP {
    int *histogram;
    int size;
};
typedef struct LBP LBP;

struct image {
    int width;
    int height;
    int max_value;
    char type[2];
    unsigned char **pixel;
};
typedef struct image image;

image *alloc_image();
void alloc_pixels(image *img);

image *fill_pixels_p5(FILE *arq, image *img);
image *fill_pixels_p2(FILE *arq, image *img);

image *read_image(FILE *arq, image *img, char *image_name);

void new_img_init(image *img, image *new);
void math(image *img, image *aux, image *new, int i, int j);
void lbp_generate(image *img, image *new);

void out_img_generate(image *new, FILE *arq_out);

void free_memory(image *img);

#endif