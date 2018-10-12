#include <stdio.h>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include "inode.h"
#include "block.h"
#include "filesystem.h"

#define MAX_COMMAND_LENGTH 255

int main() {

//  char arr[5] = "abc";
//  char* arr2 = calloc(10);
//  arr2[0] = 'a';
//  arr2[1] = 'b';
//  arr2[2] = 'c';
//  printf("%d\n", strcmp((char*)arr, arr2));
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
      // code
    } else if (strcmp(command, "mkdir") == 0) {
      mkdir(filesystem, superblock, filename, current_directory);
    } else if (strcmp(command, "fremove") == 0) {
      // code
    } else if (strcmp(command, "rmdir") == 0) {
      rmdir(filesystem, current_directory, filename);
    }
  }

  close(filesystem);
  return 0;
}