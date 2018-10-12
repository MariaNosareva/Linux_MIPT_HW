//
// Created by maria on 11.10.18.
//

#include "filesystem.h"

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
  superblock->inode_bitmap = 1;

  struct inode* root_inode = (struct inode*) ((union block*) filesystem + 1);
  root_inode->name[0] = '/';
  root_inode->is_directory = 1;
  root_inode->index_of_parent_inode = 0;

}

void add_directory(void* filesystem, struct superblock* superblock, uint8_t index_of_parent, char* name) {

  int free_inode_index = find_free_inode_index(superblock);
  if (free_inode_index == -1) {
    printf("Unable to locate directory\n");
    return;
  }
  superblock->inode_bitmap = (superblock->inode_bitmap | (1 << free_inode_index));

  struct inode* free_inode = (struct inode*) ((union block*) filesystem + 1) + free_inode_index;
  free_inode->index_of_parent_inode = index_of_parent;
  free_inode->is_directory = 1;

  memset(free_inode->name, 0, FILENAME_LENGTH);
  strcpy(free_inode->name, name);

  struct inode* parent_inode = (struct inode*) ((union block*) filesystem + 1) + index_of_parent;
  insert_inode_into_directory(parent_inode, free_inode_index);

}

void mkdir(void* filesystem, struct superblock* superblock, char* name, uint8_t current_directory_inode) {
  add_directory(filesystem, superblock, current_directory_inode, name);
}
