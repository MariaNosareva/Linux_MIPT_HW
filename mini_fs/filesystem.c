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

  superblock->inode_bitmap = (superblock->inode_bitmap & ~(1 << inode_index));
  superblock->free_inodes_count++;

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

  if (!strcmp(name, "..")) {
    struct inode* parent_inode = (struct inode*) ((union block*) filesystem + 1) + parent_index;
    parent_index = parent_inode->index_of_parent_inode;
  }

  struct inode* parent_inode = (struct inode*) ((union block*) filesystem + 1) + parent_index;

  for (int i = 0; i < parent_inode->current_num_of_files_in_directory; i++) {
    uint8_t inner_inode_index = parent_inode->inodes_indices_in_directory[i];
    struct inode* inner_inode = (struct inode*) ((union block*) filesystem + 1) + inner_inode_index;
    printf("%s\n", inner_inode->name);
  }

}

void cd(void* filesystem, uint8_t* current_directory, char* name) {

  if (!strcmp(name, ".")) {
    return;
  }

  if (!strcmp(name, "..")) {
    struct inode* current_inode = (struct inode*) ((union block*) filesystem + 1) + *current_directory;
    (*current_directory) = current_inode->index_of_parent_inode;
    return;
  }

  int index = find_inode_index_by_name(filesystem, *current_directory, name);
  if (index == -1) {
    printf("No such directory\n");
    return;
  }

  struct inode* inode = (struct inode*) ((union block*) filesystem + 1) + index;
  if (inode->is_directory) {
    printf("%s is not a directory\n", name);
    return;
  }

  (*current_directory) = (uint8_t) index;

}

void touch(void* filesystem, struct superblock* superblock, uint8_t parent_index, char* name) {

  int free_inode_index = find_free_inode_index(superblock);
  if (free_inode_index == -1) {
    printf("Unable to locate file\n");
    return;
  }

  struct inode* parent_inode = (struct inode*) ((union block*) filesystem + 1) + parent_index;

  if (insert_inode_into_directory(parent_inode, (uint8_t) free_inode_index) == -1) {
    printf("Unable to locate file\n");
    return;
  }

  superblock->inode_bitmap = (superblock->inode_bitmap | (1 << free_inode_index));
  superblock->free_inodes_count--;

  struct inode* free_inode = (struct inode*) ((union block*) filesystem + 1) + free_inode_index;
  free_inode->index_of_parent_inode = parent_index;
  free_inode->is_directory = 0;
  free_inode->size_of_file = 0;

  memset(free_inode->name, 0, FILENAME_LENGTH);
  strcpy(free_inode->name, name);

}

uint8_t check_duplicated_names(void* filesystem, struct inode* parent_inode, char* name) {

  for (int i = 0; i < parent_inode->current_num_of_files_in_directory; i++) {
    uint8_t inner_index = parent_inode->inodes_indices_in_directory[i];
    struct inode* inner_inode = (struct inode*) ((union block*) filesystem + 1) + inner_index;
    if (!strcmp(inner_inode->name, name)) {
      return 1;
    }
  }
  return 0;

}

void import_file_from_local(void* filesystem, struct superblock* superblock, uint8_t parent_index, char* name) {

  // TODO edit
  FILE* file = fopen("/home/maria/Dropbox/MIPT/prog/linux2018/Linux_MIPT_HW/mini_fs/test", "r");
  if (file == NULL) {
    printf("No such file\n");
    return;
  }

  int free_inode_index = find_free_inode_index(superblock);
  if (free_inode_index == -1) {
    printf("Unable to locate directory\n");
    fclose(file);
    return;
  }
  struct inode* goal_inode = (struct inode*) ((union block*) filesystem + 1) + free_inode_index;
  strcpy(goal_inode->name, name);

  int number_of_free_blocks = superblock->free_blocks_count;
  char* buffer = malloc(sizeof(union block) * (number_of_free_blocks + 1));
  int read_chars = (int) fread(buffer, sizeof(char), number_of_free_blocks * sizeof(union block), file);

  int num_of_blocks_needed = (int) ceil((double) read_chars / sizeof(union block));
  if ((int) ceil((num_of_blocks_needed - POINTERS_PER_INODE) / POINTERS_PER_BLOCK) + num_of_blocks_needed > number_of_free_blocks) {
    printf("No enough space\n");
    free(buffer);
    fclose(file);
    return;
  }

  struct inode* parent_inode = (struct inode*) ((union block*) filesystem + 1) + parent_index;
  if (insert_inode_into_directory(parent_inode, free_inode_index) == -1) {
    printf("Unable to locate file\n");
    free(buffer);
    fclose(file);
    return;
  }

  write_to_blocks(filesystem, superblock, buffer, goal_inode, num_of_blocks_needed);
  free(buffer);
  fclose(file);
}

void write_to_blocks(void* filesystem, struct superblock* superblock, char* buffer,
                     struct inode* goal_inode, int num_of_blocks_needed) {
  int position_in_buffer = 0;

  for (int i = 0; i < fmin(POINTERS_PER_INODE, num_of_blocks_needed); i++) {
    int index = write_to_block(filesystem, superblock, buffer, &position_in_buffer);
    if (index == -1) {
      printf("Unable to write block of data\n");
      return;
    }
    goal_inode->data_blocks[i] = (uint8_t) index;
  }

  if (num_of_blocks_needed > POINTERS_PER_INODE) {
    int index = find_free_block_index(superblock);
    if (index == -1) {
      printf("Unable to write block of data\n");
      return;
    }
    goal_inode->additional_blocks_index = index;
    superblock->blocks_bitmap[index / 64] = (superblock->blocks_bitmap[index / 64] | (1 << (index - (index / 64) * 64)));
    uint8_t* pointers = (uint8_t*) ((union block*) filesystem + 1 + NUM_BLOCKS_FOR_INODES + index);

    for (int i = 0; i < fmin(POINTERS_PER_BLOCK, num_of_blocks_needed - POINTERS_PER_INODE); i++) {
      int index = write_to_block(filesystem, superblock, buffer, &position_in_buffer);
      if (index == -1) {
        printf("Unable to write block of data\n");
        return;
      }
      pointers[i] = (uint8_t) index;
    }
  }
}

int write_to_block(void* filesystem, struct superblock* superblock, char* buffer, int* position_in_buffer) {
  int index = find_free_block_index(superblock);
  if (index == -1) {
    return index;
  }

  superblock->blocks_bitmap[index / 64] = (superblock->blocks_bitmap[index / 64] | (1 << (index - (index / 64) * 64)));

  union block* block = (union block*) filesystem + 1 + NUM_BLOCKS_FOR_INODES + index;
  memcpy(block, buffer + *position_in_buffer, sizeof(union block));
  *position_in_buffer += sizeof(union block);
  return index;
}

void cat(void* filesystem, uint8_t parent_index, char* name) {

  int index = find_inode_index_by_name(filesystem, parent_index, name);
  if (index == -1) {
    printf("No such file\n");
    return;
  }

  struct inode* goal_inode = (struct inode*) ((union block*) filesystem + 1) + index;
  if (goal_inode->is_directory) {
    printf("%s is a directory\n", name);
    return;
  }

  char* buffer = malloc(sizeof(char) * sizeof(union block));

  for (int i = 0; i < POINTERS_PER_INODE; i++) {
    if (goal_inode->data_blocks[i] == 0) {
      continue;
    }
    union block* data_block = (union block*) filesystem + 1 + NUM_BLOCKS_FOR_INODES + goal_inode->data_blocks[i];
    memcpy(buffer, data_block, sizeof(union block));
    printf("%s", buffer);
  }

  if (goal_inode->additional_blocks_index != 0) {
    uint8_t* pointers = (uint8_t*) ((union block*) filesystem + 1 + NUM_BLOCKS_FOR_INODES + goal_inode->additional_blocks_index);
    int i = 0;
    while (pointers[i] != 0) {
      // TODO
      i++;
    }
  }
  printf("\n");
  free(buffer);

}