#include "jpegdecoder.h"
#include <time.h>
#include <unistd.h>
unsigned char* readFile(char* file_name, int* data_size)
{
    FILE* fp;
    if ((fp = fopen(file_name, "rb")) == NULL)  //以二进制的方式打开文件
    {
        return NULL;
    }
    if (fseek(fp, 0, SEEK_END))  //跳过BITMAPFILEHEADE
    {
        return NULL;
    }
    *data_size = ftell(fp);
    if (fseek(fp, 0, 0))  //跳过BITMAPFILEHEADE
    {
        return NULL;
    }

    unsigned char* pBmpBuf = new unsigned char[*data_size];
    fread(pBmpBuf, sizeof(unsigned char), *data_size, fp);
    fclose(fp);   //关闭文件
    return pBmpBuf;
}

static inline int get_time(void)
{
    timespec now;

    clock_gettime(CLOCK_MONOTONIC, &now);

    return (int)now.tv_sec * 1000 + (int)now.tv_nsec / 1000000;

}

int main(int argc, char *argv[])
{
    int data_size;
    char file_str[200];
    sprintf(file_str, "1.jpg");
    unsigned char* data = readFile(file_str, &data_size);
    if (data == NULL) {
        printf("data is null");
        return 0;
    }
    JpegDecoder* jpeg_decoder = new JpegDecoder();
    int width, height;
    int start = (int)get_time() ;
    jpeg_decoder->begin_decode(data, data_size, &width, &height);
    unsigned char* dest = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 4);
    jpeg_decoder->decode(dest, width * 4);
    int end = (int)get_time() ;
    printf("decode the whole jpg cost time %d\n", end - start);
    free(dest);
    free(data);
    sprintf(file_str, "2.jpg");
    data = readFile(file_str, &data_size);
    if (data == NULL) {
        printf("data is null");
        return 0;
    }
    start = (int)get_time() ;
    int num = 1000;
    while (num--){
        jpeg_decoder->begin_decode(data, data_size, &width, &height);
        unsigned char* dest = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 4);
        jpeg_decoder->decode(dest, width * 4);
        free(dest);
    }
    end = (int)get_time() ;
    printf("decode fragment jpg cost time %d\n", end - start);
    free(data);
    delete jpeg_decoder;
    return 0;
}
