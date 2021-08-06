#ifndef JPEGDECODER_H
#define JPEGDECODER_H
#define XMD_H
#include <stdio.h>
#include "stddef.h"
#include <stdlib.h>
#include "jpeglib.h"
class JpegDecoder
{
public:
    JpegDecoder();
    ~JpegDecoder();
    void begin_decode(unsigned char* data, int data_size, int* out_width, int* out_height);
    void decode(unsigned char* dest, int stride);

private:
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr         jerr;
    struct jpeg_source_mgr        jsrc;
    UINT8*    data;
    int       data_size;
    int       width;
    int       height;
    typedef void (*converter_rgb_t)(unsigned char* src, unsigned char* dest, int width);
    static void init_source(j_decompress_ptr cinfo);
    static boolean fill_input_buffer(j_decompress_ptr cinfo);
    static void skip_input_data(j_decompress_ptr cinfo, long num_bytes);
    static void term_source(j_decompress_ptr cinfo);
    static void convert_rgb_to_bgr(unsigned char* src, unsigned char* dest, int width);
    static void convert_rgb_to_bgrx(unsigned char* src, unsigned char* dest, int width);
};
#endif // JPEGDECODER_H
