//
// Created by Martin on 2022-01-09.
//

#include "util.h"

#include "libmoor/bitmap.h"

static int poc_convert_raw_to_bmp(char const *path) {
    int rc = 0;

    fprintf(stderr, "converting [poc, 640x480]: %s\n", path);

    long raw_size = get_file_size(path);
    fprintf(stderr, "raw file: %ld bytes\n", raw_size);

    uint8_t *raw_buf = malloc(sizeof(uint8_t) * raw_size);
    if (raw_buf == NULL) {
        fprintf(stderr, "malloc() returned NULL\n");
        rc = -1;
        goto POC_CONVERT_RETURN;
    }

    rc = load_file(path, raw_buf, raw_size);
    if (rc < 0) {
        fprintf(stderr, "load_file() failed\n");
        rc = -1;
        goto POC_CONVERT_FREE_RAW_BUF;
    }

    uint32_t bmp_size = moor_bitmap_total_size(raw_size);
    fprintf(stderr, "total bitmap raw_size: %ld\n", bmp_size);
    uint8_t *bmp_buf = malloc(sizeof(uint8_t) * bmp_size);
    if (bmp_buf == NULL) {
        fprintf(stderr, "malloc() returned NULL\n");
        rc = -1;
        goto POC_CONVERT_FREE_RAW_BUF;
    }

    moor_bitmap_t *bitmap = malloc(sizeof(moor_bitmap_t));
    if (bitmap == NULL) {
        fprintf(stderr, "malloc() returned NULL\n");
        rc = -1;
        goto POC_CONVERT_FREE_BMP_BUF;
    }

    moor_bitmap_color_table_t pal_buf;
    rc = load_file("assets/moorhuhn.pal", pal_buf, 1024);
    if (rc < 0) {
        fprintf(stderr, "load_file() failed\n");
        rc = -1;
        goto POC_CONVERT_FREE_BITMAP;
    }

    /* Buffers are set up, time to create a bitmap! */
    moor_bitmap_init(bitmap);
    moor_bitmap_set_color_table(bitmap, pal_buf);
    moor_bitmap_set_pixel_array(bitmap, raw_buf, raw_size);
    moor_bitmap_write(bitmap, bmp_buf);

    /* write bitmap buffer to file */
    rc = write_file("out.bmp", bmp_buf, bmp_size);
    if (rc < 0) {
        fprintf(stderr, "Error writing bitmap buffer to file... anyways-\n");
        goto POC_CONVERT_FREE_BMP_BUF;
    }

    POC_CONVERT_FREE_BITMAP:
    free(bitmap);
    bitmap = NULL;

    POC_CONVERT_FREE_BMP_BUF:
    free(bmp_buf);
    bmp_buf = NULL;

    POC_CONVERT_FREE_RAW_BUF:
    free(raw_buf);
    raw_buf = NULL;

    POC_CONVERT_RETURN:
    return rc;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    error("usage: ./moorconvert MOORHUHN.EXE (yes, really)");
  }

  char *exepath;
  exepath = argv[1];

  fprintf(stderr, "this is moorconvert\n");

  int rc;
  rc = poc_convert_raw_to_bmp("assets/main.raw");
  if (rc == -1) {
      return -1;
  }

  return 0;
}