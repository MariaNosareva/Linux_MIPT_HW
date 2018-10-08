//
// Created by maria on 08.10.18.
//

#ifndef MINI_FS_SUPERBLOCK_H
#define MINI_FS_SUPERBLOCK_H

struct superblock {
  int inodes_count;
  int num_of_blocks;
  int free_blocks_count;
  int size_of_block;
  int root_inode_index;
  int blocks_bitmap;
  int inode_bitmap;
};

#endif //MINI_FS_SUPERBLOCK_H
