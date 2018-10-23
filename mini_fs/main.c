#include <stdio.h>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include "inode.h"
#include "block.h"
#include "filesystem.h"

#define MAX_COMMAND_LENGTH 255

int main() {

  void* filesystem = allocate_memory();
  open_filesystem(filesystem);
  struct superblock* superblock = (struct superblock*) filesystem;
  uint8_t current_directory = superblock->root_inode_index;

  char command[MAX_COMMAND_LENGTH];
  char filename[FILENAME_LENGTH];

  while (1) {
    printf(" >>>  ");
    scanf("%s", command);

    if (strcmp(command, "touch") == 0) {
      scanf("%s", filename);
      touch(filesystem, superblock, current_directory, filename);
    } else if (strcmp(command, "mkdir") == 0) {
      scanf("%s", filename);
      mkdir(filesystem, superblock, filename, current_directory);
    } else if (strcmp(command, "rmfile") == 0) {
      scanf("%s", filename);
      remove_file(filesystem, superblock, current_directory, filename);
    } else if (strcmp(command, "rmdir") == 0) {
      scanf("%s", filename);
      rmdir(filesystem, current_directory, filename);
    } else if (strcmp(command, "ls") == 0) {
      scanf("%s", filename);
      ls(filesystem, current_directory, filename);
    } else if (strcmp(command, "cd") == 0) {
      scanf("%s", filename);
      cd(filesystem, &current_directory, filename);
    } else if (strcmp(command, "import") == 0) {
      scanf("%s", filename);
      import_file_from_local(filesystem, superblock, current_directory, filename);
    } else if (strcmp(command, "cat") == 0) {
      scanf("%s", filename);
      cat(filesystem, current_directory, filename);
    } else if (strcmp(command, "quit") == 0) {
      close(filesystem);
      break;
    } else {
      printf("No such command\n");
    }
  }

  return 0;
}