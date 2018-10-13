//
// Created by maria on 08.10.18.
//

#include <memory.h>
#include "inode.h"

int insert_inode_into_directory(struct inode* parent_inode, uint8_t inode_index) {

  if (parent_inode->current_num_of_files_in_directory >= INODES_NUM_IN_DIRECTORY) {
    return -1;
  }

  parent_inode->inodes_indices_in_directory[parent_inode->current_num_of_files_in_directory] = inode_index;
  parent_inode->current_num_of_files_in_directory++;

  return 0;
}

int remove_inode_from_directory(struct inode* parent_inode, uint8_t inode_index) {

  int k = 0;
  for (int i = 0; i < parent_inode->current_num_of_files_in_directory; i++) {
    k = i + 1;
    if (parent_inode->inodes_indices_in_directory[i] == inode_index) {
      parent_inode->inodes_indices_in_directory[i] = 0;
      break;
    }
  }

  for (int i = k; i < parent_inode->current_num_of_files_in_directory; i++) {
    parent_inode->inodes_indices_in_directory[i - 1] = parent_inode->inodes_indices_in_directory[i];
  }
  parent_inode->current_num_of_files_in_directory--;

}

