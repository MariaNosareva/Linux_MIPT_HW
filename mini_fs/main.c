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

//  int fd = open("files", O_RDWR);
//  if (fd == -1) {
//    perror("Error opening the device");
//    return -1;
//  }

//  char command[MAX_COMMAND_LENGTH];
//  char filename[MAX_FILENAME_LENGTH];
//
//  while (strcmp(command, "quit") != 0) {
//    printf(" >>>  ");
//    scanf("%s %s", command, filename);
//
//    if (strcmp(command, "touch") == 0) {
//      // code
//    } else if (strcmp(command, "mkdir") == 0) {
//      // code
//    } else if (strcmp(command, "fremove") == 0) {
//      // code
//    } else if (strcmp(command, "dremove") == 0) {
//      // code
//    }
//  }

  void* filesystem = allocate_memory();
  initialize(filesystem);

  uint64_t bitmap = 3;

  uint8_t position = 0;
  uint8_t free = 0;

  while (!free && position < 64) {
    free = 1 ^ ((bitmap & ( 1 << position )) >> position);
    position++;
  }
  if (position < 64) {
    position--;
    printf("%u\n", position);
  }

  return 0;
}