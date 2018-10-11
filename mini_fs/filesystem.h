//
// Created by maria on 10.10.18.
//

#ifndef MINI_FS_FILESYSTEM_H
#define MINI_FS_FILESYSTEM_H

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "superblock.h"
#include "block.h"

void* allocate_memory();
void close(void*);
struct superblock* open_filesystem(void*, char*);
struct superblock* initialize(void*);

#endif //MINI_FS_FILESYSTEM_H
