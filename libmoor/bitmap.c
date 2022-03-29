//
// Created by mfb on 2/19/22.
//

#include <string.h>
#include <stdio.h>
#include "bitmap.h"

void moor_bitmap_init(moor_bitmap_t *bitmap,
                      int32_t const width, int32_t const height) {
    bitmap->file_header.magic_number = 'MB';
    bitmap->file_header.size = 0xDEAD;
    bitmap->file_header.reserved0 = 0;
    bitmap->file_header.reserved1 = 0;
    bitmap->file_header.pixel_array_offset = 0xDEAD;

    bitmap->info_header.size = sizeof(moor_bitmap_info_header_t);
    bitmap->info_header.width = width;
    bitmap->info_header.height = -height;
    bitmap->info_header.planes = 1;
    bitmap->info_header.bpp = 8;
    bitmap->info_header.compression = 0;
    bitmap->info_header.pixel_array_size = 0xDEAD;
    bitmap->info_header.horizontal_ppm = 0;
    bitmap->info_header.vertical_ppm = 0;
    bitmap->info_header.palette_size = 0;
    bitmap->info_header.important_color_count = 0;
}

uint32_t moor_bitmap_total_size(uint32_t pixel_count) {
    return 14 + 40 + 1024 + pixel_count;
}

static uint32_t moor_bitmap_pixel_array_offset() {
    return 14 + 40 + 1024;
}

void moor_bitmap_set_color_table(moor_bitmap_t *bitmap, uint8_t *buf) {
    memcpy(bitmap->color_table, buf, 1024);

    /* The table is in RGBA, but the BMP format expects BGRA.
     * Let's fix that! */
    for (int i = 0; i < 256; i++) {
        uint8_t *quad = &bitmap->color_table[i * 4];

        uint8_t red = *quad;
        *quad = *(quad+2);
        *(quad+2) = red;
    }
}

void moor_bitmap_set_pixel_array(moor_bitmap_t *bitmap, uint8_t *buf, uint32_t size) {
    bitmap->file_header.size = moor_bitmap_total_size(size);
    bitmap->file_header.pixel_array_offset = moor_bitmap_pixel_array_offset();

    bitmap->info_header.pixel_array_size = size;

    bitmap->pixel_array = buf;
}

void moor_bitmap_write(moor_bitmap_t *bitmap, uint8_t *buf) {
    fprintf(stderr, "copying file header\n");
    memcpy(&buf[0], &bitmap->file_header.magic_number, 2);
    memcpy(&buf[2], &bitmap->file_header.size, 4);
    memcpy(&buf[6], &bitmap->file_header.reserved0, 2);
    memcpy(&buf[8], &bitmap->file_header.reserved1, 2);
    memcpy(&buf[10], &bitmap->file_header.pixel_array_offset, 4);

    fprintf(stderr, "copying info header\n");
    memcpy(&buf[14], &bitmap->info_header.size, 4);
    memcpy(&buf[18], &bitmap->info_header.width, 4);
    memcpy(&buf[22], &bitmap->info_header.height, 4);
    memcpy(&buf[26], &bitmap->info_header.planes, 2);
    memcpy(&buf[28], &bitmap->info_header.bpp, 2);
    memcpy(&buf[30], &bitmap->info_header.compression, 4);
    memcpy(&buf[34], &bitmap->info_header.pixel_array_size, 4);
    memcpy(&buf[38], &bitmap->info_header.horizontal_ppm, 4);
    memcpy(&buf[42], &bitmap->info_header.vertical_ppm, 4);
    memcpy(&buf[46], &bitmap->info_header.palette_size, 4);
    memcpy(&buf[50], &bitmap->info_header.important_color_count, 4);

    fprintf(stderr, "copying color table (1024)\n");
    memcpy(&buf[54], bitmap->color_table, 1024);


    fprintf(stderr, "copying pixel array\n");
    memcpy(&buf[54 + 1024], bitmap->pixel_array, bitmap->info_header.pixel_array_size);
}


