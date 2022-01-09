//
// Created by Martin on 21/12/2021.
//

#include "libmoor/libmoor.h"

#include "util.h"

#include <string.h>
#include <sys/stat.h>

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

    rc = moor_dat_uncompress(&dat, ent, asset_buf, asset_buf_len);
    if (rc < 0) {
      fprintf(stderr, "moor_dat_uncompress failed, skipping\n");
      goto FREE_ASSET_BUF;
    }

#ifdef __MINGW32__
    mkdir("assets");
#else
    mkdir("assets", 0755);
#endif

    char destpath[16 + sizeof(ent->name)];
    strncpy(destpath, "assets/", 8);
    strncpy(&destpath[7], ent->name, sizeof(ent->name));
    write_file(destpath, asset_buf, asset_buf_len);

    FREE_ASSET_BUF:
    free(asset_buf);
  }


  return 0;
}