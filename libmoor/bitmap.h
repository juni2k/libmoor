//
// Created by mfb on 2/19/22.
//

#ifndef LIBMOOR_BITMAP_H
#define LIBMOOR_BITMAP_H

#include <stdint.h>

/* Bitmap file header -- 14 bytes */
typedef struct {
    uint16_t magic_number; /* 'BM' */
    uint32_t size; /* whole file size in bytes */
    uint16_t reserved0; /* can be 0 */
    uint16_t reserved1; /* can be 0 */
    uint32_t pixel_array_offset; /* pixel array offset */
} moor_bitmap_file_header_t;

/* DIB header (Windows-compatible BITMAPINFOHEADER -- 40 bytes) */
typedef struct {
    uint32_t size; /* size of this header, must be 40 */
    int32_t width; /* bitmap width in px (signed) */
    int32_t height; /* bitmap size in px (signed) */
    uint16_t planes; /* number of color planes, must be 1 */
    uint16_t bpp; /* number of bits per pixel (e.g. 8, 16, 32) */
    uint32_t compression; /* compression method used, 0 for none/BI_RGB */
    uint32_t pixel_array_size; /* raw bitmap data size, 0 is OK for BI_RGB */
    int32_t horizontal_ppm; /* horizontal resolution, pixel per metre (signed) */
    int32_t vertical_ppm; /* vertical resolution, pixel per metre (signed) */
    uint32_t palette_size; /* number of colors in palette, 0 for 2^n where n is bpp */
    uint32_t important_color_count; /* number of important colors, ignored, 0 is ok */
} moor_bitmap_info_header_t;

/* RGBA [4 bytes] * 256 [entries] = 1024 bytes */
typedef uint8_t moor_bitmap_color_table_t[1024];

typedef struct {
    moor_bitmap_file_header_t file_header;
    moor_bitmap_info_header_t info_header;
    moor_bitmap_color_table_t color_table;
    uint8_t *pixel_array; /* memory-managed by user */
} moor_bitmap_t;

void moor_bitmap_init(moor_bitmap_t *bitmap);
uint32_t moor_bitmap_total_size(uint32_t pixel_count);
void moor_bitmap_set_color_table(moor_bitmap_t *bitmap, uint8_t *buf);
void moor_bitmap_set_pixel_array(moor_bitmap_t *bitmap, uint8_t *buf, uint32_t size);
void moor_bitmap_write(moor_bitmap_t *bitmap, uint8_t *buf);

#endif //LIBMOOR_BITMAP_H
