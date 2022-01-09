#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void error(char *message) {
  fprintf(stderr, "%s\n", message);
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

#endif
