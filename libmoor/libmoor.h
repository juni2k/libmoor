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

#endif