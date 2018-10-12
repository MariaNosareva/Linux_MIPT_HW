//
// Created by maria on 08.10.18.
//

#include "inode.h"

void insert_inode_into_directory(struct inode* parent_inode, uint8_t inode_index) {

  if (parent_inode->current_num_of_files_in_directory >= INODES_NUM_IN_DIRECTORY) {
    printf("Unable to locate directory\n");
    return;
  }

  parent_inode->inodes_indices_in_directory[parent_inode->current_num_of_files_in_directory] = inode_index;
  parent_inode->current_num_of_files_in_directory++;

}