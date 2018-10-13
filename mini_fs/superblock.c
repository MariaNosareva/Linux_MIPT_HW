//
// Created by maria on 11.10.18.
//

#include "superblock.h"

int find_free_inode_index(struct superblock* superblock) {

  if (superblock->free_inodes_count == 0) {
    return -1;
  }

  uint64_t bitmap = superblock->inode_bitmap;
  uint8_t position = 0;
  uint8_t free = 0;

  while (!free && position < 64) {
    free = 1 ^ ((bitmap & ( 1 << position )) >> position);
    position++;
  }

    position--;
    return position;

}

int get_number_of_free_blocks(struct superblock* superblock) {

  int free = 0;

  for (int i = 0; i < (TOTAL_NUM_OF_BLOCKS - NUM_BLOCKS_FOR_INODES) / 64; i++) {
    uint64_t bitmap = superblock->inode_bitmap;
    uint8_t position = 0;

    while (position < 64) {
      free += 1 ^ ((bitmap & ( 1 << position )) >> position);
      position++;
    }
  }
  return free;

}