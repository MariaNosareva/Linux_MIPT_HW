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

int find_free_block_index(struct superblock* superblock) {

  if (superblock->free_blocks_count == 0) {
    return -1;
  }

  int position = 0;

  for (int i = 0; i < (TOTAL_NUM_OF_BLOCKS - NUM_BLOCKS_FOR_INODES) / 64; i++) {
    uint64_t bitmap = superblock->blocks_bitmap[i];
    position = (i == 0)? 1 : 0;
    uint8_t free = 0;

    while (!free && position < 64) {
      free += 1 ^ ((bitmap & ( 1 << position )) >> position);
      position++;
    }

    if (free) {
      position = position + i * 64 - 1;
      break;
    }
  }
  return position;

}