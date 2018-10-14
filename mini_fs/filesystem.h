//
// Created by maria on 10.10.18.
//

#ifndef MINI_FS_FILESYSTEM_H
#define MINI_FS_FILESYSTEM_H

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "superblock.h"
#include "inode.h"
#include "block.h"
#include <math.h>

void* allocate_memory();
void close(void*);
struct superblock* open_filesystem(void*, char*);
struct superblock* initialize(void*);

void add_directory(void*, struct superblock*, uint8_t, char*);
void mkdir(void*, struct superblock*, char*, uint8_t);
int find_inode_index_by_name(void*, uint8_t, char*);
void rmdir(void*, uint8_t, char*);
void ls(void*, uint8_t, char*);
void cd(void*, uint8_t*, char*);
uint8_t check_duplicated_names(void*, struct inode*, char*);
void touch(void* filesystem, struct superblock*, uint8_t, char*);
uint8_t check_duplicated_names(void*, struct inode*, char*);
void import_file_from_local(void*, struct superblock*, uint8_t, char*);
void write_to_blocks(void*, struct superblock*, char*, struct inode*, int);
void cat(void*, uint8_t, char*);

#endif //MINI_FS_FILESYSTEM_H
