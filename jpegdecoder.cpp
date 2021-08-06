#include "jpegdecoder.h"

JpegDecoder::JpegDecoder()
{
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    cinfo.src = &jsrc;
    cinfo.src->init_source = init_source;
    cinfo.src->fill_input_buffer = fill_input_buffer;
    cinfo.src->skip_input_data = skip_input_data;
    cinfo.src->resync_to_restart = jpeg_resync_to_restart;
    cinfo.src->term_source = term_source;
}

JpegDecoder::~JpegDecoder()
{
    jpeg_destroy_decompress(&cinfo);
}

void JpegDecoder::begin_decode(unsigned char* data, int data_size, int* out_width, int* out_height)
{

    if (data == NULL || data_size == 0) {
        return;
    }

    if (data)
        jpeg_abort_decompress(&cinfo);

    data_size = data_size;

    cinfo.src->next_input_byte = data;
    cinfo.src->bytes_in_buffer = data_size;

    jpeg_read_header(&cinfo, TRUE);

    cinfo.out_color_space = JCS_RGB;
    width = cinfo.image_width;
    height = cinfo.image_height;

    *out_width = width;
    *out_height = height;
}

void JpegDecoder::convert_rgb_to_bgr(unsigned char* src, unsigned char* dest, int width)
{
    int x;

    for (x = 0; x < width; x++) {
        *dest++ = src[2];
        *dest++ = src[1];
        *dest++ = src[0];
        src += 3;
    }
}

void JpegDecoder::convert_rgb_to_bgrx(unsigned char* src, unsigned char* dest, int width)
{
    int x;

    for (x = 0; x < width; x++) {
        *dest++ = src[2];
        *dest++ = src[1];
        *dest++ = src[0];
        *dest++ = 0;
        src += 3;
    }
}

void JpegDecoder::decode(unsigned char* dest, int stride)
{
    unsigned char* scan_line = (unsigned char*)malloc(sizeof(unsigned char) * width * 3);
    converter_rgb_t converter = NULL;
    int row;

    converter = convert_rgb_to_bgrx;

    jpeg_start_decompress(&cinfo);

    for (row = 0; row < height; row++) {
        jpeg_read_scanlines(&cinfo, &scan_line, 1);
        converter(scan_line, dest, width);
        dest += stride;
    }
    free(scan_line);
    jpeg_finish_decompress(&cinfo);
}

void JpegDecoder::init_source(j_decompress_ptr cinfo)
{
}

boolean JpegDecoder::fill_input_buffer(j_decompress_ptr cinfo)
{
    return false;
}

void JpegDecoder::skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
    if (!(num_bytes < (long)cinfo->src->bytes_in_buffer)) {
        return;
    }

    cinfo->src->next_input_byte += num_bytes;
    cinfo->src->bytes_in_buffer -= num_bytes;
}

void JpegDecoder::term_source(j_decompress_ptr cinfo)
{
    return;
}
