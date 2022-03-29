//
// Created by Martin on 2022-01-09.
//

#include "util.h"

#include "libmoor/bitmap.h"

static int convert_raw_to_bmp(char const *path, char const *out_path, char const *palette,
                              int32_t const width, int32_t const height) {
    int rc = 0;

    fprintf(stderr, "converting %s [%dx%d]\n", path, width, height);

    long raw_size = get_file_size(path);
    fprintf(stderr, "raw file: %ld bytes\n", raw_size);

    uint8_t *raw_buf = malloc(sizeof(uint8_t) * raw_size);
    if (raw_buf == NULL) {
        fprintf(stderr, "malloc() returned NULL\n");
        rc = -1;
        goto CONVERT_RETURN;
    }

    rc = load_file(path, raw_buf, raw_size);
    if (rc < 0) {
        fprintf(stderr, "load_file() failed\n");
        rc = -1;
        goto CONVERT_FREE_RAW_BUF;
    }

    uint32_t bmp_size = moor_bitmap_total_size(raw_size);
    fprintf(stderr, "total bitmap raw_size: %ld\n", bmp_size);
    uint8_t *bmp_buf = malloc(sizeof(uint8_t) * bmp_size);
    if (bmp_buf == NULL) {
        fprintf(stderr, "malloc() returned NULL\n");
        rc = -1;
        goto CONVERT_FREE_RAW_BUF;
    }

    moor_bitmap_t *bitmap = malloc(sizeof(moor_bitmap_t));
    if (bitmap == NULL) {
        fprintf(stderr, "malloc() returned NULL\n");
        rc = -1;
        goto CONVERT_FREE_BMP_BUF;
    }

    moor_bitmap_color_table_t pal_buf;
    rc = load_file(palette, pal_buf, 1024);
    if (rc < 0) {
        fprintf(stderr, "load_file() failed\n");
        rc = -1;
        goto CONVERT_FREE_BITMAP;
    }

    /* Buffers are set up, time to create a bitmap! */
    moor_bitmap_init(bitmap, width, height);
    moor_bitmap_set_color_table(bitmap, pal_buf);
    moor_bitmap_set_pixel_array(bitmap, raw_buf, raw_size);
    moor_bitmap_write(bitmap, bmp_buf);

    /* write bitmap buffer to file */
    rc = write_file(out_path, bmp_buf, bmp_size);
    if (rc < 0) {
        fprintf(stderr, "Error writing bitmap buffer to file... anyways-\n");
        goto CONVERT_FREE_BMP_BUF;
    }

    CONVERT_FREE_BITMAP:
    free(bitmap);
    bitmap = NULL;

    CONVERT_FREE_BMP_BUF:
    free(bmp_buf);
    bmp_buf = NULL;

    CONVERT_FREE_RAW_BUF:
    free(raw_buf);
    raw_buf = NULL;

    CONVERT_RETURN:
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
  rc = convert_raw_to_bmp("assets/main.raw", "out.bmp", "assets/moorhuhn.pal", 640, 480);
  if (rc == -1) {
      return -1;
  }

  return 0;
}