#ifndef __LBP__
#define __LBP__

struct LBP {
    int histogram[256];
    char histogram_name[128];
    double size;
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
LBP *alloc_lbp();
void alloc_pixels(image *img);

image *fill_pixels_p5(FILE *arq, image *img);
image *fill_pixels_p2(FILE *arq, image *img);

image *read_image(FILE *arq, image *img, char *image_name);

void new_img_init(image *img, image *new);
void print_matriz(image *img);
void math(image *img, image *new, int i, int j);
void lbp_generate(image *img, image *new);

void out_img_generate(image *new, FILE *arq_out);

void define_histogram(char *file_in, image *img, LBP *lbp);
void euclidian_distance(LBP *aux, LBP *lbp_origin, LBP *lbp_compare);
void directory_read(char *directory_name, LBP *lbp_arq_in);
void lbp_convert(char file_in[256], LBP *lpb_arq_in);
void lbp_convert_origin(char *file_in);

void free_memory(image *img);
#endif