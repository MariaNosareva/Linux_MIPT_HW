//
// Created by maria on 10.10.18.
//

#ifndef MINI_FS_BLOCK_H
#define MINI_FS_BLOCK_H

#include "superblock.h"
#include "inode.h"

union block {
  struct superblock superblock;
  struct inode inodes[INODES_PER_BLOCK];
  uint8_t pointers[POINTERS_PER_BLOCK];
  char data[SIZE_OF_BLOCK / 8];
};

#endif //MINI_FS_BLOCK_H
