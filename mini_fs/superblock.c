//
// Created by maria on 11.10.18.
//

#include "superblock.h"

int find_free_inode_index(struct superblock* superblock) {

  uint64_t bitmap = superblock->inode_bitmap;
  uint8_t position = 0;
  uint8_t free = 0;

  while (!free && position < 64) {
    free = 1 ^ ((bitmap & ( 1 << position )) >> position);
    position++;
  }
  if (position < 64) {
    position--;
    return position;
  }
  return -1;

}