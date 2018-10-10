//
// Created by maria on 11.10.18.
//

#include <stdlib.h>
#include <stdio.h>
#include "filesystem.h"
#include "block.h"

void* allocate_memory() {
  void* filesystem = calloc(TOTAL_NUM_OF_BLOCKS + 1, sizeof(union block));
  return filesystem;
}

void close(void* filesystem) {
  free(filesystem);
}

struct superblock* open_filesystem(void* filesystem, char* filename) {

  FILE* filesystem_by_filename = fopen(filename, "r+t");

  if (filesystem_by_filename == NULL) {
    return initialize(filesystem);
  }

  struct superblock* superblock = (struct superblock*) filesystem;
  fread(superblock, sizeof(superblock), 1, filesystem_by_filename);
  fclose(filesystem_by_filename);
  return superblock;
  
}

struct superblock* initialize(void* filesystem) {

  struct superblock* superblock = (struct superblock*) filesystem;
  superblock->root_inode_index = 0;
  superblock->free_blocks_count = TOTAL_NUM_OF_BLOCKS - NUM_BLOCKS_FOR_INODES;
  superblock->free_inodes_count = NUM_BLOCKS_FOR_INODES * INODES_PER_BLOCK;
  superblock->blocks_for_inodes_count = NUM_BLOCKS_FOR_INODES;
  superblock->blocks_count = TOTAL_NUM_OF_BLOCKS;

  struct inode* root_inode = (struct inode*) ((union block*) filesystem + 1);
  root_inode->name[0] = '/';
  root_inode->is_directory = 1;

}