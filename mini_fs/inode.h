//
// Created by maria on 08.10.18.
//

#ifndef MINI_FS_INODE_H
#define MINI_FS_INODE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "fs_constants.h"

struct inode {
  char name[FILENAME_LENGTH]; // FILENAME_LENGTH
  uint32_t size_of_file;
  uint8_t is_directory;
  uint8_t inodes_indices_in_directory[INODES_NUM_IN_DIRECTORY];
  uint8_t current_num_of_files_in_directory;
  uint8_t data_blocks[POINTERS_PER_INODE];
  uint8_t additional_blocks_index;
  uint8_t index_of_parent_inode;
};

int insert_inode_into_directory(struct inode*, uint8_t);
int remove_inode_from_directory(struct inode*, uint8_t);

#endif //MINI_FS_INODE_H
