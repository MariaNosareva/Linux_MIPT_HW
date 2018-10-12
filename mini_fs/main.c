#include <stdio.h>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include "inode.h"
#include "block.h"
#include "filesystem.h"

#define MAX_FILENAME_LENGTH 255
#define MAX_COMMAND_LENGTH 255

int main() {

  void* filesystem = allocate_memory();
  struct superblock* superblock = initialize(filesystem);
  uint8_t current_directory = superblock->root_inode_index;
//  int fd = open("files", O_RDWR);
//  if (fd == -1) {
//    perror("Error opening the device");
//    return -1;
//  }

  char command[MAX_COMMAND_LENGTH];
  char filename[MAX_FILENAME_LENGTH];

  while (strcmp(command, "quit") != 0) {
    printf(" >>>  ");
    scanf("%s %s", command, filename); // TODO get filename char-by-char

    printf("%s", filename);
    if (strcmp(command, "touch") == 0) {
      // code
    } else if (strcmp(command, "mkdir") == 0) {
      mkdir(filesystem, superblock, filename, current_directory);
    } else if (strcmp(command, "fremove") == 0) {
      // code
    } else if (strcmp(command, "dremove") == 0) {
      // code
    }
  }

  close(filesystem);
  return 0;
}