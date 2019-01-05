#include "./readbitmap.h"

int fileType, img_width, img_height;
unsigned int fileSize;
unsigned int pixelPos;
unsigned int bitCount;

BmpInfo * readBmp(char * path) {
    FILE * bmpfile = fopen(path, "rb");
    if (bmpfile == NULL) oops("can not open file");
    readBmpHeader(bmpfile);
    readBmpInfoHeader(bmpfile);
    if (bitCount == 24) {
        BmpInfo * info = (BmpInfo *) calloc(1, sizeof(BmpInfo));
        uint64_t size = img_height * img_width * 3;
        uint8_t * data = getRgbData(bmpfile);
        info->img_height = img_height;
        info->img_width = img_width;
        info->rgbData = data;
        info->type = 24;
        return info;
    } else if (bitCount == 32) {
        BmpInfo * info = (BmpInfo *) calloc(1, sizeof(BmpInfo));
        uint8_t * data = getRgbaData(bmpfile);
        info->img_height = img_height;
        info->img_width = img_width;
        info->type = 32;
        info->rgbData = data;
        return info;
    }
    return NULL;
}

void readBmpHeader(FILE * bmpfile) {
    unsigned char * p = (unsigned char *) calloc(BIT_HEADER_SIZE, sizeof(unsigned char));
    size_t readsize = fread(p, sizeof(unsigned char), BIT_HEADER_SIZE, bmpfile);
    if (readsize != BIT_HEADER_SIZE) oops("header size error");
    fileType = (p[0] << 8) | p[1];
    fileSize = (p[5] << 24) | (p[4] << 16) | (p[3] << 8) | p[2];
    pixelPos = (p[13] << 24) | (p[12] << 16) | (p[11] << 8) | p[10];
    printf("fileType: %x, fileSize: %u, pixelPos: %u\n", fileType, fileSize, pixelPos);
    free(p);
}

void readBmpInfoHeader(FILE * bmpfile) {
    unsigned char * infoSize = (unsigned char *) calloc(BMP_HEADER_SIZE_SIZE, sizeof(unsigned char));
    size_t rs = fread(infoSize, sizeof(unsigned char), BMP_HEADER_SIZE_SIZE, bmpfile);
    if (rs != BMP_HEADER_SIZE_SIZE) oops("info size error");
    int info_size = (infoSize[3] << 24) | (infoSize[2] << 16) | (infoSize[1] << 8) | infoSize[0];
    int remainInfoSize = info_size - BMP_HEADER_SIZE_SIZE;
    unsigned char * info = (unsigned char *) calloc(remainInfoSize, sizeof(unsigned char));
    size_t rsi = fread(info, sizeof(unsigned char), remainInfoSize, bmpfile);
    if (rsi != remainInfoSize) oops("info size error");
    img_width = (info[3] << 24) | (info[2] << 16) | (info[1] << 8) | info[0];
    img_height = (info[7] << 24) | (info[6] << 16) | (info[5] << 8) | info[4];
    bitCount = (info[11] << 8) | info[10];
    int biCompress = (info[15] << 24) | (info[14] << 16) | (info[13] << 8) | info[12];
    int colors = (info[31] << 24) | (info[30] << 16) | (info[29] << 8) | info[28];
    printf("width: %d, height: %d, bitCount: %d, biCompress: %d, colors: %d\n", img_width, img_height, bitCount, biCompress, colors);
}

uint8_t * getRgbData(FILE * bmpfile) {
    int size = img_width * img_height * 3;
    uint8_t * rgbData = (uint8_t *) calloc(size, sizeof(uint8_t));
    if (rgbData == NULL) oops("malloc failed");
    size_t readsize = fread(rgbData, sizeof(uint8_t), size, bmpfile);
    if (readsize != size) {
        free(rgbData);
        oops("read rgb data failed");
    }

    return rgbData;
}


uint8_t * getRgbaData(FILE * bmpfile) {
    uint32_t size = img_width * img_height * 4;
    uint8_t * data = (uint8_t *) calloc(size, sizeof(uint8_t));
    size_t rd = fread(data, sizeof(uint8_t), size, bmpfile);
    if (rd != size) oops("read size error");
    /*
    for (int i = 0; i < size; i += 4) {
        printf("r = %u, g = %u, b = %u, a = %u\n", data[i], data[i + 1], data[i + 2], data[i + 3]);
    }
    */
    return data;
}