//
// Created by Martin on 21/12/2021.
//

#include "libmoor.h"

#include <stdio.h>

void moor_hello(void) {
  printf("Hello from LibMoor!\n");
}

/* DAT implementation */

uint32_t moor_dat_asset_count(moor_dat_t *dat) {
  /* The number of stored assets is in the last 4 bytes */
  return dat->buf[dat->size - 4];
}
