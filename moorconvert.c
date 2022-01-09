//
// Created by Martin on 2022-01-09.
//

#include "util.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    error("usage: ./moorconvert MOORHUHN.EXE (yes, really)");
  }

  char *exepath;
  exepath = argv[1];

  fprintf(stderr, "this is moorconvert (stub)\n");

  return 0;
}