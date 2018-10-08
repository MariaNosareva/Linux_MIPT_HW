//
// Created by maria on 08.10.18.
//

#ifndef MINI_FS_INODE_H
#define MINI_FS_INODE_H

#include <stdint.h>
#include "fs_constants.h"

struct inode {
  char* name; // FILENAME_LENGTH
  uint8_t is_directory;
  uint8_t* inodes_indices_in_directory; // INODES_NUM_IN_DIRECTORY
  uint32_t size_of_file;
  uint8_t* data_blocks; // POINTERS_PER_INODE
  uint8_t additional_blocks_index;
};

#endif //MINI_FS_INODE_H
