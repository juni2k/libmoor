//
// Created by Martin on 21/12/2021.
//

#ifndef LIBMOOR_H
#define LIBMOOR_H

#include <stdint.h>

void moor_hello(void);

/* DAT file */
typedef struct {
  long size;
  uint8_t *buf;
} moor_dat_t;

uint32_t moor_dat_asset_count(moor_dat_t *dat);

/* DAT file asset entry -- 44 bytes (32 + 3*4) */
typedef struct {
  char name[32]; /* filename */
  uint32_t offset; /* offset in DAT */
  uint32_t csize; /* compressed size */
  uint32_t usize; /* uncompressed size */
} moor_dat_entry_t ;

moor_dat_entry_t* moor_dat_get_entry(moor_dat_t *dat, uint32_t index);
int moor_dat_uncompress(moor_dat_t *dat, moor_dat_entry_t *ent, uint8_t *dest, unsigned long dest_len);

#endif