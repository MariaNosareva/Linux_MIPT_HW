//
// Created by maria on 08.10.18.
//

#ifndef MINI_FS_INODE_H
#define MINI_FS_INODE_H

#include <stdint.h>
#include "fs_constants.h"

struct inode {
  char name[12]; // FILENAME_LENGTH
  uint32_t size_of_file;
  uint8_t is_directory;
  uint8_t inodes_indices_in_directory[8];
  uint8_t data_blocks[4];
  uint8_t additional_blocks_index;
};

#endif //MINI_FS_INODE_H
