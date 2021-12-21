//
// Created by Martin on 21/12/2021.
//

#include "libmoor/libmoor.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

  return 0;
}