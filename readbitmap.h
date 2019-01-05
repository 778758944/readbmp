#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define oops(m) {perror(m); exit(1);}
#define BIT_HEADER_SIZE 14
#define BMP_HEADER_SIZE_SIZE 4

typedef struct bmpInfo {
    uint8_t type;
    int img_width;
    int img_height;
    uint8_t * rgbData;
} BmpInfo;


BmpInfo * readBmp(char * path);

void readBmpHeader(FILE * bmpfile);
void readBmpInfoHeader(FILE * bmpfile);
uint8_t * getRgbData(FILE * bmpfile);
uint8_t * getRgbaData(FILE * bmpfile);