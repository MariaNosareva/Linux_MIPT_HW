//
// Created by maria on 08.10.18.
//

#ifndef MINI_FS_INODE_H
#define MINI_FS_INODE_H

struct inode {
  int uid;
  char name[255];
  int data_blocks[12];
  int additional_blocks_index;
};

#endif //MINI_FS_INODE_H
