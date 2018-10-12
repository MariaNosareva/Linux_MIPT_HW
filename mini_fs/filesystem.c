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

  struct inode* parent_inode = (struct inode*) ((union block*) filesystem + 1) + index_of_parent;

  if (insert_inode_into_directory(parent_inode, free_inode_index) == -1) {
    printf("Unable to locate directory\n");
    return;
  }

  superblock->inode_bitmap = (superblock->inode_bitmap | (1 << free_inode_index));
  superblock->free_inodes_count--;

  struct inode* free_inode = (struct inode*) ((union block*) filesystem + 1) + free_inode_index;
  free_inode->index_of_parent_inode = index_of_parent;
  free_inode->is_directory = 1;

  memset(free_inode->name, 0, FILENAME_LENGTH);
  strcpy(free_inode->name, name);

}

void mkdir(void* filesystem, struct superblock* superblock, char* name, uint8_t current_directory_inode) {
  add_directory(filesystem, superblock, current_directory_inode, name);
}

void remove_directory(void* filesystem, struct superblock* superblock, uint8_t index_of_parent, uint8_t inode_index) {

  struct inode* goal_inode = (struct inode*) ((union block*) filesystem + 1) + inode_index;

  for (int i = 0; i < goal_inode->current_num_of_files_in_directory; i++) {
    uint8_t inner_inode_index = goal_inode->inodes_indices_in_directory[i];
    struct inode* inner_inode = (struct inode*) ((union block*) filesystem + 1) + inner_inode_index;
    if (inner_inode->is_directory) {
      remove_directory(filesystem, superblock, inode_index, inner_inode_index);
    } else {
      // TODO remove_file()
    }
  }
  goal_inode->current_num_of_files_in_directory = 0;
  memset(goal_inode->name, '\0', FILENAME_LENGTH - 1);
  memset(goal_inode->inodes_indices_in_directory, 0, INODES_NUM_IN_DIRECTORY);
  goal_inode->is_directory = 0;
  goal_inode->index_of_parent_inode = 0;

  struct inode* parent_inode = (struct inode*) ((union block*) filesystem + 1) + index_of_parent;
  remove_inode_from_directory(parent_inode, inode_index);

}

int find_inode_index_by_name(void* filesystem, uint8_t parent_index, char* name) {

  struct inode* parent_inode = (struct inode*) ((union block*) filesystem + 1) + parent_index;
  for (int i = 0; i < parent_inode->current_num_of_files_in_directory; i++) {
    uint8_t inner_inode_index = parent_inode->inodes_indices_in_directory[i];
    struct inode* inner_inode = (struct inode*) ((union block*) filesystem + 1) + inner_inode_index;
    if (!strcmp(inner_inode->name, name)) {
      return inner_inode_index;
    }
  }
  return -1;

}

void rmdir(void* filesystem, uint8_t parent_index, char* name) {

  int inode_index = find_inode_index_by_name(filesystem, parent_index, name);
  if (inode_index == -1) {
    printf("No such directory\n");
    return;
  }

  struct superblock* superblock = (struct superblock*) filesystem;
  remove_directory(filesystem, superblock, parent_index, inode_index);

}

void ls(void* filesystem, uint8_t parent_index, char* name) {
  if (strcmp(name, ".")) {
    int index = find_inode_index_by_name(filesystem, parent_index, name);
    if (index == -1) {
      printf("No such directory\n");
      return;
    }
    parent_index = (uint8_t) index;
  }
  struct inode* parent_inode = (struct inode*) ((union block*) filesystem + 1) + parent_index;

  for (int i = 0; i < parent_inode->current_num_of_files_in_directory; i++) {
    uint8_t inner_inode_index = parent_inode->inodes_indices_in_directory[i];
    struct inode* inner_inode = (struct inode*) ((union block*) filesystem + 1) + inner_inode_index;
    printf("%s\n", inner_inode->name);
  }
}