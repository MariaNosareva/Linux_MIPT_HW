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
  initialize(filesystem);
  struct superblock* superblock = (struct superblock*) filesystem;
  uint8_t current_directory = superblock->root_inode_index;
//  int fd = open("files", O_RDWR);
//  if (fd == -1) {
//    perror("Error opening the device");
//    return -1;
//  }
  char command[MAX_COMMAND_LENGTH];
  char filename[FILENAME_LENGTH];

  while (strcmp(command, "quit") != 0) {
    printf(" >>>  ");
    scanf("%s %s", command, filename);

    if (strcmp(command, "touch") == 0) {
      touch(filesystem, superblock, current_directory, filename);
    } else if (strcmp(command, "mkdir") == 0) {
      mkdir(filesystem, superblock, filename, current_directory);
    } else if (strcmp(command, "fremove") == 0) {
      // code
    } else if (strcmp(command, "rmdir") == 0) {
      rmdir(filesystem, current_directory, filename);
    } else if (strcmp(command, "ls") == 0) {
      ls(filesystem, current_directory, filename);
    } else if (strcmp(command, "cd") == 0) {
      cd(filesystem, &current_directory, filename);
    } else if (strcmp(command, "import") == 0) {
      import_file_from_local(filesystem, superblock, current_directory, filename);
    } else if (strcmp(command, "cat") == 0) {
      cat(filesystem, current_directory, filename);
    }
  }

  close(filesystem);
  return 0;
}