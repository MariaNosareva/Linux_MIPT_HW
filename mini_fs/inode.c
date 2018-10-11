//
// Created by maria on 08.10.18.
//

#include "inode.h"

void add_directory(void* filesystem, struct superblock* superblock, uint8_t index_of_parent) {
  int free_inode_index = find_free_inode_index(superblock);
  if (free_inode_index == -1) {
    printf("Unable to locate directory\n");
  }
  superblock->inode_bitmap = superblock->inode_bitmap | (1 << free_inode_index);

  struct inode* free_inode = (struct inode*) ((union block*) filesystem + 1) + free_inode_index;
  free_inode->index_of_parent_inode = index_of_parent;
  free_inode->is_directory = 1;

  struct inode* parent_inode = (struct inode*) ((union block*) filesystem + 1) + index_of_parent;
}

void insert_file_into_directory(struct inode* parent_inode) {
  
}