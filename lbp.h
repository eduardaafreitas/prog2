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

image *read_image(FILE *arq, image *img, char *image_name);


#endif