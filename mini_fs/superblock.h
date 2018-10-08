//
// Created by maria on 08.10.18.
//

#ifndef MINI_FS_SUPERBLOCK_H
#define MINI_FS_SUPERBLOCK_H

#include <stdint.h>
#include "fs_constants.h"

struct superblock {
  uint32_t blocks_for_inodes_count;
  uint32_t blocks_count;
  uint32_t free_blocks_count;
  uint32_t free_inodes_count;
  uint32_t root_inode_index;
  uint64_t* blocks_bitmap; // (TOTAL_NUM_OF_BLOCKS - NUM_BLOCKS_FOR_INODES) div 64
  uint64_t inode_bitmap;
};

#endif //MINI_FS_SUPERBLOCK_H
