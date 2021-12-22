//
// Created by Martin on 21/12/2021.
//

#include "libmoor/libmoor.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void error(char *message) {
  fprintf(stderr, message);
  exit(-1);
}

/* Return: file size in bytes */
long get_file_size(char *path) {
  int rc;

  FILE *fp = fopen(path, "r");
  if (fp == NULL) {
    error("fopen() failed");
  }

  /* Seek to end of file */
  rc = fseek(fp, 0, SEEK_END);
  if (rc != 0) {
    error("could not fseek() to end");
  }

  /* Get position */
  long size = ftell(fp);
  if (size == -1L) {
    error("ftell() failed");
  }

  rc = fclose(fp);
  if (rc == EOF) {
    error("could not close file");
  }

  return size;
}

/* Return: < 0 if unsuccessful */
int load_file(char *path, uint8_t *buf, long count) {
  int success = 0;
  int rc;

  FILE *fp = fopen(path, "rb");
  if (fp == NULL) {
    fprintf(stderr, "fopen() failed\n");
    success = -1;
    goto LOAD_FILE_RETURN;
  }

  size_t read = fread(buf, sizeof(*buf), count, fp);
  if (read != count) {
    fprintf(stderr, "read: %d, expected: %d\n", read, count);
    success = -1;
    goto LOAD_FILE_FCLOSE;
  }

  LOAD_FILE_FCLOSE:
  rc = fclose(fp);
  if (rc == EOF) {
    fprintf(stderr, "fclose() failed\n");
    success = -1;
    goto LOAD_FILE_RETURN;
  }

  LOAD_FILE_RETURN:
  return success;
}

int write_file(char *path, uint8_t *buf, size_t len) {
  fprintf(stderr, "writing file \"%s\" (%d bytes)\n", path, len);

  int success = 0;
  int rc;

  FILE *fp = fopen(path, "wb");
  if (fp == NULL) {
    fprintf(stderr, "fopen() failed\n");
    success = -1;
    goto WRITE_FILE_RETURN;
  }

  unsigned long long written = fwrite(buf, sizeof(*buf), len, fp);
  if (written != len) {
    fprintf(stderr, "wrote: %d, expected: %d\n", written, len);
    success = -1;
    goto WRITE_FILE_FCLOSE;
  }

  WRITE_FILE_FCLOSE:
  rc = fclose(fp);
  if (rc == EOF) {
    fprintf(stderr, "fclose() failed\n");
    success = -1;
    goto WRITE_FILE_RETURN;
  }

  WRITE_FILE_RETURN:
  return success;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    error("usage: ./moorextract MOORHUHN.DAT");
  }

  char *datpath;
  datpath = argv[1];

  fprintf(stderr, "Loading: %s\n", datpath);
  long size = get_file_size(datpath);
  fprintf(stderr, "size: %d bytes\n", size);

  uint8_t *buf = malloc(sizeof(uint8_t) * size);
  if (buf == NULL) {
    error("malloc() returned NULL");
  }
  int rc = load_file(datpath, buf, size);
  if (rc < 0) {
    error("load_file() failed");
  }

  moor_dat_t dat;
  dat.size = size;
  dat.buf = buf;

  uint8_t asset_count = moor_dat_asset_count(&dat);
  fprintf(stderr, "asset count: %d\n", asset_count);

  /* Iterate through the asset entries and try to
   * decompress the listed assets */
  moor_dat_entry_t *ent;
  for (int i = 0; i < asset_count; i++) {
    ent = moor_dat_get_entry(&dat, i);
    if (ent == NULL) {
      fprintf(stderr, "invalid entry: %d, skipping\n", i);
      continue;
    }

    fprintf(stderr,
            "[unpacking asset] %s (offset: %d, c: %d, u: %d)\n",
            ent->name, ent->offset, ent->csize, ent->usize);

    unsigned long asset_buf_len = sizeof(uint8_t) * ent->usize;
    uint8_t *asset_buf = malloc(asset_buf_len);
    if (asset_buf == NULL) {
      fprintf(stderr, "malloc (asset_buf) returned NULL, skipping\n");
      continue;
    }

    rc = moor_dat_uncompress(&dat, ent, asset_buf, asset_buf_len - 1);
    if (rc < 0) {
      fprintf(stderr, "moor_dat_uncompress failed, skipping\n");
      goto FREE_ASSET_BUF;
    }

    mkdir("assets");

    char destpath[16 + sizeof(ent->name)];
    strncpy(destpath, "assets/", 8);
    strncpy(&destpath[7], ent->name, sizeof(ent->name));
    write_file(destpath, asset_buf, asset_buf_len);

    FREE_ASSET_BUF:
    free(asset_buf);
  }


  return 0;
}