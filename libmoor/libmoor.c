//
// Created by Martin on 21/12/2021.
//

#include "libmoor.h"

#include <stdio.h>

#include "zlib.h"

void moor_hello(void) {
  printf("Hello from LibMoor!\n");
}

/* DAT implementation */

uint32_t inline moor_dat_asset_count(moor_dat_t *dat) {
  /* The number of stored assets is in the last 4 bytes */
  return dat->buf[dat->size - 4];
}

/* Return: a pointer to the desired entry or NULL if invalid */
moor_dat_entry_t* moor_dat_get_entry(moor_dat_t *dat, uint32_t index) {
  uint32_t asset_count = moor_dat_asset_count(dat);
  if (index >= asset_count) {
    return NULL;
  }

  long entries_start = dat->size - 4 - (asset_count * sizeof(moor_dat_entry_t));
  return (moor_dat_entry_t *)&dat->buf[entries_start + (index * sizeof(moor_dat_entry_t))];
}

int moor_dat_uncompress(moor_dat_t *dat, moor_dat_entry_t *ent, uint8_t *dest, unsigned long dest_len) {
  if (dest_len < ent->usize) {
    fprintf(stderr, "buffer size mismatch (programmer says: %d, entry says: %d)\n", dest_len, ent->usize);
    return -1;
  }
  int rc = uncompress(dest, &dest_len, &dat->buf[ent->offset], ent->csize);

  if (rc == Z_OK) {
    return 0;
  } else {
    fprintf(stderr, "zlib error: %d\n", rc);
    return -1;
  }
}
